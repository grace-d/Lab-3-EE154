/*  SparkFun ACS712 Demo
    Created by George Beckstein for SparkFun
    4/30/2017

    Uses an Arduino to set up the ACS712 Current Sensor
    See the product page at: https://www.sparkfun.com/products/8883

    Parts you may need:
    - 100 Ohm, 1/2W or greater resistor OR two 220 Ohm 1/4 resistors in parallel
    - ACS712 Breakout with on-board amplifier

    Optional equipment:
    - Oscilloscope
    - Multimeter (or two)
    - A power supply with current limiting/constant current would be handy to calibrate the device without using resistors
*/

const int analogPin = A0;

const float resistor_value = 1.5;
// Number of samples to average the reading over
// Change this to make the reading smoother... but beware of buffer overflows!
const int avgSamples = 10;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  float voltage_difference = 0;
  for (int i = 0; i < avgSamples; i++)
  {
    voltage_difference += analogRead(analogPin);
    Serial.println("Voltage diff " + String(voltage_difference));
    // wait 2 milliseconds before the next loop
    // for the analog-to-digital converter to settle
    // after the last reading:
    delay(2);

  }
  float voltage_drop = voltage_difference/avgSamples;
  Serial.println(String(voltage_drop) + "mV");
  float current = voltage_drop / resistor_value;
  Serial.println(String(current) + "mA");
  delay(1000);
}
