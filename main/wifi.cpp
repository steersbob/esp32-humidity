#include "intellisense.hpp"

#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static const char* TAG = "humidity.wifi";

static int s_retry_num = 0;

static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < CONFIG_ESP_WIFI_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "Retrying ...");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
        ESP_LOGI(TAG, "Got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

constexpr char _nibble_d2h(uint8_t bin)
{
    return char(bin + (bin > 9 ? 'A' - 10 : '0'));
}

namespace humidity {

std::string_view device_id()
{
    static std::string id;
    if (id.empty()) {
        std::array<uint8_t, 6> mac;
        if (esp_wifi_get_mac(WIFI_IF_STA, mac.data()) == ESP_OK) {
            id.append("DHT22/MAC");
            for (const auto i : mac) {
                id.push_back(':');
                id.push_back(_nibble_d2h(uint8_t(i & 0xF0) >> 4));
                id.push_back(_nibble_d2h(uint8_t(i & 0xF)));
            }
        }
    }
    return id;
}

void wifi_init()
{
    ESP_LOGI(TAG, "Starting ...");

    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    uint8_t ssid[32] = CONFIG_ESP_WIFI_SSID;
    uint8_t password[64] = CONFIG_ESP_WIFI_PASSWORD;

    // We can't use array initializer syntax due to a GCC bug
    // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55227
    wifi_config_t wifi_config{};
    std::memcpy(wifi_config.sta.ssid, ssid, sizeof(ssid));
    std::memcpy(wifi_config.sta.password, password, sizeof(password));

    ESP_LOGI(TAG, "Connecting to AP SSID:'%s' password:'%s' ...",
             wifi_config.sta.ssid, wifi_config.sta.password);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected");
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed");
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
}

}
