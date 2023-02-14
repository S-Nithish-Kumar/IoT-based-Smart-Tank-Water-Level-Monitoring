#include <HCSR04.h>
HCSR04 hc(13, 12); // Initialize Pin D7, D6
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

 
#define FIREBASE_HOST "led-on-and-off-48ee4-default-rtdb.firebaseio.com" // Firebase host
#define FIREBASE_AUTH "y7b7ySKtT40V8FFKpUWzX6LNfpp9xDIQllz7Ps3H" //Firebase Auth code
#define WIFI_SSID "Saravanan" //Enter your wifi Name
#define WIFI_PASSWORD "12345678" // Enter your password

int distance;
int percent;

void setup()
{
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop()
{
  distance = hc.dist();
  Serial.println(hc.dist()); // Print in centimeters the value from the sensor
  percent = 100-(100*(distance/217));
  Firebase.setInt("Percent", percent);
  delay(50);
}
