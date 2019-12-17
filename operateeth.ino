/*  Example of playing a sampled sound,
    using Mozzi sonification library.
  
    Demonstrates one-shot samples scheduled
    with EventDelay.
  
    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.0/3.1, or 
    check the README or http://sensorium.github.com/Mozzi/
  
    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users
  
    Tim Barrass 2012, CC by-nc-sa.
*/

//#include <ADC.h>  // Teensy 3.0/3.1 uncomment this line and install http://github.com/pedvide/ADC
#include <MozziGuts.h>
#include <Sample.h> // Sample template
#include <samples/operatable.h>
#include <EventDelay.h>
const int buttonPin = 5;
int buttonState = LOW;
#define CONTROL_RATE 63

// use: Sample <table_size, update_rate> SampleName (wavetable)
Sample <operasample_NUM_CELLS, AUDIO_RATE> aSample(operasample_DATA);

// for scheduling sample start
EventDelay kTriggerDelay;

boolean triggered = false;

EventDelay kGainChangeDelay;
char gain = 1;

void setup(){

  startMozzi(CONTROL_RATE);
  aSample.setFreq((float) (operasample_SAMPLERATE * .6) / (float) operasample_NUM_CELLS); // play at the speed it was recorded
  kTriggerDelay.set(1500); // 1500 msec countdown, within resolution of CONTROL_RATE
  
  kGainChangeDelay.set(60000); // 1 second countdown, within resolution of CONTROL_RATE
  kGainChangeDelay.start();
}


void updateControl(){
  pinMode(buttonPin, INPUT);
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH){
    if(!triggered){
      kGainChangeDelay.set(9000); // 1 second countdown, within resolution of CONTROL_RATE
      kGainChangeDelay.start();
      //kTriggerDelay.start();
      triggered = true;
    }
  }

  if (kGainChangeDelay.ready()) {
    aSample.start();
    kGainChangeDelay.set(60000); // 1 second countdown, within resolution of CONTROL_RATE
    kGainChangeDelay.start();
    triggered = false;
  }
}

int updateAudio(){
  return (int) aSample.next();
}


void loop(){
  audioHook();
}
