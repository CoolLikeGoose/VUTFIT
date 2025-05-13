#ifndef CONFIG_IMP_PRJ
#define CONFIG_IMP_PRJ

#include "esp_log.h"
#include "esp_err.h"

extern int debug;

#define GPIO_LED    2
#define GPIO_TEMP   39

typedef struct config {
    char wifi_ssid[32];
    char wifi_pwd[64];
    char broker_url[128];
    float threshold;
    float hysteresis;
} config_t;

#endif