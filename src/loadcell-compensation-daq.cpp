#include "Particle.h"
#include "scale/scale.h"

SYSTEM_MODE(AUTOMATIC);

#define SYS_DELAY_MS 100

ScaleReading scaleReading = {0.0, 0};
char buf[128];

SerialLogHandler logHandler(
    LOG_LEVEL_NONE, // Default logging level for all categories
    {
        {"app", LOG_LEVEL_ALL} // Only enable all log levels for the application
    });

void setup()
{
  Serial.begin(115200);

  Particle.function("tare", tare);
  Particle.function("calibrate", calibrate);
  initializeScale();
}

float knownWeightValue = 0.0;

void loop()
{
  readScale(&scaleReading);
  unsigned long timeSinceTare = getTimeSinceTare();
  float error = knownWeightValue - scaleReading.weight;

  snprintf(buf, sizeof(buf), "%ld,%ld,%f,%f", timeSinceTare, scaleReading.raw, scaleReading.weight, error);
  Serial.println(buf);

  if (Serial.available() > 0)
  {
    String incomingString = Serial.readStringUntil('\n');
    incomingString.trim();
    String cmd = incomingString.substring(0, 1);
    String val = incomingString.substring(1);
    if (cmd == "t" || cmd == "T") // Tare the load cell and reset "time since tare"
    {
      tare("");
    }
    else if (cmd == "c" || cmd == "C") // Calibrate the load cell with a known weight
    {
      calibrate(val);
    }
    else if (cmd == "s" || cmd == "S") // Start the data collection and provide an expected weight
    {
      knownWeightValue = atof(val);
    }
  }
  delay(SYS_DELAY_MS);
}