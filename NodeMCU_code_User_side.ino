#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include<SoftwareSerial.h>
SoftwareSerial s(3,1);


#define FIREBASE_HOST "led-on-and-off-48ee4-default-rtdb.firebaseio.com" // Firebase host
#define FIREBASE_AUTH "y7b7ySKtT40V8FFKpUWzX6LNfpp9xDIQllz7Ps3H" //Firebase Auth code
#define WIFI_SSID "Saravanan" //Enter your wifi Name
#define WIFI_PASSWORD "12345678" // Enter your password

int d;
int distance;
int percent;
int p=0;
int pre=0;


void setup()
{
  Serial.begin(9600);
  s.begin(9600);

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
  p = Firebase.getInt("Percent");
  if(pre != p)
  {
  s.write(int(p));
  pre = p;
  }
    delay(2000);}
