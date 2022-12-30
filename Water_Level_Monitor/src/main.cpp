#include <Arduino.h>

int sensorPin = A0;               // Measure on Analog 0 pin
int avgReadings = 16;             // Number of readings to take per cycle
int minSensorADCReading = 117;    // Minimum ADC reading with 0cm of water
float minSensorADCVoltage = 0.57; // Minimum ADC voltage reading with 0cm of water
float OffSet = 2.50;              // Water height start 0 (in cm)
float maxTankHeight = 24.5;       // Tank water height at 100% (in cm)
float maxTankVolume = 9000.0;     // Tank Capacity at 100% (in mL)

// FUNCTIONS //
int getADCReading();
void displayReadings();
float mapfloat(float value, float in_min, float in_max, float out_min, float out_max);

// SETUP //
void setup()
{
  Serial.begin(115200); // Start serial communication at a baud rate of 115200
}

// MAIN LOO{P} //
void loop()
{
  displayReadings(); //  Get sensor readings and display them

  delay(1000); // Pause for 1 second after each reading
}

// GET ADC READ //
int getADCReading()
{
  int rawADC = 0;
  for (int i = 0; i < avgReadings; i++)
  {
    rawADC += analogRead(A0);
  }
  rawADC = rawADC / avgReadings;
  return rawADC;
}

// MAPPING FUNCITON //
float mapFloat(float value, float in_min, float in_max, float out_min, float out_max)
{
  return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// CALCULATIONS AND DISPLAY //
void displayReadings()
{
  int rawSensorReading = getADCReading();
  float rawADCVoltage = rawSensorReading * (5.0 / 1023.0);
  float mappedRawADC = map(rawSensorReading, minSensorADCReading, 1023, 0, 500);
  float mappedRawADCVoltage = mapFloat(rawADCVoltage, minSensorADCVoltage, 5.00, 0.00, 500.0);
  float errorCorrectedValue = mappedRawADCVoltage + OffSet;
  float rawLiters = mapFloat(errorCorrectedValue, 0.0, maxTankHeight, 0.00, maxTankVolume);
  float waterLevelPercentage = (rawLiters / maxTankVolume) * 100.0;

  if (mappedRawADC > 0)
  {
    Serial.println((String) "Raw ADC: " + rawSensorReading + " | Raw ADC Voltage: " + rawADCVoltage + "V | Mapped ADC Level: " + mappedRawADC + "cm | Mapped ADC Voltage Level: " + mappedRawADCVoltage + "cm | Actual Level: " + errorCorrectedValue + "cm | Water Capacity: " + rawLiters + "mL | Water Level: " + waterLevelPercentage + "%");
  }
  else
  {
    Serial.println("Sensor cannot detect any water!!!");
  }
}