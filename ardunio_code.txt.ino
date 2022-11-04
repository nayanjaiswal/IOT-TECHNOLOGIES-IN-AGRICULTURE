#include <WiFi.h>
 #include <HTTPClient.h>


#define AOUT_PIN 36 // ESP32 pin GIOP36 (ADC0) that connects to AOUT pin of moisture sensor ---> soil moisture sensor
#define THRESHOLD 1000 // CHANGE YOUR THRESHOLD HERE
#define DHT11PIN 16


#include "DHT.h"

// Replace with your network credentials
const char* ssid     = "ESP";
const char* password = "nayanjaiswalji";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://192.168.219.133/nayanproject/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

String SensorName = "Capacitive_Soil_Moisture";
String Sensorname = "DH11_Sensor";
String soilStatus = "";
String sensorLocation = "My Garden";
DHT dht(DHT11PIN, DHT11);

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());


 
 // Soil Mosture sensor
 
  

   dht.begin();

}
void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  float humi = dht.readHumidity();
  float temp = dht.readTemperature();

  int value = analogRead(AOUT_PIN); // read the analog value from sensor
  
  if (value < THRESHOLD)
  
   soilStatus = "The soil is DRY (" ;
  else
    soilStatus = "The soil is WET (";

   
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + SensorName
                          + "&location=" + sensorLocation + "&value1=" + String(dht.readHumidity())
+ "&value2=" + String(value) + "&value3=" + String(dht.readTemperature()) + "";

  
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
 // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     






    
    
    
    
  if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
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
  //Send an HTTP POST request every 15 seconds
  delay(15000);  
}
