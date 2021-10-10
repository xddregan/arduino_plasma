// Change 1
// Change 2

#define TFT_CS 10
#define TFT_RST 9
// Create ST7735 instance

//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//DOMINICK Added these as I have them in mine so best to initialize the same
#define TFT_MOSI 11  // Data out
#define TFT_SCLK 13  // Clock out

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void  setup () {
   // TFT init 
  Serial.begin(9600);
  tft.initR(INITR_GREENTAB);
  tft.fillScreen (ST7735_BLUE); 
  tft.setTextColor (ST7735_WHITE);
  tft.setTextSize (2);
  // to write the text: 
  tft.setCursor ( 20,20 );
  tft.print ( "Voltage:" );
  tft.setTextSize (2);
  tft.setTextColor (ST7735_WHITE);
  tft.setCursor (1,110);
  tft.print ( "Source LLP" );
  // to set it to a text size 2
  tft.setTextSize (2);
  tft.setTextColor (ST7735_GREEN);
  tft.setCursor (100,64);
  tft.print ( "V" );
  
}

float voltage = 0;
float voltageAnt = 0;
void  loop () {
   // to read the value of voltage


   voltage =  analogRead (A0) * (3.3 / 1023.0); 

  if (voltage-voltageAnt> 0.01 || voltage-voltageAnt <-0.01)  
  {
      tft.setTextColor (ST7735_RED);
      tft.setCursor (30,64);
      tft.print (voltageAnt);
      
      tft.setTextColor (ST7735_GREEN);
      tft.setCursor (30,64);
      tft.print (voltage);
      voltageAnt = voltage;
      
  }
  else
  {
  //tft.print("Invalid voltage  "); 
  //tft.println(voltage);
  }
  delay (100);
}
