#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include "config.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

#define AP_TAG "WIFI_CON"

void wifi_init_sta(char *wifi_ssid, char *wifi_pwd);
void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

#endif