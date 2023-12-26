/*
Author  : Sandro Jurinovich
Source  : https://github.com/AndreaLombardo/L298N/

Basic usage of PumpMix library
*/

#include <PumpMix.h>

const int nPumps    = 3;                  // Define number of pumps used in mixing
const int cycleTime = 1000;               // Define duration of mixing cycle time (ms)
int pumpPins[]      = {5,6,7};            // Define pump out pin
String pumpNames[]  = {"P1","P2","P3"};   // Define pump name
int mixRatio[]      = {50,25,25};         // Define mixing ratio (in percentage)

// Create object <pump> from PumpMix class
PumpMix pump(nPumps,cycleTime,pumpPins,pumpNames);

void setup(){ 
  // Initilize pumps
  pump.begin();
  // Set mixing ratio
  pump.mix(mixRatio);
}

void loop() {
  // Cycle over pumps
  for(int i = 0; i < nPumps; i++){
    // Start mixing
    pump.mixRun(i);
  }
}