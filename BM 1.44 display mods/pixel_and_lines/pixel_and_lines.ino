
#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <Adafruit_ST7789.h>
#define TFT_CS        10
#define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         8
//#define TFT_SCLK  13
//#define TFT_MISO  11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST, TFT_SCLK, TFT_MISO);

void setup(void) {
  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab
  //tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(0); // set display orientation
tft.fillScreen(ST77XX_BLACK);
  //print_text(85,80,"time(S)",1,ST77XX_BLUE);
}

  
 void loop() {
 tft.fillScreen(ST77XX_BLACK);
  print_text(85,80,"time(S)",1,ST77XX_BLUE);
    

  //tft.fillRect(54, 4, 20, 17, ST77XX_BLUE); //Run button
tft.fillCircle (64, 16, 15, ST7735_BLUE); //Run button

//tft.fillCircle (64, 64, 10, ST7735_GREEN);
  
  tft.fillTriangle(36, 4, 31, 10, 41, 10, ST77XX_YELLOW); //powerup triangle
  
  tft.fillTriangle(36, 20, 31, 14, 41, 14, ST77XX_YELLOW); //powerdown triangle
  tft.fillTriangle(120, 4, 116, 10, 126, 10, ST77XX_YELLOW);  //timeup triangle
  tft.fillTriangle(120, 20, 116, 14, 126, 14, ST77XX_YELLOW);  //time down triangle
  tft.fillRect(2, 4, 20, 17, ST77XX_YELLOW); //power button
  
  tft.fillRect(89, 4, 20, 17, ST77XX_YELLOW); //time button
  tft.fillCircle (64, 112, 15, ST7735_RED);
  //tft.fillRect(54, 108, 20, 16, ST77XX_RED); //stop button
  //tft.fillCircle(64, 115, 10, ST77XX_RED); //to replace rect "STOP"button with round button
  tft.fillRect(2, 108, 20, 17, ST77XX_YELLOW); //alarm button
  tft.fillRect(106, 108, 20, 17, ST77XX_YELLOW); //HW test button
   
  
 
  print_text(85,80,"time(S)",1,ST77XX_BLUE);
  //tft.drawRect(5,5,120,120,ST77XX_RED);
 tft.drawFastHLine(20,90,100,ST77XX_YELLOW);
  tft.drawFastVLine(20,30,60,ST77XX_YELLOW);
  
  
  delay(1000);
}

void print_text(byte x_pos, byte y_pos, char *text, byte text_size, uint16_t color) {
  tft.setCursor(x_pos, y_pos);
  tft.setTextSize(text_size);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}
