#include "threshold.h"

int led_status = 0;
int thershold_temperature = 0;
int hysteresis_temperature = 0;

void init_led() {
    esp_rom_gpio_pad_select_gpio(GPIO_LED);
    gpio_set_direction(GPIO_LED, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_LED, 0);
}

void init_threshold(float temperature, float hysteresis)
{
    thershold_temperature = temperature;
    hysteresis_temperature = hysteresis;
    init_led();

    ESP_LOGI(TH_TAG, "Initialization completed");
}

void set_threshold(float temperature)
{
    thershold_temperature = temperature;
}

void set_hysteresis(float hysteresis)
{
    hysteresis_temperature = hysteresis;
}

void display_threshold(float temperature)
{
    if (temperature >= thershold_temperature + hysteresis_temperature)
    {
        if (!led_status)
        {
            if (debug) {
                ESP_LOGI(TH_TAG, "Threshold achieved, led is now ON");
            }
            gpio_set_level(GPIO_LED, 1);
            led_status = 1;
        }
    }
    else if (temperature <= thershold_temperature - hysteresis_temperature)
    {
        if (led_status)
        {
            if (debug) {
                ESP_LOGI(TH_TAG, "Threshold achieved, led is now OF");
            }
            gpio_set_level(GPIO_LED, 0);
            led_status = 0;
        }
    }
}
