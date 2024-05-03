#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "GP2Y0A02YK0F.h"

#define analogPin A0 /* ESP8266 Analog Pin ADC0 = A0 */

int adcValue = 0;  /* Variable to store Output of ADC */

const char* ssid = /*<Insert SSID>*/;
const char* password = /*<Insert Password>*/;


//Your Domain name with URL path or IP address with path
String serverName = "<>"; //Your API Address
// String serverName = "http://192.168.137.1:5000/beep-r";
String deviceName = "<>"; //Your Device name

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 50000;

GP2Y0A02YK0F irSensor;
int cm;
uint8_t LED_Pin = D4;

void setup() {
  // irSensor.begin(A0);	//	Assign A0 as sensor   pin
  pinMode(LED_Pin, OUTPUT);   // Initialize the LED pin as an output
  Serial.begin(115200); 

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  digitalWrite(LED_Pin, HIGH);// Turn the LED off
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_Pin, HIGH);// Turn the LED off
    delay(250);
    Serial.print(".");
    digitalWrite(LED_Pin, LOW);// Turn the LED on
    delay(250);
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  digitalWrite(LED_Pin, HIGH);// Turn the LED off
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // cm = irSensor.getDistanceCentimeter();
  // Serial.print("cm = ");
  // Serial.println(cm);
  digitalWrite(LED_Pin, HIGH);// Turn the LED off
  adcValue = 0;
  adcValue = analogRead(analogPin); /* Read the Analog Input value 180 is about 125cm */
  Serial.print("ADC Value = ");
  Serial.println(adcValue);

  delay(50);
  // Send an HTTP POST request depending on timerDelay
  // if ((millis() - lastTime) > timerDelay) {
  // Send an HTTP GET request if ADC is above 200 value
  if (adcValue > 200){ //180
    Serial.print("ADC Value = ");
    Serial.println(adcValue);
    digitalWrite(LED_Pin, LOW); // Turn the LED on
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      String serverPath = serverName+"?device="+deviceName+"&adc="+adcValue;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
  
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
        
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
