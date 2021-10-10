// Adafruit and SPI libraries
//This example uses LCD to graph value of an analog input at A0. A 1000 ohm resistor is placed from A0 to ground and common of input voatage source is connected to Arduino common. 

#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <Adafruit_ST7789.h>
 
// Pins
#define TFT_DC 8
#define TFT_CS 10
#define TFT_RST 9
// Create ST7735 instance

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Variable for the X coordinate. 
int xPos = 0;


void  setup () {
   // TFT init 
  Serial.begin(9600);
  tft.initR(INITR_144GREENTAB);
  clean();
}


void  loop () {
  // read the value of the input voltage 
  int voltage = analogRead (A0);
  int voltageScaled = map (voltage, 0, 1023, 0, tft.height ());
  // draw a 3x3 pixel rectangle
  tft.fillRect (xPos, tft.height() - voltageScaled-1, 3, 3, ST7735_GREEN);

  tft.fillCircle (64, 64, 10, ST7735_GREEN);
  // check if we have already reached the edge of the TFT
  if(xPos>= tft.width () - 1){
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
  tft.fillScreen (ST7735_BLUE);
  tft.setTextSize (2);
  tft.setTextColor (ST7735_WHITE);
  tft. setCursor ( 5,5 );
  tft. print ( "Vref" );
  
  tft.setTextColor (ST7735_WHITE);
  tft. setCursor (20,100);
  tft. print ( "V(t)" );
}
   
