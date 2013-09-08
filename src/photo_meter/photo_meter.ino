#include "def.h"
#include <MsTimer2.h>
// A bitfield of run status flags, defined in def.h  
volatile byte run_status = 0;
// The exposure time given current F settings
double exposure_time = 0;
int sample_time = 1000;
int F_stop = 4;
int interval = 50000;
// Update Light Sensor run flags

void setup() 
{ 
 pinMode(PIN_SHUTTER, OUTPUT);
 pinMode(PIN_FOCUS, OUTPUT);
 pinMode(PIN_LIGHT_SENSOR, INPUT);
 digitalWrite(PIN_LIGHT_SENSOR, HIGH);
 Serial.begin(9600);
 Serial.println("Welcome to lapser");
 Serial.println("Press space to start time lapse");
 run_status = 0;
}

void loop() 
{
  // Handle any input
  if (Serial.available() > 0 && Serial.read() == 32) 
  {    
      if (run_status & FLG_RUN)
      {
        stop_run_loop();
      }
      else
      {
        Serial.println("Time lapse started, press space to stop");
        MsTimer2::set(interval, start_run_loop);
        MsTimer2::start();
      }
  }
  run_loop();
}

void start_run_loop()
{
  MsTimer2::stop();
  run_status = FLG_RUN;
}

void stop_run_loop()
{
  MsTimer2::stop();
  stopSampling();
  stopCamera();
  run_status = 0;
}

// This loop is processed when we are running
void run_loop()
{
  if(!(run_status & FLG_RUN))
  {
    return;
  }
  // Camera has fired, reset everything
  if (run_status & FLG_CM_FIN)
  {
    stop_run_loop();
    printDivider();
    MsTimer2::set(interval, start_run_loop);
    MsTimer2::start();
    return;
  }
  // Light sensor isn't running and hasn't run
  if (   !(run_status & FLG_LS_RUN) 
      && !(run_status & FLG_LS_FIN)
    )
  {
    printTiming("light sensor", sample_time);
    calculateExpTime();
  }
  // Light sensor not running but has run
  // Camera is not running and has not run
  if (   !(run_status & FLG_LS_RUN) 
      &&  (run_status & FLG_LS_FIN)
      && !(run_status & FLG_CM_RUN)
      && !(run_status & FLG_CM_FIN)
    )
  {
    printTiming("camera", exposure_time);
    fireCamera(exposure_time);
  } 
}

void printTiming(String device, int ms)
{
  Serial.print("RUNNING " + device + " for ");
  Serial.print(ms);
  Serial.println("ms");
}

void printDivider()
{
  Serial.println("=======================");
}
