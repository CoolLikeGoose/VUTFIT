#include "config.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_sntp.h"
#include "time.h"

//components
#include "temp_sensor.h"
#include "threshold.h"
#include "wifi_handler.h"
#include "mqtt_handler.h"
#include "memory_handler.h"

#define TAG "MAIN_TEMPERATURE"

config_t cfg;

//time set via NTP
void obtain_time() {
    ESP_LOGI(TAG, "Initialization SNTP...");
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();

    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;

    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
    tzset();

    while (timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for time synchronization... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGE(TAG, "Cant sync time via NTP");
    } else {
        ESP_LOGI(TAG, "Time successfully synced via NTP: %s", asctime(&timeinfo));
    }
}

void get_timestamp(char* time_str, size_t buf_size) {
    time_t now = time(NULL);
    struct tm timeinfo;

    localtime_r(&now, &timeinfo);
    strftime(time_str, buf_size, "[%Y-%m-%d|%H:%M:%S]", &timeinfo);
}

void init() {
    init_nvs();

    cfg = config_get_data();

    wifi_init_sta(cfg.wifi_ssid, cfg.wifi_pwd);
    obtain_time();
    init_mqtt(cfg.broker_url);
    init_temp_sensor();
    init_threshold(cfg.threshold, cfg.hysteresis);
    mqtt_register_threshold_callback(set_threshold);
    mqtt_register_hysteresis_callback(set_hysteresis);

    ESP_LOGE(TAG, "Initialization of the application completed successfully");
}

int debug = 0;
void app_main() {
    ESP_LOGE(TAG, "Application started");

    init();
    char time_str[32];

    while(1) {
        float temperature = get_temperature();
        mqtt_send_temperature(temperature);
        display_threshold(temperature);
        
        get_timestamp(time_str, sizeof(time_str));

        nvs_save_string("temp_record", time_str);
        printf("%s: %.2f°C\n", time_str, temperature);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}