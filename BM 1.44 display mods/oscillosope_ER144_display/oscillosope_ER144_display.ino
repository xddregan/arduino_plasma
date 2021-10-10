// Adafruit and SPI libraries 
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>

#define WIDTH 128
#define HEIGHT 128
 
// Pins
#define TFT_DC 8
#define TFT_CS 10
#define TFT_MISO 12
#define TFT_CLK 13
#define TFT_MOSI 11
#define TFT_RST 9 
// Create ILI9341 instance
Adafruit_ST7735 tft = Adafruit_ST7735 (TFT_CS, TFT_DC, TFT_MOSI,TFT_CLK, TFT_MISO);
 
// Variable for the X coordinate. 
int xPos = 0;
 
void  setup () {
   // Start TFT
   Serial.begin(9600); 
  //tft.begin();
  tft.initR(INITR_GREENTAB);
  tft.setRotation(1); // Horizontal 
  //clear()  // function to clean the screen
}

void  loop () {
   // We read the value of the voltage 
  int voltage = analogRead (A0);
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
  tft.fillScreen (ST7735_RED);
  tft.setTextSize (1);
  tft.setTextColor (ST7735_WHITE);
  tft.setCursor(5,5);
  tft. print ("Return Power");
  
  tft.setTextColor (ST7735_WHITE);
  tft. setCursor (5,100);
  tft. print ( "Source LLP" );
}
