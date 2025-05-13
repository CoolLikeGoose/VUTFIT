#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include "config.h"

#include "esp_event.h"
#include "mqtt_client.h"

#define MQ_TAG "MQTT_HANDLER"

void init_mqtt(const char* broker_url);
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_send_temperature(float temperature);

void mqtt_register_threshold_callback(void (*callback)(float));
void mqtt_register_hysteresis_callback(void (*callback)(float));

#endif