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

float p = 3.1415926;
 
// Variable for the X coordinate. 
int xPos = 0;

int analogPin = A1;
int val = 0;
 
void  setup () {
   // Start TFT 
  Serial.begin (9600);
  tft.initR(INITR_144GREENTAB);
   Serial.println(F("Initialized"));

  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_GREEN);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  tft.fillScreen(ST7735_BLACK);
  
void  tftPrintTest();
    delay(4000);

    tft.drawPixel(tft.width()/2, tft.height()/2, ST7735_GREEN);
  tft.drawPixel(64, 80, ST7735_GREEN);
  delay(1000);
  
  
  
  tft.setTextSize(2);
  
  tft.setCursor(10,30);
  tft.print("Voltage: ");
  tft.setTextSize(1);
  tft.setTextColor(ST7735_GREEN);
  tft.setCursor(10,120);
  tft.print("Source LLP");
  
  tft.setTextSize(2);
  tft.setTextColor(ST7735_GREEN);
  tft.setCursor(100,90);
  tft.print("V");
  
}

//float voltage=0;
//float voltageAnt=0;
void loop(){
    val = analogRead(analogPin)*(3.3/1023.0);
Serial.println(val, 2); //adding 2 did not fix up to 2 decimal places
delay(1000);
    
    //voltage = analogRead(A1) * (3.3 / 1023.0);
  //if(voltage-voltageAnt>0.01||voltage-voltageAnt<-0.01)
  
      tft.setTextColor(ST7735_RED);
      tft.setCursor(30,90);
      tft.print(val , 2);
      //tft.print(voltageAnt);
      
      //tft.setTextColor(ST7735_GREEN);
      //tft.setCursor(30,90);
      //tft.print(voltage);
      //voltageAnt=voltage;
      delay(1000);
  }
  
  //delay(100);
 
  
