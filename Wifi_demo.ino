#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//#include "GP2Y0A02YK0F.h"

#define analogPin A0 /* ESP8266 Analog Pin ADC0 = A0 */
#define externalLED D3 //0 is turn on
int adcValue = 0;  /* Variable to store Output of ADC */

#define TIME_DELAY 1000
#define ENCRYPTION false

const char* ssid = <SSID>;
const char* password = <SSID_Password>;


//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.137.1/iot"; //Your API Address


// String serverName = "http://192.168.137.1:5000/beep-r";
String deviceName = "demo1"; //Your Device name

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 50000;

//GP2Y0A02YK0F irSensor;
int cm;
uint8_t LED_Pin = D4;

void setup() {
  // irSensor.begin(A0);	//	Assign A0 as sensor   pin
  pinMode(LED_Pin, OUTPUT);   // Initialize the LED pin as an output
  pinMode(externalLED, OUTPUT);   // Initialize the LED pin as an output
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
  digitalWrite(LED_Pin, HIGH);// Turn the LED off

  // Do the data collection here
  adcValue = 0;
  adcValue = analogRead(analogPin); 

  if(ENCRYPTION == true){
    //do the encryption here
    
  }

  delay(TIME_DELAY);
  // Send an HTTP POST request depending on timerDelay
  if (true){
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
        if(ENCRYPTION == true){
          //do the decryption here
          
        }
        if(payload == "[1]"){
          Serial.println("on led");
          digitalWrite(externalLED, LOW);
        }
        else{
          Serial.println("off LED");
          digitalWrite(externalLED, HIGH);
        }
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
