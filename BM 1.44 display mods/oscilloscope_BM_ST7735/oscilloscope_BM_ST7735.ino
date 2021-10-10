// Adafruit and SPI libraries 

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
 
// Pins
#define TFT_DC 8
#define TFT_CS 10
#define TFT_MISO 12
#define TFT_SCLK 13
#define TFT_MOSI 11
#define TFT_RST 9 

#define ST7735_TFTWIDTH_128 128
#define ST7735_TFTHEIGHT_128 128

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
// Create ST7735 instance

//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
 
// Variable for the X coordinate. 
int xPos = 0;
 
void  setup () {
   // Start TFT 
  Serial.begin (9600);
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_GREEN);
  //tft.setRotation (0); // Horizontal 
  //clear()  // function to clean the screen

  tft.drawPixel(tft.width()/2, tft.height()/2, ST7735_GREEN);
  tft.drawPixel(64, 90, ST7735_WHITE);
}

void  loop () {
   // We read the value of the voltage 
  int voltage = analogRead (A1);
  //voltage = analogRead(A1)*(3.3/1023.0);
  int voltageScaled = map (voltage, 0, 1023, 0, tft.height ());
  // We draw a 3x3 pixel rectangle
  tft.fillRect (xPos, tft.height ()-voltageScaled-1, 3, 3, ST7735_GREEN);
  // We check if we have already reached the edge of the TFT 

 if (xPos >= tft.width()-1){ 
    xPos = 0;
    clean();
  }
  else {
    xPos ++;
  }
  delay (5); //Sampling time
}

void clean ()
{
  tft.fillScreen (ST7735_BLACK);
  tft.setTextSize (1);
  tft.setTextColor (ST7735_GREEN);
  tft.setCursor(20,5);
  tft. print ( "Reflected Power" );
  
  tft.setTextColor (ST7735_GREEN);
  tft. setCursor (20,120);
  tft. print ( "Source LLP" );
}
