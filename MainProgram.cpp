//Software for Magical Fountain 
//ECE 3872 Project
//Written by Austin Crawford

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>
#include <string.h>

using namespace std;

int notes[20] = { 0 };
int prevSwitch;
bool prevReset, resetVal;
chrono::time_point<chrono::system_clock> startTime;
chrono::time_point<chrono::system_clock> endTime;
double timer;
int noteIn;
int length = 0;
int mode = 0;
int playLoc = 0;
bool  first;

void intialHw()
{
	//this is where all the hardware initialization will be added
	
}

void deleteRecord()
{
	//sets the entire record array (notes) to 0 and sets the internal length back to 0
	memset(notes, 0, 20);
	length = 0;
	return;
}


void audioOff()
{
	//We will turn off the speakers in this function
}

void ledOff()
{
	//We will turn off the LED chain in this function
}

void valveOff()
{
	//We will close all the valves in this function
}

void playNote(int note)
{
	//We will feed in the integer that corresponds to a note
	//This function will actually play the note
}

void ledOn(int note)
{
	//We will feed in the integer that corresponds to a note
	//This function will actually turn on corresponding LED
}

void valveOn(int note)
{
	//We will feed in the integer that corresponds to a note
	//This function will actually open corresponding valve
}

int switchIn()
{
	int switchVal;
	//We will obtain the switch value from inputs here
	//values will be converted into an integer and passed out where
	// 0 = stop, 1  = record, 2 = play record, 3 = play live
	return switchVal;
}

bool resetIn()
{
	bool reset;
	//We will obtain input value from buton here
	return reset;
}

int sensorIn()
{
	int sensorInput;
	//We will take the sensor input information and correspond it to  notes
	//0 will be no input
	//1-8 will be the 8 required notes
	return sensorInput;
}




void switchMove()
{
	//This function moves to the correct state based on the switch input
	//This function in addition will always do the proper initialization for each state
	//values are for 0 is idle 
	//1 is record
	//2 is play record
	//3 is play live
	switch (switchIn())
	{
	case 0:
		audioOff();
		ledOff();
		valveOff();
		prevSwitch = switchIn();
		prevReset = false;
		mode = 0;
		break;
	case 1:
		audioOff();
		ledOff();
		valveOff();
		noteIn = sensorIn();
		mode = 1;
		break;
	case 2:
		audioOff();
		ledOff();
		valveOff();
		noteIn = sensorIn();
		playLoc = 0;
		first = true;
		mode = 2;
		break;
	case 3:
		audioOff();
		ledOff();
		valveOff();
		noteIn = sensorIn();
		mode = 3;
		break;
	}
}

void addRecord()
//This function will add the note that is input to the recording
//It will also update the length if the array isnt full
//or append the array if it is full
{
	if (length >= 20)
	{
		memmove(notes, notes + 1, 19);
		notes[19] = noteIn;
	}
	else
	{
		notes[length] = noteIn;
		length++;
	}
}



int main()
{
	intialHw();
	prevSwitch = 5; //making this a value so that the initialization will run for any opening state
	while (true)
		//main loop the program will run on
	{
		//the check for user input to change states
		if (prevSwitch != (switchIn()))
		{
			switchMove();
			prevSwitch = switchIn();
		}
		if (resetIn())
			mode = 0;
		
		
		switch (mode)
		{
			//the switch case is the main logic that runs the state machine
		case 0:
			//idle mode
			audioOff();
			ledOff();
			valveOff();
			prevSwitch = switchIn();
			prevReset = false;

			if (resetIn() & prevReset)
			{
				endTime = chrono::system_clock::now();
				timer = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
				if (timer >= 2000)
				{
					prevReset = false;
					deleteRecord();
				}

			}
			else if (resetIn())
			{
				startTime = chrono::system_clock::now();
				prevReset = true;
			}
			else
			{
				prevReset = false;
			}
			break;
		case 1:
			//record mode
			if (noteIn)
			{
				addRecord();
				playNote(noteIn);
			}
			break;
		case 2:
			//play record mode
			if (playLoc < 19)
			{
				playNote(notes[playLoc]);
				valveOn(notes[playLoc]);
				ledOn(notes[playLoc]);
				playLoc++;
				first = true;
			}
			else
			{
				if (first)
				{
					playNote(notes[playLoc]);
					valveOn(notes[playLoc]);
					ledOn(notes[playLoc]);
					startTime = chrono::system_clock::now();
					first = false;
				}
				else
				{
					endTime = chrono::system_clock::now();
					timer = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
					audioOff();
					ledOff();
					valveOff();
					if (timer > 500)
					{
						playLoc = 0;
					}
				}
			}
			break;
		case 3:
			//play live mode
			if (noteIn)
			{
				playNote(noteIn);
				valveOn(noteIn);
				ledOn(noteIn);
			}
		}
	}
	return 0;
}