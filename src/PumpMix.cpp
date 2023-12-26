#include "Arduino.h"
#include "PumpMix.h"

// ****************************************************************************
// Class constructor
//
PumpMix::PumpMix(int nPumps,unsigned long pumpCycleTime, int pumpPins[], 
  String pumpNames[]){//,HardwareSerial &print){
	_NP        = nPumps;
	_pumpPins  = pumpPins;
	_pumpNames = pumpNames;
	_pumpCycleTime = pumpCycleTime;
  //printer = &print;
}


// ****************************************************************************
// Setup function to set up pump output and initialize variables
// 
void PumpMix::begin(){
	for(int i = 0; i < _NP; i++){
    // Set pin mode OUTPUT
    pinMode(_pumpPins[i],OUTPUT);
    _nCycles[i] = 0;      // Set the cycle counter to zero
    _startCycle[i] = 0;   // Set the start cycle time to zero
    // Stop all pumps
    allStop();
	}
}

// ****************************************************************************
// Simple functions to control pumps
// 

// Start a single pump
void PumpMix::start(int i){
	digitalWrite(_pumpPins[i],ON);
}

// Stops a single pump
void PumpMix::stop(int i){
	digitalWrite(_pumpPins[i],OFF);
}

// Starts all the pumps
void PumpMix::allStart(){
	for(int i = 0; i < 3; i++){
		start(i);
	}
}

// Stops all the pumps
void PumpMix::allStop(){
	for(int i = 0; i < 3; i++){
		stop(i);
	}
}

// Change the pump state
void PumpMix::changeState(int i, bool state){
	_pumpStates[i] = state;
}


// ****************************************************************************
// Return some basic information
// 

// Get current pump state
bool PumpMix::getState(int i){
	return _pumpStates[i];
}

float PumpMix::getMix(int i){
  return _pumpTimeOn[i];
}

// Get the cycle number of mixing
unsigned long PumpMix::getCycle(int i){
  return _nCycles[i];
}


// ****************************************************************************
// Define the time functioning intervals depending on <mixPerc>
// This function have to be called at least one time before mixRun
// 

void PumpMix::mix(int mixPerc[]){

  float mixRatio[_NP] = {0};
  float scale = 1.0;

  // Controlla che la somma sia 100
  int sum = 0;

  if( _NP > 1 ){
    for (int i = 0; i < _NP; i++) {
      sum += mixPerc[i];
    }
  }else{
    sum = 100;
  }
  
  // Se la somma non è 100 riscala le percentuali a 100
  if(sum != 100){
    scale = 100.0/sum;
  }

  // Crea array di mixRatio
  for (int i = 0; i < _NP; i++) {
      mixRatio[i] = mixPerc[i]*scale/100;
  }

  // Azzera i parametri TimeDel, TimeOn, TimeOff per tutte le pompe
  for (int i = 0; i < _NP; i++) {
    _pumpTimeDel[i] = 0;
    _pumpTimeOn[i]  = 0;
    _pumpTimeOff[i] = 0;
  }

  // Scrive i parametri per il ciclo della pompa
  int initPump = 0;
  unsigned long delayTime = 0;
  unsigned long offTime   = 0;
  unsigned long onTime    = 0;
  for (int i = 0; i < _NP ; i++) {
    if(mixRatio[i] > 0.0){
      onTime = mixRatio[i]*_pumpCycleTime;
      _pumpTimeDel[i] = delayTime;
      _pumpTimeOn[i]  = onTime;
      _pumpTimeOff[i] = _pumpCycleTime-(delayTime+onTime);
      initPump = i;
      delayTime += onTime;
    }
  }

  /*
  for (int i = 0; i < _NP; i++) {
    printer->print(_pumpTimeDel[i]); printer->print("\t");
    printer->print(_pumpTimeOn[i]);  printer->print("\t");
    printer->print(_pumpTimeOff[i]); printer->print("\t");
    printer->println();
  }
  */

}


// ****************************************************************************
// Control pumps on the basis of time intervals defined in <mix> function
// 
// 

void PumpMix::mixRun(int i){

  // If this is the first cycle, intiialize variables
  if( _nCycles[i] == 0 ){
    _startCycle[i] = millis();
    _nCycles[i]++;
    _pumpStates[i] = true;
  }

  // If pumpState == True ... (pumps is enabled to work )
  if(_pumpStates[i] == true){
    // if pumpTimeOn == 0 does not control TimeDel and TimeOn/Off, pump always ON
    if(_pumpTimeOn[i] == 0){
      start(i);
    }
    // else pumpTimeOn != 0, check TimeDel and TimeOn/Off
    else{
      unsigned long dT = millis() - _startCycle[i];
      // we are inside the pump time cycle
      if(dT < _pumpCycleTime){
        // we are inside the deleay time, pump should not work
        if( dT < _pumpTimeDel[i]){
          stop(i);
        }
        // we are exceeded the time delay, pump ON/OFF depending on TimeOn
        else{
          // we are inside the TimeOn interval, pump ON
          if(dT < ( _pumpTimeDel[i] + _pumpTimeOn[i]) ){
            start(i);
          }
          // we are outside the TimeOn interval, pump OFF
          else{
            stop(i);
          }
        }
      }
      else{
        // Start a new cycle
        _startCycle[i] = millis();
        _nCycles[i]++ ;
      }
    } 
  }
  // pumpState == False (pump is not enabled to work)
  else{
    // Turn off the pump
    stop(i);
  }

}