
#include <Arduino.h>

#include "Displace.h"

unsigned long times[2];

Displacement Displace(times);

void int_left(void)
{
    times[0]++;
}

void int_right(void)
{
    times[1]++;
}

Displacement::Displacement(unsigned long *times)
{
    _times = times;
    _times[0] = 0;
    _times[1] = 0;
}

void Displacement::begin(void)
{
    attachInterrupt(digitalPinToInterrupt(SENSOR_L), int_left, RISING);
    attachInterrupt(digitalPinToInterrupt(SENSOR_R), int_right, RISING);
}

void Displacement::update(void)
{
    left = (float)_times[0] / 20;
    right = (float)_times[1] / 20;

    _times[0] = 0;
    _times[1] = 0;
}

