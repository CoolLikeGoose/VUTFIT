#include "memory_handler.h"

void init_nvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

esp_err_t nvs_save_string(const char *key, const char *value) {
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(ME_TAG, "Error opening NVS handle: %s", esp_err_to_name(err));
        return err;
    }

    err = nvs_set_str(nvs_handle, key, value);
    if (err != ESP_OK) {
        ESP_LOGE(ME_TAG, "Error setting string: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return err;
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(ME_TAG, "Error committing NVS changes: %s", esp_err_to_name(err));
    }

    nvs_close(nvs_handle);
    return err;
}

esp_err_t nvs_get_string(const char *key, char *value, size_t max_length) {
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(ME_TAG, "Error opening NVS handle: %s", esp_err_to_name(err));
        return err;
    }

    size_t required_size;
    err = nvs_get_str(nvs_handle, key, NULL, &required_size);
    if (err != ESP_OK) {
        ESP_LOGE(ME_TAG, "Error getting string size: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return err;
    }

    if (required_size > max_length) {
        ESP_LOGE(ME_TAG, "Buffer too small for string. Required: %d, Provided: %d", required_size, max_length);
        nvs_close(nvs_handle);
        return ESP_ERR_NVS_INVALID_LENGTH;
    }

    err = nvs_get_str(nvs_handle, key, value, &required_size);
    if (err != ESP_OK) {
        ESP_LOGE(ME_TAG, "Error getting string: %s", esp_err_to_name(err));
    }

    nvs_close(nvs_handle);
    return err;
}

config_t config_get_data()
{
    config_t cfg = { 0 };
    char temp_value[128];

    if (CONFIG_WIFI_SSID != NULL && strlen(CONFIG_WIFI_SSID) > 0) {
        strncpy(cfg.wifi_ssid, CONFIG_WIFI_SSID, sizeof(cfg.wifi_ssid) - 1);
        ESP_LOGI(ME_TAG, "Loaded WiFi SSID from CONFIG: %s", cfg.wifi_ssid);
        nvs_save_string("wifi_ssid", CONFIG_WIFI_SSID);
    } else if (nvs_get_string("wifi_ssid", temp_value, sizeof(temp_value)) == ESP_OK) {
        strncpy(cfg.wifi_ssid, temp_value, sizeof(cfg.wifi_ssid) - 1);
        ESP_LOGI(ME_TAG, "Loaded WiFi SSID from NVS: %s", cfg.wifi_ssid);
    } else {
        ESP_LOGW(ME_TAG, "WiFi SSID not found in CONFIG or NVS");
    }

    if (CONFIG_WIFI_PWD != NULL && strlen(CONFIG_WIFI_PWD) > 0) {
        strncpy(cfg.wifi_pwd, CONFIG_WIFI_PWD, sizeof(cfg.wifi_pwd) - 1);
        ESP_LOGI(ME_TAG, "Loaded WiFi password from CONFIG: %s", cfg.wifi_pwd);
        nvs_save_string("wifi_pwd", CONFIG_WIFI_PWD);
    } else if (nvs_get_string("wifi_pwd", temp_value, sizeof(temp_value)) == ESP_OK) {
        strncpy(cfg.wifi_pwd, temp_value, sizeof(cfg.wifi_pwd) - 1);
        ESP_LOGI(ME_TAG, "Loaded WiFi password from NVS: %s", cfg.wifi_pwd);
    } else {
        ESP_LOGW(ME_TAG, "WiFi password not found in CONFIG or NVS");
    }

    if (CONFIG_MQTT_BROKER_URL != NULL && strlen(CONFIG_MQTT_BROKER_URL) > 0) {
        strncpy(cfg.broker_url, CONFIG_MQTT_BROKER_URL, sizeof(cfg.broker_url) - 1);
        ESP_LOGI(ME_TAG, "Loaded MQTT broker URL from CONFIG: %s", cfg.broker_url);
        nvs_save_string("broker_url", CONFIG_MQTT_BROKER_URL);
    } else if (nvs_get_string("broker_url", temp_value, sizeof(temp_value)) == ESP_OK) {
        strncpy(cfg.broker_url, temp_value, sizeof(cfg.broker_url) - 1);
        ESP_LOGI(ME_TAG, "Loaded MQTT broker URL from NVS: %s", cfg.broker_url);
    } else {
        ESP_LOGW(ME_TAG, "MQTT broker URL not found in CONFIG or NVS");
    }

    if (atof(CONFIG_TEMPERATURE_THRESHOLD) > 0) {
        cfg.threshold = atof(CONFIG_TEMPERATURE_THRESHOLD);
        ESP_LOGI(ME_TAG, "Loaded temperature threshold from CONFIG: %.2f", cfg.threshold);
        nvs_save_string("temp_th", CONFIG_TEMPERATURE_THRESHOLD);
    } else if (nvs_get_string("temp_th", temp_value, sizeof(temp_value)) == ESP_OK) {
        cfg.threshold = atof(temp_value);
        ESP_LOGI(ME_TAG, "Loaded temperature threshold from NVS: %.2f", cfg.threshold);
    } else {
        ESP_LOGW(ME_TAG, "Temperature threshold not found in CONFIG or NVS");
        cfg.threshold = 0.0f;
    }

    if (atof(CONFIG_HYSTERESIS) > 0) {
        cfg.hysteresis = atof(CONFIG_HYSTERESIS);
        ESP_LOGI(ME_TAG, "Loaded temperature hysteresis from CONFIG: %.2f", cfg.hysteresis);
        nvs_save_string("temp_hyst", CONFIG_HYSTERESIS);
    } else if (nvs_get_string("temp_hyst", temp_value, sizeof(temp_value)) == ESP_OK) {
        cfg.hysteresis = atof(temp_value);
        ESP_LOGI(ME_TAG, "Loaded temperature hysteresis from NVS: %.2f", cfg.hysteresis);
    } else {
        ESP_LOGW(ME_TAG, "Temperature hysteresis not found in CONFIG or NVS");
        cfg.hysteresis = 0.0f;
    }

    return cfg;
}