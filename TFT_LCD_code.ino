#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
// *** SPFD5408 change -- End

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

// Calibrate values
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//#define BOXSIZE 40
#define PENRADIUS 3

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define BLACK 0x0000
#define NAVY 0x000F
#define DARKGREEN 0x03E0
#define DARKCYAN 0x03EF
#define MAROON 0x7800
#define PURPLE 0x780F
#define OLIVE 0x7BE0
#define LIGHTGREY 0xC618
#define DARKGREY 0x7BEF
#define BLUE 0x001F
#define GREEN 0x07E0
#define CYAN 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define GREENYELLOW 0xAFE5
#define PINK 0xF81F
int h;
int per=0;
int check=0;
int percent;
int curr_percent1=0;
int pre_percent1=0;
int pre_heigh;
int diff;
int data;

long duration;
int distance;


void setup()
{
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  
  Serial.begin(9600);
  Serial1.begin(9600);
  tft.reset();
  tft.begin(0x9341); 
  tft.setRotation(3);
  
  drawBorder();
  
  // Initial screen
  
  tft.setCursor (100, 20);
  tft.setTextSize (3);
  tft.setTextColor(GREEN);
  tft.println("WELCOME");
  tft.setCursor (20, 70);
  tft.setTextSize (3);
  tft.setTextColor(RED);
  tft.println("Tank water level");
  tft.setCursor (65, 100);
  tft.println("Monitoring");
  tft.setCursor (65, 185);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println("Touch to proceed");
  waitOneTouch();

  tft.fillScreen(ORANGE); 
  // height 320, width 240
  tft.drawFastVLine(31,71,140,BLACK);
  tft.drawFastVLine(32,71,140,BLACK);
  tft.drawFastVLine(33,71,140,BLACK);
  tft.drawFastVLine(34,71,140,BLACK);
  tft.drawFastVLine(35,71,140,BLACK);
  tft.drawFastVLine(169,71,140,BLACK);
  tft.drawFastVLine(168,71,140,BLACK);
  tft.drawFastVLine(167,71,140,BLACK);
  tft.drawFastVLine(166,71,140,BLACK);
  tft.drawFastVLine(165,71,140,BLACK);
  tft.drawFastHLine(36,206,130,BLACK);
  tft.drawFastHLine(36,207,130,BLACK);
  tft.drawFastHLine(36,208,130,BLACK);
  tft.drawFastHLine(36,209,130,BLACK);
  tft.drawFastHLine(36,210,130,BLACK);
  tft.drawFastHLine(80,50,45,BLACK);
  tft.drawFastHLine(80,49,45,BLACK);
  tft.drawFastHLine(80,48,45,BLACK);
  tft.drawFastHLine(80,47,45,BLACK);
  tft.drawFastHLine(80,46,45,BLACK);
  tft.drawLine(31,72,79,46,BLACK);
  tft.drawLine(32,72,79,47,BLACK);
  tft.drawLine(33,72,79,48,BLACK);
  tft.drawLine(34,72,79,49,BLACK);
  tft.drawLine(35,72,79,50,BLACK);
  tft.drawLine(169,72,125,46,BLACK);
  tft.drawLine(168,72,125,47,BLACK);
  tft.drawLine(167,72,125,48,BLACK);
  tft.drawLine(166,72,125,49,BLACK);
  tft.drawLine(165,72,125,50,BLACK);
  tft.setCursor(228,9);
  tft.setTextSize(2);
  //tft.setTextColor(WHITE,BLACK);
  tft.print("TANK");
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.fillCircle(250,60,30,DARKGREY);
  tft.drawCircle(250,60,31,BLACK);
  tft.drawCircle(250,60,32,BLACK);
  tft.drawCircle(250,60,33,BLACK);
  tft.setCursor(244,50);
  tft.print("1");
  tft.fillCircle(250,130,30,DARKGREY);
  tft.drawCircle(250,130,31,BLACK);
  tft.drawCircle(250,130,32,BLACK);
  tft.drawCircle(250,130,33,BLACK);
  tft.setCursor(242,120);
  tft.print("2");
  tft.fillCircle(250,200,30,DARKGREY);
  tft.drawCircle(250,200,31,BLACK);
  tft.drawCircle(250,200,32,BLACK);
  tft.drawCircle(250,200,33,BLACK);
  tft.setCursor(242,190);
  tft.print("3"); 
  tft.setRotation(3);
}

void loop()
{
  TSPoint p = ts.getPoint(); 
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
  {
    p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
    // *** SPFD5408 change -- End
    p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());

    //Serial.print(p.x, p.y);
    
       if ((p.y > 20) && (p.y < 90) && (p.x > 50) && (p.x < 115))
     {
        tank1Monitor();
     }
        if ((p.y > 20) && (p.y < 90) && (p.x > 120) && (p.x < 175))
     {
        tank2Monitor();
     }
        if ((p.y > 20) && (p.y < 90) && (p.x > 180) && (p.x < 240))
     {
        tank3Monitor();
     }
  }
}

void drawBorder() 
{

  // Draw a border

  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;

  tft.fillScreen(RED);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);
  
}

TSPoint waitOneTouch() 
{

  // wait 1 touch to exit function
  
  TSPoint p;
  
  do {
    p= ts.getPoint(); 
  
    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);
  
  } while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));
  
  return p;
}

void tank1ON()
{
  tft.fillCircle(250, 60, 30, GREEN);
  tft.setCursor(244,50);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("1");

  tft.fillCircle(250,130,30,DARKGREY);
  tft.setCursor(242,120);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("2");

  tft.fillCircle(250, 200, 30, DARKGREY);
  tft.setCursor(242,190);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("3");
}

void tank2ON()
{
  tft.fillCircle(250, 60, 30, DARKGREY);
  tft.setCursor(244,50);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("1");

  tft.fillCircle(250,130,30,GREEN);
  tft.setCursor(242,120);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("2");

  tft.fillCircle(250, 200, 30, DARKGREY);
  tft.setCursor(242,190);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("3");
}

void tank3ON()
{
  tft.fillCircle(250, 60, 30, DARKGREY);
  tft.setCursor(244,50);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("1");

  tft.fillCircle(250,130,30, DARKGREY);
  tft.setCursor(242,120);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("2");

  tft.fillCircle(250, 200, 30, GREEN);
  tft.setCursor(242,190);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("3");
}

void resetTank()
{
  tft.fillRect(0,0,200,240,ORANGE);
  tft.drawFastVLine(31,71,140,BLACK);
  tft.drawFastVLine(32,71,140,BLACK);
  tft.drawFastVLine(33,71,140,BLACK);
  tft.drawFastVLine(34,71,140,BLACK);
  tft.drawFastVLine(35,71,140,BLACK);
  tft.drawFastVLine(169,71,140,BLACK);
  tft.drawFastVLine(168,71,140,BLACK);
  tft.drawFastVLine(167,71,140,BLACK);
  tft.drawFastVLine(166,71,140,BLACK);
  tft.drawFastVLine(165,71,140,BLACK);
  tft.drawFastHLine(36,206,130,BLACK);
  tft.drawFastHLine(36,207,130,BLACK);
  tft.drawFastHLine(36,208,130,BLACK);
  tft.drawFastHLine(36,209,130,BLACK);
  tft.drawFastHLine(36,210,130,BLACK);
  tft.drawFastHLine(80,50,45,BLACK);
  tft.drawFastHLine(80,49,45,BLACK);
  tft.drawFastHLine(80,48,45,BLACK);
  tft.drawFastHLine(80,47,45,BLACK);
  tft.drawFastHLine(80,46,45,BLACK);
  tft.drawLine(31,72,79,46,BLACK);
  tft.drawLine(32,72,79,47,BLACK);
  tft.drawLine(33,72,79,48,BLACK);
  tft.drawLine(34,72,79,49,BLACK);
  tft.drawLine(35,72,79,50,BLACK);
  tft.drawLine(169,72,125,46,BLACK);
  tft.drawLine(168,72,125,47,BLACK);
  tft.drawLine(167,72,125,48,BLACK);
  tft.drawLine(166,72,125,49,BLACK);
  tft.drawLine(165,72,125,50,BLACK);
}

void tank1Monitor()
{
      tank1ON();
      resetTank();
      while(1)
      {
      if(Serial1.available()>0)
      {  
        per = Serial1.read();
      }
      //curr_percent1 = per;
      
      tft.setCursor(50,12);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("TANK 1");

      //digitalWrite(trigPin, HIGH);
      //delayMicroseconds(150);
      //digitalWrite(trigPin, LOW);
      
      //Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
      /*duration = pulseIn(echoPin, HIGH);
      
      //Calculate the distance:
      distance = duration*0.034/2;
      
      // Print the distance on the Serial Monitor (Ctrl+Shift+M):
      Serial.print("Distance = ");
      Serial.print(distance);
      Serial.println(" cm");*/
      
      pre_percent1 = curr_percent1;
      pre_heigh = (132*pre_percent1)/100;
      curr_percent1 = per;
      int heigh;
      heigh = (132*curr_percent1)/100;
      diff = pre_heigh - heigh;
      if(curr_percent1 < pre_percent1)
      {
        tft.fillRect(36,240-pre_heigh-35,129,diff,ORANGE);
        tft.fillTriangle(36,240-pre_heigh-35,61,240-pre_heigh-35,48.5,240-pre_heigh-4-35,ORANGE);
        tft.fillTriangle(73.5,240-pre_heigh-35,98.5,240-pre_heigh-35,86,240-pre_heigh-4-35,ORANGE);
        tft.fillTriangle(111,240-pre_heigh-35,136,240-pre_heigh-35,123.5,240-pre_heigh-4-35,ORANGE);
        tft.fillTriangle(138.5,240-pre_heigh-35,163.5,240-pre_heigh-35,151,240-pre_heigh-4-35,ORANGE); 
      }

      if(curr_percent1>=98)
      {digitalWrite(30,HIGH);}
      else
      {digitalWrite(30,LOW);}
      if(curr_percent1<=5)
      {digitalWrite(31,HIGH);}
      else
      {digitalWrite(31,LOW);}

      
      tft.fillRect(36,240-heigh-35,129,heigh+1,DARKCYAN);
      tft.fillTriangle(36,240-heigh-35,61,240-heigh-35,48.5,240-heigh-4-35,DARKCYAN);
      tft.fillTriangle(73.5,240-heigh-35,98.5,240-heigh-35,86,240-heigh-4-35,DARKCYAN);
      tft.fillTriangle(111,240-heigh-35,136,240-heigh-35,123.5,240-heigh-4-35,DARKCYAN);
      tft.fillTriangle(138.5,240-heigh-35,163.5,240-heigh-35,151,240-heigh-4-35,DARKCYAN);
      if(curr_percent1 != pre_percent1)
      {tft.fillRect(77,213,90,25,ORANGE);}
      tft.setCursor(77,213);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print(curr_percent1);
      tft.print("%");
      TSPoint p = ts.getPoint(); 
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);

      if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
      {
        p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width())
        p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
     if ((p.y > 20) && (p.y < 90) && (p.x > 120) && (p.x < 175))
     {
        tank2Monitor();
     }
     if ((p.y > 20) && (p.y < 90) && (p.x > 180) && (p.x < 240))
     {
        tank3Monitor();
     }
      }
      }
}

void tank2Monitor()
{
      tank2ON();
      resetTank();
      while(1)
      {
      tft.setCursor(50,12);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("TANK 2");
      percent = 10;
      int heigh;
      heigh = (132*percent)/100;
      tft.fillRect(36,240-heigh-35,129,heigh+1,DARKCYAN);
      tft.fillTriangle(36,240-heigh-35,61,240-heigh-35,48.5,240-heigh-4-35,DARKCYAN);
      tft.fillTriangle(73.5,240-heigh-35,98.5,240-heigh-35,86,240-heigh-4-35,DARKCYAN);
      tft.fillTriangle(111,240-heigh-35,136,240-heigh-35,123.5,240-heigh-4-35,DARKCYAN);
      tft.fillTriangle(138.5,240-heigh-35,163.5,240-heigh-35,151,240-heigh-4-35,DARKCYAN);
      tft.setCursor(77,213);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print(percent);
      tft.print("%");
      TSPoint p = ts.getPoint(); 
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);

      if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
      {
        p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
        // *** SPFD5408 change -- End
        p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
     if ((p.y > 20) && (p.y < 90) && (p.x > 50) && (p.x < 115))
     {
        tank1Monitor();
     }
     if ((p.y > 20) && (p.y < 90) && (p.x > 180) && (p.x < 240))
     {
        tank3Monitor();
     }
      }
      }
}

void tank3Monitor()
{
      tank3ON();
      resetTank();
      while(1)
      {
      tft.setCursor(50,12);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("TANK 3");
      percent = 90;
      int heigh;
      heigh = (132*percent)/100;
      tft.fillRect(36,240-heigh-35,129,heigh+1,DARKCYAN);
      tft.fillTriangle(36,240-heigh-35,61,240-heigh-35,48.5,240-heigh-4-35,DARKCYAN);
      tft.fillTriangle(73.5,240-heigh-35,98.5,240-heigh-35,86,240-heigh-4-35,DARKCYAN);
      tft.fillTriangle(111,240-heigh-35,136,240-heigh-35,123.5,240-heigh-4-35,DARKCYAN);
      tft.fillTriangle(138.5,240-heigh-35,163.5,240-heigh-35,151,240-heigh-4-35,DARKCYAN);
      tft.setCursor(77,213);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print(percent);
      tft.print("%");
      TSPoint p = ts.getPoint(); 
      pinMode(XM, OUTPUT);
      pinMode(YP, OUTPUT);

      if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
      {
        p.y = map(p.y, TS_MINX, TS_MAXX, 0, tft.width());
        // *** SPFD5408 change -- End
        p.x = map(p.x, TS_MINY, TS_MAXY, 0, tft.height());
     if ((p.y > 20) && (p.y < 90) && (p.x > 50) && (p.x < 115))
     {
        tank1Monitor();
     }
     if ((p.y > 20) && (p.y < 90) && (p.x > 120) && (p.x < 175))
     {
        tank2Monitor();
     }
      }
      }
}
