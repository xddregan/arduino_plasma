
#include <Adafruit_GFX.h>    // in rev 01_9_22, added fill color to RED, YELLOW and BLUE boxes
#include <Adafruit_ST7735.h> //hw lib for 1.44 display
#include <Adafruit_ST7789.h> //
#include <TouchScreen.h>
#include <SPI.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define TFT_DC 8
#define TFT_CS 10
#define TFT_MISO 12
#define TFT_CLK 13
#define TFT_MOSI 11
#define TFT_RST 9

#define TFT_SPI_HARDWARE

#define CS_PIN 25 //added line 0n 1-7-22 for touch chip select

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 3   // can be a digital pin
#define XP 5   // can be a digital pin

#define TS_MINX 300 //changed to 300 from 270 for more accurate touch
#define TS_MINY 290 //changed to 290 from 150 for more accurate touch point
#define TS_MAXX 830 //changed to 825 from 815 for a more accurate touch
#define TS_MAXY 840

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 430);

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

#define BOXSIZE 42
#define PENRADIUS 3
int oldcolor, currentcolor;

#define MINPRESSURE 10
#define MAXPRESSURE 1000

void setup(void) {
  Serial.begin(9600);
  Serial.println( F("Paint!"));
  pinMode(CS_PIN, OUTPUT); //added line 1-7-22
  digitalWrite(CS_PIN, HIGH); //added 1-7-22
  pinMode(TFT_CS, OUTPUT); //added 1-7-22
  digitalWrite(TFT_CS, HIGH); //added 1-7-22
  pinMode(A5, OUTPUT);
  digitalWrite(A5, LOW);
  
  tft.initR(INITR_144GREENTAB);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.fillScreen(BLACK);
 }

void loop(){
 TSPoint p = ts.getPoint();
   // if sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(YM, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    
  Serial.print("X = "); Serial.print(p.x);
  Serial.print("\tY = "); Serial.print(p.y);
  Serial.print("\tPressure = "); Serial.println(p.z);
       
  if (p.y > (TS_MAXY-5)) {
  //Serial.println("erase");
      // press the bottom of the screen to erase 
      tft.fillRect(0, BOXSIZE, tft.width(), tft.height()-BOXSIZE, BLACK);
    }
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    
    Serial.print("("); Serial.print(p.x);
    Serial.print(", "); Serial.print(p.y);
    Serial.println(")");
    
    if (p.y < BOXSIZE) {
       oldcolor = currentcolor;

       if (p.x < BOXSIZE) { 
         currentcolor = RED; 
         tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
         //tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
       } else if (p.x < BOXSIZE*2) {
         currentcolor = YELLOW;
         tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
         //tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
       } else if (p.x < BOXSIZE*3) {
         currentcolor = BLUE;
         tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, WHITE);
         //tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, BLUE);
       } 
       
       if (oldcolor != currentcolor) {
          if (oldcolor == RED) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
          if (oldcolor == YELLOW) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
          if (oldcolor == BLUE) tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, BLUE);
         
       }
    }
    if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
      tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);

        }
   } 

}

void clean(){

tft.fillScreen(BLACK);
tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, BLUE);
  
 
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE); 
  currentcolor = RED;
 
  }
