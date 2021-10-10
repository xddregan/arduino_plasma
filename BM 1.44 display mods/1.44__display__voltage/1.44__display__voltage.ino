// Adafruit and SPI libraries
//This example uses LCD to display value of an analog input at A0. A 1000 ohm resistor is placed from A0 to ground and common of input voatage source is connected to Arduino common. 

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

void  setup () {
   // TFT init 
  Serial.begin(9600);
  tft.initR(INITR_GREENTAB);
  tft.fillScreen (ST7735_BLUE); 
  tft.setTextColor (ST7735_WHITE);
  tft.setTextSize (2);
  // to write the text: 
  tft. setCursor ( 20,20 );
  tft. print ( "Voltage:" );
  tft.setTextSize (2);
  tft.setTextColor (ST7735_WHITE);
  tft. setCursor (1,110);
  tft. print ( "Source LLP" );
  // to set it to a text size 2
  tft.setTextSize (2);
  tft.setTextColor (ST7735_GREEN);
  tft. setCursor (100,64);
  tft. print ( "V" );
  
}

float voltage = 0;
float voltageAnt = 0;
void  loop () {
   // to read the value of voltage
   voltage =  analogRead (A0) * (3.3 / 1023.0); 
 
  if (voltage-voltageAnt> 0.01 || voltage-voltageAnt <-0.01)  
  {
      tft.setTextColor (ST7735_RED);
      tft. setCursor (30,64);
      tft. print (voltageAnt);
      
      tft.setTextColor (ST7735_GREEN);
      tft. setCursor (30,64);
      tft. print (voltage);
      voltageAnt = voltage;
      
  }
  delay (100);
}
