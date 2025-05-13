#include "temp_sensor.h"


adc_oneshot_unit_handle_t adc1_handle;


void init_temp_sensor()
{
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc1_handle));

    adc_oneshot_chan_cfg_t chan_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_3, &chan_config));

    ESP_LOGI(TS_TAG, "Initialization completed");
}


float get_voltage()
{
    int raw_value = -1;
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_3, &raw_value));
    if (raw_value == -1) {
        return -1.0;
    }

    // float voltage = raw_value * 3.3 / 4095.0;
    // Increasing the accuracy of internal ADC readings with an approximation - https://esp32.com/viewtopic.php?f=19&t=43410
    float voltage = 1240.90909091 * ((raw_value / 1333.8762215)); //+0.13
    if (debug) {
        ESP_LOGI(TS_TAG, "ADC Raw: %d, Voltage: %.2fmV", raw_value, voltage);
    }
    
    return voltage;
}

float convert_voltage_to_temp(float voltage)
{
    return (
        (8.194 - sqrt(
            ((-8.194) * (-8.194)) + 4.0 * 0.00262 * (1324.0 - voltage)
        )) 
        / (2.0 * (-0.00262))
    ) + 30;
}

float get_temperature()
{
    float voltage = get_voltage();
    if (voltage == -1.0) {
        ESP_LOGE(TS_TAG, "Failed to get voltage");
        return 0;
    }
    float temperature = convert_voltage_to_temp(voltage);
    if (debug) {
        ESP_LOGI(TS_TAG, "Temperature: %.2f °C", temperature);
    }

    return temperature;
}
