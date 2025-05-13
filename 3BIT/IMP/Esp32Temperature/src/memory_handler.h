#ifndef MEMORY_HANDLER_H
#define MEMORY_HANDLER_H

#include "config.h"

#include <nvs_flash.h>
#include <nvs.h>
#include "string.h"

#define ME_TAG "MEMORY_HANDLER"

void init_nvs();
esp_err_t nvs_save_string(const char *key, const char *value);
esp_err_t nvs_get_string(const char *key, char *value, size_t max_length);
config_t config_get_data();

#endif