


#include <QMC5883LCompass.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <ThingSpeak.h>

const char *readapi_key = "97HNBX83QIQ34OIJ";
const char *writeapi_key = "6XUT2VL5DAKTEVLS";
const char *ssid = "TELUSWiFi2555";
const char *pass = "CGe6dpt4YM";
const char *server = "api.thingspeak.com";
unsigned long channelID = 2986043;
int httpResponseCode = 0;

unsigned long updateTime = 0;
const long updateInterval = 20000;

WiFiClient client;
QMC5883LCompass compass;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  compass.init();
  ThingSpeak.begin(client);
  
  // QMC5883L sketch 
  compass.setCalibrationOffsets(400.00, 1139.00, 1681.00);
  compass.setCalibrationScales(2.68, 0.94, 0.64);

  // join SDA and SCL pins to D21 and D22 (I2C and ESP32 communication)
  Wire.begin(21, 22);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(10000);
    Serial.print("Connecting...");
  }

  Serial.println("WiFi connected!");



  delay(1000);



}

void loop() {
  // put your main code here, to run repeatedly:

  // if the difference btwn time passed since board started running and the update time is greater than update interval, update immediately 
  if (millis() - updateTime >= updateInterval) {
    updateTime = millis();

    compass.read();

    float x = compass.getX();
    float y = compass.getY();
    float z = compass.getZ();
    String xpos = "-1";
    String ypos = "-1";


    Serial.print("X: ");
    Serial.print(x);
    Serial.print("\n");
    Serial.print("Y: ");
    Serial.print(y);
    Serial.print("\n");
    Serial.print("Z: ");
    Serial.print(z);
    Serial.print("\n");


    float magneticIntensity = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    Serial.print("Raw Magnetic Field Intensity: ");
    Serial.println(magneticIntensity);

    float sens = 12000.0; // used to convert LSB units to microteslas

    float intensityMuTesla = (magneticIntensity / sens) * 100.0;

      // set field values
      ThingSpeak.setField(1, x);
      ThingSpeak.setField(2, y);
      ThingSpeak.setField(3, z);
      ThingSpeak.setField(4, intensityMuTesla);
      ThingSpeak.setField(5, xpos);
      ThingSpeak.setField(6, ypos);

    // communicate with ThingSpeak API
    httpResponseCode = ThingSpeak.writeFields(channelID, writeapi_key);
    
    if (httpResponseCode == 200) {

      Serial.println("X position: " + xpos);
      Serial.println("Y position: " + ypos);
      Serial.print("Magnetic Intensity in microteslas: ");
      Serial.println(intensityMuTesla);

      Serial.println("Data being sent to ThingSpeak...");
      // Serial.println(post);




      // GET request to ThingSpeak for live data updates
      // client.print(String("GET ") + post + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n");
      
      Serial.println("Data sent to ThingSpeak.");

      

      
    } else {
      Serial.println("HTTPS ERROR: Cannot connect to ThingSpeak");
    }
  }

    // get CSV data: https://api.thingspeak.com/channels/2986043/feed.csv?api_key=97HNBX83QIQ34OIJ

    delay(1000);

}
