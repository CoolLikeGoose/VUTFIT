#ifndef TRESHOLD_H
#define TRESHOLD_H

#include "config.h"
#include <stdio.h>
#include <stdbool.h>
#include "driver/gpio.h"

#define TH_TAG "THRESHOLD_DISPLAY"

void init_threshold(float temperature, float hysteresis);
void set_threshold(float temperature);
void set_hysteresis(float hysteresis);
void display_threshold(float temperature);

void init_led();

#endif