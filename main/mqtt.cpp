#include "intellisense.hpp"

#include "esp_event.h"
#include "esp_log.h"
#include "mqtt.hpp"
#include "mqtt_client.h"

static const char* TAG = "humidity.mqtt";

static esp_mqtt_client_handle_t s_mqtt_client = NULL;

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_t* event)
{
    // your_context_t *context = event->context;
    switch (event->event_id) {
    case MQTT_EVENT_BEFORE_CONNECT:
        break;
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "Connected");
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "Disconnected");
        break;
    case MQTT_EVENT_PUBLISHED: // Not received when publishing with qos=0
        ESP_LOGI(TAG, "Published, msg_id=%ld", event->msg_id);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "Error");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void* handler_args,
                               esp_event_base_t base,
                               int32_t event_id,
                               void* event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(static_cast<esp_mqtt_event_t*>(event_data));
}

namespace humidity {

void mqtt_init()
{
    esp_mqtt_client_config_t mqtt_cfg{
        .broker{
            .address{
                .uri = CONFIG_BROKER_URL,
            }}};

    ESP_LOGI(TAG, "Connecting to broker:'%s' ...", CONFIG_BROKER_URL);

    s_mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(s_mqtt_client,
                                   esp_mqtt_event_id_t::MQTT_EVENT_ANY,
                                   mqtt_event_handler,
                                   s_mqtt_client);
    esp_mqtt_client_start(s_mqtt_client);
}

void publish(const char* topic, const char* data)
{
    esp_mqtt_client_publish(s_mqtt_client, topic, data, 0, 0, 0);
}

}
