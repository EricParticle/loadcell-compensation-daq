#include "temp.h"

const float scalingFactor = 0.01;

void readTemp(float *temp)
{
    float total = 0.0;
    uint8_t samplesAcquired = 0;
    uint8_t averageAmount = 64;
    unsigned long timeout_ms = 2000;

    unsigned long startTime = millis();
    while (1)
    {
        float voltage = (analogRead(TEMP_ANALOG) * 3.3) / 4095.0;
        float temp = voltage / scalingFactor;
        total += temp;
        if (++samplesAcquired == averageAmount)
            break; // All done

        if (millis() - startTime > timeout_ms)
        {
            return;
        }
        delay(1);
    }
    total /= averageAmount;
    *temp = total;
}

void initializeTemp()
{
    pinMode(TEMP_ANALOG, INPUT);
}