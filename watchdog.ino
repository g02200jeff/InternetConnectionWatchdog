#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "YOURSSID";
const char* password = "YOURPASSWORD";
const int relayPin = 4;
bool isRestarting = false;

WiFiClient client;

void setup() 
{
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);       // RELAY OFF
  delay(1000);
  Serial.println("RelayPin ok");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("WiFi connection...");
  }
  Serial.println("Connection OK");
}

void loop() 
{
  if (WiFi.status() == WL_CONNECTED)
   {
    HTTPClient http;
    String url = "http://192.168.10.1"; //IP of internet device
    http.begin(client,url);  
    int httpCode = http.GET();
    if (httpCode != 200) {
      Serial.println("Device not found, restarting...");
      isRestarting = true;
      digitalWrite(relayPin, LOW);    // RELAY ON
      delay(5000);                    // Waint 5s
      digitalWrite(relayPin, HIGH);   // RELAY OFF
    }
    else
    {
      if(isRestarting)
      {
        isRestarting = false;
        Serial.println("Device not found");
      }
    }
    http.end();
  }
  delay(60000);                       //Check every minute
}
