#ifndef TEMP_SENSOR
#define TEMP_SENSOR

#include "config.h"
#include <math.h>
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"

#define TS_TAG "TEMP_SENSOR"

void init_temp_sensor();
float get_voltage();
float convert_voltage_to_temp(float voltage);
float get_temperature();

#endif 