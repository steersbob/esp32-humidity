#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt.hpp"
#include "nvs_flash.h"
#include "sensor.hpp"
#include "wifi.hpp"
#include <string>

static const char* TAG = "humidity";

extern "C" {
void app_main();
}

void sensor_task(void* pvParameter)
{
    vTaskDelay(pdMS_TO_TICKS(1000));
    char buf[100];
    float humi = 0;
    float temp = 0;

    while (true) {
        auto err = humidity::read_sensor();
        switch (err) {
        case humidity::SensorError::OK:
            humi = humidity::get_humidity();
            temp = humidity::get_temperature();

            std::snprintf(buf, 100,
                          "{"
                          "\"key\":\"DHT22\","
                          "\"data\":{"
                          "\"temperature[degC]\":%.1f,"
                          "\"humidity[pct]\":%.1f"
                          "}}",
                          temp,
                          humi);

            ESP_LOGI(TAG, "Temperature %.1f°C | Humidity %.1f%%", temp, humi);
            humidity::publish("brewcast/history/humidity", buf);
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

        // -- wait at least 2 sec before reading again ------------
        // The interval of whole process must be beyond 2 seconds !!
        vTaskDelay(pdMS_TO_TICKS(5000));
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

    xTaskCreate(&sensor_task, "Sensor", 2048, NULL, 5, NULL);
}
