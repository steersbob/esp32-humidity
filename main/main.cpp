#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "json.hpp"
#include "mqtt.hpp"
#include "nvs_flash.h"
#include "sensor.hpp"
#include "wifi.hpp"
#include <asio.hpp>
#include <string>

static const char* TAG = "humidity";
std::unique_ptr<asio::steady_timer> task_timer;

extern "C" {
void app_main();
}

void publish_sensor_values()
{
    float temp = humidity::get_temperature();
    float humi = humidity::get_humidity();
    auto obj = json::JSON();

    obj["key"] = "DHT22";
    obj["data"] = json::Object();
    obj["data"]["temperature[degC]"] = temp;
    obj["data"]["humidity[pct]"] = humi;

    ESP_LOGI(TAG, "Temperature %.1f°C | Humidity %.1f%%", temp, humi);
    humidity::publish("brewcast/history/humidity", obj.dump().c_str());
}

void sensor_task(const std::error_code& ec)
{
    if (ec) {
        ESP_LOGI(TAG, "Sensor task error: %s", ec.message());
        return;
    }

    switch (humidity::read_sensor()) {
    case humidity::SensorError::OK:
        publish_sensor_values();
        break;

    case humidity::SensorError::CHECKSUM_ERROR:
        ESP_LOGE(TAG, "Sensor read error: CHECKSUM_ERROR");
        break;

    case humidity::SensorError::TIMEOUT_ERROR:
        ESP_LOGE(TAG, "Sensor read error: TIMEOUT_ERROR");
        break;

    default:
        ESP_LOGE(TAG, "Sensor read error: unknown error");
        break;
    }

    if (task_timer) {
        task_timer->expires_from_now(std::chrono::seconds(5));
        task_timer->async_wait(sensor_task);
    }
}

void app_main()
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    humidity::wifi_init();
    humidity::mqtt_init();

    static asio::io_context ioc;
    task_timer = std::make_unique<asio::steady_timer>(ioc);

    task_timer->expires_from_now(std::chrono::seconds(1));
    task_timer->async_wait(sensor_task);

    ioc.run();
}
