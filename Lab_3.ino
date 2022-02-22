#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <SparkFunLSM9DS1.h>

#include "SparkFunBME280.h"


// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
#define chipSelect 4
File dataFile;
LSM9DS1 imu;
BME280 externalSensor;
BME280 internalSensor;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  //Serial.println("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);

  //digitalWrite(10, HIGH);

  // The chipSelect pin you use should also be set to output
  pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  else{
    SD.remove("data.csv");
  }
  //Serial.println("Card initialized.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open("data.csv", FILE_WRITE);
  String header = "Time Elapsed s,Int Temp C,Ext Temp C,Ext Pressure,Altitude Meters,Ext Humidity %RH,Gx (deg/s),Gy (deg/s),Gz (deg/s),Ax (g),Ay (g),Az(g),Mx (gauss),My(guass),Mz(gauss),Current(mA)"; //update as more sensors added
  if (dataFile) {
      dataFile.println(header);
      dataFile.flush();
  }
  //Serial.println(header);
  //Now to initialize BME sensor:
  
  Wire.begin();
  externalSensor.setI2CAddress(0x77); //need to close a jumper to change address to 0x76
  internalSensor.setI2CAddress(0x76); //second sensor

  if (externalSensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("External Sensor connect failed");
    //while(1); //freeze
  }
  if (internalSensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("Internal Sensor connect failed");
    //while(1); //freeze
  }
  if (imu.begin() == false) //this uses default addresses AG = 0x6B and M = 0x1E) and I2C port
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    while(1); //freeze
  }
  Serial.println("All sensors initialized");
}

void loop()
{
  // make a string for assembling the data to log:
  String dataString = "";
  // if the file is available, write to it:
  if (dataFile) {
    for (int i = 0; i < 4; i++){
      // read sensor and append to the string:
      dataString += String(millis()/1000) + ",";
      //BME sensor data:
      dataString += String(internalSensor.readTempC()) + ",";
      dataString += String(externalSensor.readTempC()) + "," + String(externalSensor.readFloatPressure()) + "," + String(externalSensor.readFloatAltitudeMeters()) + "," + String(externalSensor.readFloatHumidity()) + ",";
      //IMU sensor data:
      if (imu.gyroAvailable()){
        imu.readGyro();
      }
      
      if (imu.accelAvailable()){
        imu.readAccel();
      }
      if (imu.magAvailable()){
        imu.readMag();
      }
      dataString += String(imu.calcGyro(imu.gx)) + "," + String(imu.calcGyro(imu.gy)) + "," + String(imu.calcGyro(imu.gz)) + ",";
      dataString += String(imu.calcAccel(imu.ax)) + "," + String(imu.calcAccel(imu.ay)) + "," + String(imu.calcAccel(imu.az)) + ",";
      dataString += String(imu.calcMag(imu.mx)) + "," + String(imu.calcMag(imu.my)) + "," + String(imu.calcMag(imu.mz)) + ",";

      dataFile.println(dataString);

      //print to serial monitor too:
      //Serial.println(dataString);
      delay(1000); //how long before data points taken
      dataString = ""; //clear data string again before rereading
      }
    dataFile.flush();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening data.csv");
    while(1); //freeze
  }
}
