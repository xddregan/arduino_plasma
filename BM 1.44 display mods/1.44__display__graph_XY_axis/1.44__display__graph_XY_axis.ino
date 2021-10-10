
//This example uses LCD to graph value of an analog input at A0 with XY axis lbeled. A 1000 ohm resistor is placed from A0 to ground and common of input voatage source is connected to Arduino common. 

#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <Adafruit_ST7789.h>

// Pins
#define TFT_DC 8
#define TFT_CS 10
#define TFT_RST 9
#define BKLIGHT 6 // Backlight control pin

// Create ST7735 instance

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define BRIGHTNESS 200 // Backlight intensity (0-255)
#define RANGE 100 // Vertical size of the graph in pixels.
#define WIDTH 128 // Horizontal size of Display in pixels.
#define HEIGHT 128 // Vertical size of Display in pixels.
#define PERSIST 500 // persistence of the graph (milliseconds).


int value ;
int last_value;
int x_pos= 0; // cursor position
int offset= 40; // vertical graph displacement in pixels.

void setup(){
// TFT init 
  Serial.begin(9600);
  tft.initR(INITR_144GREENTAB);
  //clearScreen();
  tft.setCursor (50, 5);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_YELLOW);
  tft.print("Bruce");

tft.fillScreen (ST7735_YELLOW);
tft.setTextSize (1);
tft.setTextColor (ST7735_WHITE);

analogWrite(BKLIGHT,BRIGHTNESS); //Set brightness.
tft. setCursor ( 20,20 );
tft. print ( "Graph" );
drawFrame();
clearGraph();
}

void drawFrame(){
tft.fillScreen(ST7735_WHITE);
tft.drawRect(0, 0, WIDTH, 21, ST7735_YELLOW);

tft.setTextSize(1);
tft.setCursor(10, 10);
tft.print("Volts: 1.0v/Div");
tft.setCursor(2, 12);
tft.print("Time: 10mS/Div");
}
void clearGraph(){
tft.fillScreen( ST7735_BLUE );
tft.drawRect(10, 30, 108, 60, ST7735_RED);
tft.drawLine(12, 80, 110, 80, ST7735_WHITE);
tft.drawLine(12, 80, 12, 35, ST7735_WHITE);

}

void plotLine(int last,int actual){
if(x_pos==0)last= actual; // first value is a dot.
tft.drawLine(x_pos-1, last, x_pos, actual, ST7735_RED);
x_pos++;
if(x_pos > WIDTH){
x_pos=0;
delay(PERSIST);
clearGraph();
}
}

void loop(){
last_value= value;
value= analogRead(A0);
value= map(value,0,1023,115,23);
value -= offset;
if(value <= 23) value= 23; //truncate off screen values.
plotLine(last_value ,value );
delay(10);
}
