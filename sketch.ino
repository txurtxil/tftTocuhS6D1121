/*
* Arduino 2″ TFT Touch Display and dht22 humidity and trmparature sensor.
*
* Created by surfero
* surfero.blogspot.com
*
*/

#include                          //http://www.rinkydinkelectronics.com/resource/UTFT/UTFT.pdf
#include                      //Touchscreen library
#include                           //Humidity and temperature sensor library
#define DCS  6                            // Chip select pin of touch screen 
#define DCLK A1                            // Clock pin of touch screen 
#define DIN  A2                            // Data in pin of touch screen 
#define DOUT 7                            // Data out pin of touch screen 
#define YAXIS 0                           //Axis of the TFT sceen
#define XAXIS 1
#define DHTPIN 3                        // Temp and humidity sensor
#define DHTTYPE DHT22                    //Sensor type
#define PRESSDELAY 200                   //Delay between touch screen readings
Touch_AS Touch(DCS, DCLK, DIN, DOUT);    // Initialize touchscreen
UTFT myGLCD(ITDB24E_8,A2,A1,A3,A0);  // Initialize display
DHT dht(DHTPIN, DHTTYPE);                // Initialize temp and humidity sensor
extern uint8_t BigFont[];                //Setting fonts for the LCD http://www.rinkydinkelectronics.com/r_fonts.php
extern uint8_t SmallFont[];
extern uint8_t SevenSegNumFont[];
unsigned long temparatureMillis = 0;     //Store the last temp reading time
int x, y;                                //Store touch cordinates
String tempUnit = "C";                  //Temperature measurement unit
unsigned long millisNow =0;             //Store touch time to ignore repeated touch signals
void setup()
{

// Initial setup
  Serial.begin(9600);
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 255);

  drawScreen(0);                         //Call draw screen function
  dht.begin();                           //Sratr the temp and humidity sensor
}

void loop()
{
    if (Touch.Pressed()){               //Check to see if the touch screen is pressed
      x=Touch.GetCoord(XAXIS);          //Get X cord 
      y=Touch.GetCoord(YAXIS);          //Get Y cord


      if(x>105 && x<225 amp="" y="">95 && y<130 span="">
        //Skip touchscreen readings for the next 500ms(set in PRESSDELAY). 
        //This is used because whenever the sceen in touched it sends the cordinates continously.
        //We only need to toggle the unit once per touch.
        if(millis()-millisNow >= PRESSDELAY){
          millisNow = millis();      
         if(tempUnit == "C"){          //Toggle temparature unit
            tempUnit = "F"; 
         }
         else if(tempUnit == "F"){

           tempUnit = "C"; 

         }
         readTemparature();
        }
      }
    }
    //Check temparature every 2000ms (2 seconds)
    if(millis() - temparatureMillis >= 2000){
      temparatureMillis = millis();
      readTemparature();
      Serial.println(temparatureMillis);
    }

}

//Function to read temparature and humidity

void readTemparature(){
  float h = dht.readHumidity();              // Read humidity
  float t = 0;
  if(tempUnit == "C"){
    t = dht.readTemperature();               // Read temperature in C
  }
  else{
    t = dht.readTemperature(true);            // Read temperature in F
  }

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } 


  //Serial.print("Temp: ");
  //Serial.print(t);
  updateTemp(t, tempUnit);
  updateHumidity(h);
}

//Function to draw screens. Parameter will set which sreen to draw (When more than one screen)
//Documentation for the screen library- http://www.rinkydinkelectronics.com/resource/UTFT/UTFT.pdf

void drawScreen(int screen){
  //0= home screen (More screens will be added later)
  if(screen == 0){
    myGLCD.setBackColor(0,0,0);
    myGLCD.setColor(255,255,255);
    myGLCD.setFont(BigFont);
    myGLCD.print("Data from sensor", CENTER, 30);
    myGLCD.setColor(255, 0, 0);
    myGLCD.drawLine(0,60,319,60);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setFont(BigFont);
    myGLCD.print("Temparature", CENTER, 70);
    myGLCD.setColor(255, 255, 255);
    myGLCD.setFont(BigFont);
    myGLCD.print("Humidity", CENTER, 150);

  }

}

//Function to update temparature

void updateTemp(float val, String unit){
    myGLCD.setColor(255,255,255);
    myGLCD.drawRoundRect (105, 95, 225, 130);
    myGLCD.setColor(0,255,0);
    myGLCD.setFont(BigFont);
    myGLCD.printNumF(val, 2, CENTER, 100);
    myGLCD.print(unit, 205, 100);

}

//Function to update humidity

void updateHumidity(float val){
    myGLCD.setColor(255,255,255);
    myGLCD.drawRoundRect (105, 175, 225, 210);
    myGLCD.setColor(0,255,0);
    myGLCD.setFont(BigFont);
    myGLCD.printNumF(val, 2, CENTER, 178);
    myGLCD.print("%", 205, 178);


}




