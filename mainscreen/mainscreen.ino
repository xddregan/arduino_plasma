
// Adafruit and SPI libraries 
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <TouchScreen.h>
#include <millisDelay.h>
// Added a comment



#define WIDTH 128
#define HEIGHT 128
 
// Pins
#define TFT_DC 8
#define TFT_CS 10
#define TFT_MISO 12
#define TFT_CLK 13
#define TFT_MOSI 11
#define TFT_RST 9 



#define TS_MINX 250
#define TS_MINY 290
#define TS_MAXX 850
#define TS_MAXY 850

#define BOXSIZE 40
#define PENRADIUS 3

#define MINPRESSURE 10
#define MAXPRESSURE 1000


#define MIN_POWER 5
#define MAX_POWER 50
#define POWER_STEP 5


#define MIN_TIME 30 
#define MAX_TIME 300 
#define TIME_STEP 30 


#define ST7735_GRAY 0x8410 

#define YP A2
#define XM A3
#define YM 3
#define XP 5


#define TS_MINX 250
#define TS_MINY 290
#define TS_MAXX 850
#define TS_MAXY 850

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 450);
Adafruit_ST7735 tft = Adafruit_ST7735 (TFT_CS, TFT_DC, TFT_RST);
enum State 
{

	READY,
	RUNNING,
	STARTING_PHASE1,
	STARTING_PHASE2,
	COOLDOWN,
	WAITING_RESET,
	NUM_STATES  // <<This has to be last and is used as count of states
	
};

// These are useful for debugging 
char * stateName[NUM_STATES] = 
{
	"ready",
	"running",
	"starting_phase1",
	"starting_phase2",
	"cooldown",
	"waiting_reset"
};

int runTimeInSeconds =  60;  
int powerLevel = 25;

// Timers
millisDelay phase1Delay;
millisDelay phase2Delay;
millisDelay runningTimeDelay;

State state = READY;


void  setup () {
   // Start TFT
   Serial.begin(9600); 
  
  tft.initR(INITR_GREENTAB);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.fillScreen(ST77XX_BLACK);
  
}





void loop() 
{


  TSPoint touchPoint = ts.getPoint();


	switch (state)
	{
		case READY:
			readStartButton(touchPoint, touchPoint.x, touchPoint.y);
			readPowerTouchInput(touchPoint, touchPoint.x, touchPoint.y);
			drawStartButton(true);
			drawPowerAdjustButtons(true);
			drawTimeAdjustButtons(true);
		break;
		case STARTING_PHASE1:
		if( phase1Delay.justFinished())
		{
			startPhase2();			
		}
			
		break;	
		case STARTING_PHASE2:
		if( phase2Delay.justFinished())
		{
			startRunning();			
		}

		break;
		case RUNNING:
		drawGraph();
		if (runningTimeDelay.justFinished())
		{
			startCooldown();	
		}
		break;
		case COOLDOWN:

		break;
		case WAITING_RESET:
		break;

	}

	// do common drawing tasks here

}


void startPhase1()
{
	state = STARTING_PHASE1;
	phase1Delay.start(1000);


}

void startCooldown()
{
	//print 2
	state = COOLDOWN;


}

void drawGraph()
{

}

void startPhase2()
{
	state = STARTING_PHASE2;
	phase2Delay.start(1000);
}

void startRunning()
{
	state = RUNNING;
	runningTimeDelay.start(runTimeInSeconds * 1000);
}

void drawStartButton(bool on)
{
	if ( on) 
	{
  		tft.fillCircle (64, 16, 15, ST7735_BLUE); //Run button
	}
	else
	{

  		tft.fillCircle (64, 16, 15, ST7735_GRAY); //Run button
	}

}


void drawPowerAdjustButtons(bool on)
{
	if (on == true)
	{
	  tft.fillTriangle(36, 4, 30, 10, 42, 10, ST77XX_YELLOW); //powerup triangle
	  tft.fillTriangle(36, 20, 30, 14, 42, 14, ST77XX_YELLOW); //powerdown triangle
	}
	else
	{
	  tft.fillTriangle(36, 4, 30, 10, 42, 10, ST7735_GRAY); //powerup triangle
	  tft.fillTriangle(36, 20, 30, 14, 42, 14, ST7735_GRAY); //powerdown triangle

	}

}

void drawTimeAdjustButtons(bool on)
{
	if (on == true)
	{
  		tft.fillTriangle(120, 4, 114, 10, 126, 10, ST77XX_YELLOW);  //timeup triangle
  		tft.fillTriangle(120, 20, 114, 14, 126, 14, ST77XX_YELLOW);  //time down triangle
	}
	else
	{
  		tft.fillTriangle(120, 4, 114, 10, 126, 10, ST7735_GRAY);  //timeup triangle
  		tft.fillTriangle(120, 20, 114, 14, 126, 14, ST7735_GRAY);  //time down triangle
	}
}

// We validate the touchX and touchY so this is only called if touch is valid
// This saves duplicated the checking code we do it once at start of input. 



void readPowerTouchInput(TSPoint touchPoint, int touchX, int touchY)
{
	// I'm checking the rectangle that encloses the button.  
    // This is easier than checking the triangle and should be accurate enough
	// if it's a problem term I can do the test to check you are inside 

	if (touchX >= 114 && touchX < 120 && touchY >4 && touchY < 10 )
	{
		if ((powerLevel + POWER_STEP)  <= MAX_POWER)
		{
			powerLevel = powerLevel + POWER_STEP;

		}		
		else
		{
			powerLevel = MAX_POWER; 
		}
		
		
		
		
	}
	else if (touchX >= 114 && touchX < 120 && touchY >14 && touchY < 20 )
	{
		if ((powerLevel - POWER_STEP)  >= MIN_POWER)
		{
			powerLevel = powerLevel - POWER_STEP;

		}		
		else
		{
			powerLevel = MIN_POWER; 
		}
		

	}

}
void readStartButton(TSPoint touchPoint, int touchX, int touchY)
{
	if (touchX >= (64-15) && touchX < (64+15) && touchY > (16-15) && touchY < (16+15) )
	{
		startPhase1();	
	}

}
