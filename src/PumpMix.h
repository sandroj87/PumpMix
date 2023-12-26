#ifndef PumpMix_h
#define PumpMix_h

#include "Arduino.h"

#define NMAX 8		// Maximum dimension of pump array
#define ON	HIGH	// Define which state of digital output corresponds to ON
#define OFF LOW		// Define which state of digital output corresponds to OFF

class PumpMix
{
	private:
		int _NP;						// Number of pumps
		uint32_t _pumpCycleTime;		// Pump cycle time (ms)
		int *_pumpPins;					// Pump pinouts
		String 	*_pumpNames;			// Pump names
		bool _pumpStates[NMAX];			// Pump states
		uint32_t  _pumpTimeDel[NMAX];	// Delay time
    	uint32_t  _pumpTimeOn[NMAX];	// On time
    	uint32_t  _pumpTimeOff[NMAX];	// Off time
		uint32_t  _startCycle[NMAX];	// Start cycle time
		uint32_t  _nCycles[NMAX] = {0};	// Counter for cycles
		
	public:
		PumpMix(int nPumps,unsigned long pumpCycleTime,int pumpPins[], 
			String pumpNames[]);
		void begin();
		void start(int i);
		void stop(int i);
		void allStart();		
		void allStop();
		void changeState(int i, bool state);
		bool getState(int i);
		float getMix(int i);
		uint32_t  getCycle(int i);
		void mix(int mixPerc[]);
		void mixRun(int i);

};

#endif