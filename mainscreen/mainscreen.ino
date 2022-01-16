
// Adafruit and SPI libraries 
#include <stddef.h>
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


//Bruce these contol min/maxs and the step for powere and time 
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


//Bruce here are the delays for the different phases
// I think 1 is minimum persion if you need shorter I might need change

#define PHASE1_DELAY 5 
#define PHASE2_DELAY 5 
#define COOLDOWN_PERIOD 5 


#define START_BUTTON_X 45   
#define START_BUTTON_Y 40 
#define START_BUTTON_SIZEX  40
#define START_BUTTON_SIZEY  40 

#define STOP_BUTTON_X 85   
#define STOP_BUTTON_Y 95 
#define STOP_BUTTON_SIZEX  40 
#define STOP_BUTTON_SIZEY  25 

#define STATUS_TEXT_X 6   
#define STATUS_TEXT_Y 95 
#define STATUS_TEXT_SIZEX 22 
#define STATUS_TEXT_SIZEY  25 

#define STATUS_BUTTON_X 34   
#define STATUS_BUTTON_Y 95 
#define STATUS_BUTTON_SIZEX 52 
#define STATUS_BUTTON_SIZEY  25 

#define GRAPH_REGION_X 5 
#define GRAPH_REGION_Y 36 
#define GRAPH_REGION_LIMIT_X 120
#define GRAPH_REGION_LIMIT_Y 90 
#define GRAPH_SIZE_X (GRAPH_REGION_LIMIT_X - GRAPH_REGION_X)
#define GRAPH_SIZE_Y (GRAPH_REGION_LIMIT_Y - GRAPH_REGION_Y)


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 450);
Adafruit_ST7735 tft = Adafruit_ST7735 (TFT_CS, TFT_DC, TFT_RST);
Adafruit_GFX * gfx  = (Adafruit_GFX *) &tft;

enum State 
{

	READY,
	RUNNING,
	STARTING_PHASE1,
	STARTING_PHASE2,
	COOLDOWN,
	WAITING_RESET,
	ALARM,
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
	"alarm"
};

int runTimeInSeconds =  60;  
int powerLevel = 25;

// Timers
millisDelay phase1Delay;
millisDelay phase2Delay;
millisDelay runningTimeDelay;
millisDelay cooldownDelay;

State state = READY;



Adafruit_GFX_Button startButton = Adafruit_GFX_Button();
Adafruit_GFX_Button startButtonClear = Adafruit_GFX_Button();
Adafruit_GFX_Button stopButton = Adafruit_GFX_Button();
Adafruit_GFX_Button stopButtonClear = Adafruit_GFX_Button();
Adafruit_GFX_Button statusButton = Adafruit_GFX_Button();
Adafruit_GFX_Button statusButtonTxt = Adafruit_GFX_Button();
Adafruit_GFX_Button statusButtonClear = Adafruit_GFX_Button();
Adafruit_GFX_Button resetButton = Adafruit_GFX_Button();
Adafruit_GFX_Button resetButtonClear = Adafruit_GFX_Button();


Adafruit_GFX_Button powerDisplay= Adafruit_GFX_Button();
Adafruit_GFX_Button timeDisplay= Adafruit_GFX_Button();
Adafruit_GFX_Button * powerDisplayTxt= NULL; 
Adafruit_GFX_Button * timeDisplayTxt= NULL; 
//Adafruit_GFX_Button * timeDisplayTxt= Adafruit_GFX_Button();


void  setup () {
   // Start TFT
   Serial.begin(9600); 
  
  	randomSeed(analogRead(0)); 
	tft.initR(INITR_GREENTAB);
	tft.setTextColor(ST77XX_WHITE);
	tft.setTextSize(1);
	tft.fillScreen(ST77XX_BLACK);
	makeStartButton();
	makeStopButton();
	makeStatusButton();
	makeResetButton();
	startReady();	
	
  Serial.print("Done with setup");
}





void loop() 
{



  	TSPoint touchPoint = ts.getPoint();
	uint16_t pressure = ts.pressure();	
	bool validTouch = false;
	
  	if (pressure > MINPRESSURE && pressure < MAXPRESSURE) 
	{
		touchPoint.x = map(touchPoint.x, TS_MINX, TS_MAXX, tft.width(), 0);
		touchPoint.y = map(touchPoint.y, TS_MINY, TS_MAXY, 128, 0);
		validTouch = true;
		Serial.print("("); Serial.print(touchPoint.x);
		Serial.print(", "); Serial.print(touchPoint.y);
		Serial.print(", "); Serial.print(touchPoint.z);
		Serial.print(", "); Serial.print(tft.width());
		Serial.print(", "); Serial.print(tft.height());
		Serial.println(")");
	}
	

	//Serial.print("Pressure =");Serial.print(ts.pressure());Serial.print("\n");
	switch (state)
	{
		case READY:
			drawStartButton(true);
			drawPowerAdjustButtons(true);
			drawTimeAdjustButtons(true);
			drawStatusButton("Ready",ST77XX_GREEN);
  			if (pressure > MINPRESSURE && pressure < MAXPRESSURE) 
			{
				readPowerTouchInput(touchPoint, touchPoint.x, touchPoint.y);
				readTimeTouchInput(touchPoint, touchPoint.x, touchPoint.y);
				if( readStartButton(touchPoint, touchPoint.x, touchPoint.y))
				{
					drawStartButton(false);
					drawPowerAdjustButtons(false);
					drawTimeAdjustButtons(false);
					startPhase1();
					
				}

			}
			
		break;
		case STARTING_PHASE1:
		drawStatusButton("PHASE 1",ST77XX_ORANGE);
		if( phase1Delay.justFinished())
		{
				
			startPhase2();			
		}
			
		break;	
		case STARTING_PHASE2:
		drawStatusButton("PHASE 2",ST77XX_ORANGE);
		if( phase2Delay.justFinished())
		{
			startRunning();			
			drawStopButton(true);
			drawStatusButton("RUNNING",ST77XX_GREEN);
		}

		break;
		case RUNNING:


		addGraphValue(readValueForGraph()); // Just putting random data
		drawGraph(true);
		if (runningTimeDelay.justFinished() ||(validTouch && stopButton.contains(touchPoint.x,touchPoint.y)) )
		{
			startCooldown();	
			drawStopButton(false);
			drawStatusButton("COOLDOWN",ST77XX_MAGENTA);
		}
		break;
		case COOLDOWN:
			if( cooldownDelay.justFinished())
			{
				startReset();
				drawStatusButton("RESET",ST77XX_MAGENTA);
				drawResetButton(true);	
			}
			//drawResartButton(false);
		break;
		case WAITING_RESET:
		if( validTouch && resetButton.contains(touchPoint.x,touchPoint.y))
		{
			drawResetButton(false);	
			drawGraph(false);
			clearGraphEntries();
			startReady();
		}
		break;
		case ALARM:
		if( validTouch && resetButton.contains(touchPoint.x,touchPoint.y))
		{
			drawResetButton(false);	
			drawGraph(false);
			clearGraphEntries();
			startReady();
		}
		break;
		break;

	}
	drawTime(runTimeInSeconds);
	drawPower(powerLevel);
	delay(250);

	// do common drawing tasks here

}

/********************Bruce most if not all changes you need to make should be in this section *******************/
/*************************************************Start Bruce Section *******************************************/
void startReady()
{
	state = READY;
	
}

void startPhase1()
{
	state = STARTING_PHASE1;
	phase1Delay.start(PHASE1_DELAY * 1000);
	Serial.println("Starting phase1");

	//BRUCE  Add code to run immediately after start here 

}
void startPhase2()
{
	state = STARTING_PHASE2;
	phase2Delay.start(PHASE1_DELAY * 1000);
	Serial.println("Starting phase2");

	//BRUCE  Add code to run after phase 1 timer here 
}

void startRunning()
{
	state = RUNNING;
	runningTimeDelay.start(runTimeInSeconds * 1000);
	Serial.println("Starting running ");

	//BRUCE  Add code to run start of running 
}


void startCooldown()
{
	//print 2
	state = COOLDOWN;
	cooldownDelay.start(COOLDOWN_PERIOD * 1000);

	//BRUCE  Add code to init cooldown
   /// turn off  	
}

void startReset()
{
	state = WAITING_RESET;
}

int readValueForGraph()
{
	//This function called ever 1/4 second when running 
	//BRUCE  read any values during runtime here
    // Convert the value to range 1-100	
	// then return here insteat of 
	//int value = /?????
	// return value;
	return random(0,100);
}

// BRUCE this function will need to be connected to the ALARM interrupt
void doAlarm()
{

	drawStatusButton("ALARM",ST77XX_RED);
	state = ALARM;

	
}
/****************************************End Bruce Section ***********************/
  




#define GRAPH_ENTRIES (GRAPH_SIZE_X - 2)

int startIndex = 0;
int endIndex = 0;
int graphCount = 0;
int graphData[GRAPH_ENTRIES];

void addGraphValue(int val)
{
	if (val < 0 ) val = 0; 		
	if (val > 100 ) val = 100; 		
	int scaledValue = (val * GRAPH_SIZE_Y)/100;  	
	endIndex = (endIndex + 1)% GRAPH_ENTRIES;
	if (graphCount <  GRAPH_ENTRIES)   	
		graphCount++;
	if (graphCount  == GRAPH_ENTRIES) 
		startIndex = (startIndex + 1)% GRAPH_ENTRIES;
	graphData[endIndex] = scaledValue;
}

void clearGraphEntries()
{
	startIndex = 0;
	endIndex = 0;
	graphCount = 0;

}


void drawGraph(bool on)
{
	//gfx->startWrite();
	if (on)
	{
		gfx->drawFastHLine(GRAPH_REGION_X,GRAPH_REGION_Y,GRAPH_SIZE_X,ST77XX_CYAN);
		gfx->drawFastVLine(GRAPH_REGION_X,GRAPH_REGION_Y,GRAPH_SIZE_Y,ST77XX_CYAN);
		gfx->drawFastHLine(GRAPH_REGION_X,GRAPH_REGION_LIMIT_Y,GRAPH_SIZE_X,ST77XX_CYAN);
		gfx->drawFastVLine(GRAPH_REGION_LIMIT_X,GRAPH_REGION_Y,GRAPH_SIZE_Y,ST77XX_CYAN);
		gfx->fillRect(GRAPH_REGION_X+1,GRAPH_REGION_Y+1,GRAPH_SIZE_X-1,GRAPH_SIZE_Y-2,ST77XX_BLACK);
		for (int i = 0; i <	graphCount ; i++)
		{
			int val = graphData[(startIndex +i)  %  GRAPH_ENTRIES];
			gfx->drawFastVLine(GRAPH_REGION_X+i+1,GRAPH_SIZE_Y + GRAPH_REGION_Y-val,val,ST77XX_BLUE);
		}	
		
	}
	else
	{
		gfx->fillRect(GRAPH_REGION_X,GRAPH_REGION_Y,GRAPH_SIZE_X+1,GRAPH_SIZE_Y+1,ST77XX_BLACK);
	}
	//gfx->endWrite();
}


void makeStartButton()
{
	startButton.initButtonUL((Adafruit_GFX*)&tft
							 ,START_BUTTON_X,START_BUTTON_Y
							 ,START_BUTTON_SIZEX,START_BUTTON_SIZEY
							 ,ST77XX_RED,ST77XX_GREEN,ST77XX_BLACK
							 ,"Run",2); 

	startButtonClear.initButtonUL((Adafruit_GFX*)&tft
							  ,START_BUTTON_X,START_BUTTON_Y
							  ,START_BUTTON_SIZEX,START_BUTTON_SIZEY
							  ,ST77XX_BLACK,ST77XX_BLACK,ST77XX_BLACK
							  ,"",2); 


}

void drawStartButton(bool on)
{
	if (on)
		startButton.drawButton();
	else
		startButtonClear.drawButton();

}

void makeStopButton( )
{
	stopButton.initButtonUL((Adafruit_GFX*)&tft
							 ,STOP_BUTTON_X,STOP_BUTTON_Y
							 ,STOP_BUTTON_SIZEX,STOP_BUTTON_SIZEY
							 ,ST77XX_RED,ST77XX_RED,ST77XX_WHITE
							 ,"STOP",1); 

	stopButtonClear.initButtonUL((Adafruit_GFX*)&tft
							  ,STOP_BUTTON_X,STOP_BUTTON_Y
							  ,STOP_BUTTON_SIZEX,STOP_BUTTON_SIZEY
							  ,ST77XX_BLACK,ST77XX_BLACK,ST77XX_BLACK
							  ,"",1); 

}

void makeResetButton( )
{
	resetButton.initButtonUL((Adafruit_GFX*)&tft
							 ,STOP_BUTTON_X,STOP_BUTTON_Y
							 ,STOP_BUTTON_SIZEX,STOP_BUTTON_SIZEY
							 ,ST77XX_ORANGE,ST77XX_ORANGE,ST77XX_WHITE
							 ,"RESET",1); 

	resetButtonClear.initButtonUL((Adafruit_GFX*)&tft
							  ,STOP_BUTTON_X,STOP_BUTTON_Y
							  ,STOP_BUTTON_SIZEX,STOP_BUTTON_SIZEY
							  ,ST77XX_BLACK,ST77XX_BLACK,ST77XX_MAGENTA
							  ,"",1); 

}

void drawStopButton(bool on)
{
	if (on)
		stopButton.drawButton();
	else
		stopButtonClear.drawButton();
}

void drawResetButton(bool on)
{
	if (on)
		resetButton.drawButton();
	else
		resetButtonClear.drawButton();
}


void makeStatusButton()
{

	statusButtonTxt.initButtonUL((Adafruit_GFX*)&tft
							 ,STATUS_TEXT_X,STATUS_TEXT_Y
							 ,STATUS_TEXT_SIZEX,STATUS_TEXT_SIZEY
							 ,ST77XX_BLACK,ST77XX_BLACK,ST77XX_WHITE
							 ,"STATUS",1); 


	statusButton.initButtonUL((Adafruit_GFX*)&tft
							 ,STATUS_BUTTON_X,STATUS_BUTTON_Y
							 ,STATUS_BUTTON_SIZEX,STATUS_BUTTON_SIZEY
							 ,ST77XX_RED,ST77XX_RED,ST77XX_WHITE
							 ,"READY",1); 

	statusButtonClear.initButtonUL((Adafruit_GFX*)&tft
							  ,STATUS_BUTTON_X,STATUS_BUTTON_Y
							  ,STATUS_BUTTON_SIZEX,STATUS_BUTTON_SIZEY
							  ,ST77XX_BLACK,ST77XX_BLACK,ST77XX_BLACK
							  ,"",1); 

}

void drawStatusButton(char * status,uint16_t color)
{
	statusButtonTxt.drawButton();
	statusButtonClear.drawButton();
	statusButton.initButtonUL((Adafruit_GFX*)&tft
							 ,STATUS_BUTTON_X,STATUS_BUTTON_Y
							 ,STATUS_BUTTON_SIZEX,STATUS_BUTTON_SIZEY
							 ,color,ST7735_GRAY,ST77XX_WHITE
							 ,status,1); 
	statusButton.drawButton();

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

void drawTime(int time)
{
	if (!timeDisplayTxt)
	{
		String timeStrTxt = String("TIME");  
		timeDisplayTxt = new Adafruit_GFX_Button();
		timeDisplayTxt->initButtonUL((Adafruit_GFX*)&tft,97,24,22,18
					  ,ST77XX_BLACK,ST77XX_BLACK,ST77XX_WHITE
					  ,(char*)timeStrTxt.c_str(),1); 
		timeDisplayTxt->drawButton(); 	
	}


	String timeStr = String(time);  
	timeDisplay.initButtonUL((Adafruit_GFX*)&tft,88,4,22,18
				  ,ST77XX_WHITE,ST77XX_BLACK,ST77XX_WHITE
				  ,(char*)timeStr.c_str(),1); 
	timeDisplay.drawButton();


}


void drawPower(int power)
{
	if (!powerDisplayTxt)
	{
		String powerStrTxt = String("POWER");  
		powerDisplayTxt = new Adafruit_GFX_Button();
		powerDisplayTxt->initButtonUL((Adafruit_GFX*)&tft,13,24,22,18
					  ,ST77XX_BLACK,ST77XX_BLACK,ST77XX_WHITE
					  ,(char*)powerStrTxt.c_str(),1); 
		powerDisplayTxt->drawButton(); 	
	}
	String powerStr = String(power);  
	powerDisplay.initButtonUL((Adafruit_GFX*)&tft,4,4,22,18
				  ,ST77XX_WHITE,ST77XX_BLACK,ST77XX_WHITE
				  ,(char*)powerStr.c_str(),1); 
	powerDisplay.drawButton();

}




// We validate the touchX and touchY so this is only called if touch is valid
// This saves duplicated the checking code we do it once at start of input. 



void readPowerTouchInput(TSPoint touchPoint, int touchX, int touchY)
{
	// I'm checking the rectangle that encloses the button.  
    // This is easier than checking the triangle and should be accurate enough
	// if it's a problem term I can do the test to check you are inside 

	if (touchX >= 30 && touchX < 42 && touchY >4 && touchY < 10 )
	{
    Serial.print("Got power touch");
		if ((powerLevel + POWER_STEP)  <= MAX_POWER)
		{
			powerLevel = powerLevel + POWER_STEP;

		}		
		else
		{
			powerLevel = MAX_POWER; 
		}
		
		
		
		
	}
	else if (touchX >= 30 && touchX < 42 && touchY >14 && touchY < 20 )
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

void readTimeTouchInput(TSPoint touchPoint, int touchX, int touchY)
{
	// I'm checking the rectangle that encloses the button.  
    // This is easier than checking the triangle and should be accurate enough
	// if it's a problem term I can do the test to check you are inside 

	if (touchX >= 114 && touchX < 136 && touchY >4 && touchY < 10 )
	{
    Serial.print("Got time touch");
		if ((runTimeInSeconds + TIME_STEP)  <= MAX_TIME)
		{
			runTimeInSeconds = runTimeInSeconds + TIME_STEP;

		}		
		else
		{
			runTimeInSeconds = MAX_TIME; 
		}
		
	}
	else if (touchX >= 114 && touchX < 136 && touchY >14 && touchY < 20 )
	{
		if ((runTimeInSeconds - TIME_STEP)  >= MIN_TIME)
		{
			runTimeInSeconds = runTimeInSeconds - TIME_STEP;

		}		
		else
		{
			runTimeInSeconds = MIN_TIME; 
		}
		

	}
}


bool readStartButton(TSPoint touchPoint, int touchX, int touchY)
{
	if (startButton.contains(touchX,touchY))
	{
		return true;
	}
	else
	{
		return false;
	}
	

}
