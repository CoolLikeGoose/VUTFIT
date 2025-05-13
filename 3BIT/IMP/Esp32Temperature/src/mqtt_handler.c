#include "mqtt_handler.h"

static void (*threshold_callback)(float) = NULL;
static void (*hysteresis_callback)(float) = NULL;
esp_mqtt_client_handle_t client;
int mqtt_conection = 0;

void init_mqtt(const char* broker_url)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = broker_url,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    if (client == NULL) {
        ESP_LOGE(MQ_TAG, "Failed to create MQTT client");
    }

    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    ESP_ERROR_CHECK(esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL));
    ESP_ERROR_CHECK(esp_mqtt_client_start(client));

    ESP_LOGI(MQ_TAG, "Initialization completed");
}

void mqtt_register_threshold_callback(void (*callback)(float)) {
    threshold_callback = callback;
}

void mqtt_register_hysteresis_callback(void (*callback)(float)) {
    hysteresis_callback = callback;
}

void mqtt_send_temperature(float temperature) {
    if (!mqtt_conection) return;

    char topic[32], msg[32];
    snprintf(topic, sizeof(topic), "xsmirn02/temperature");
    snprintf(msg, sizeof(msg), "%.2f", temperature);
    esp_mqtt_client_publish(client, topic, msg, 0, 1, 0);

    if (debug) {
        ESP_LOGI(MQ_TAG, "Temperature Information sent");
    }
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(MQ_TAG, "Connected to server");
            mqtt_conection = 1;

            esp_mqtt_client_subscribe(client, "xsmirn02/temp_threshold", 0);
            ESP_LOGI(MQ_TAG, "Subscribed to topic: xsmirn02/temp_threshold");

            esp_mqtt_client_subscribe(client, "xsmirn02/temp_hysteresis", 0);
            ESP_LOGI(MQ_TAG, "Subscribed to topic: xsmirn02/temp_hysteresis");
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(MQ_TAG, "Disconected from server");
            mqtt_conection = 0;
            break;

        case MQTT_EVENT_DATA:
            if (debug) {
                ESP_LOGI(MQ_TAG, "MQTT message received on topic: %.*s", event->topic_len, event->topic);
            }

            if (strncmp(event->topic, "xsmirn02/temp_threshold", event->topic_len) == 0) {
                float new_threshold = atof(event->data);
                if (threshold_callback) {
                    threshold_callback(new_threshold);
                }
                ESP_LOGI(MQ_TAG, "Threshold updated: %.2f", new_threshold);
            } else if (strncmp(event->topic, "xsmirn02/temp_hysteresis", event->topic_len) == 0) {
                float new_hysteresis = atof(event->data);
                if (hysteresis_callback) {
                    hysteresis_callback(new_hysteresis);
                }
                ESP_LOGI(MQ_TAG, "Hysteresis updated: %.2f", new_hysteresis);
            }
            break;

          default:
               break;
     }
}
