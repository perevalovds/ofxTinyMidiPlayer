#pragma once

#include "ofMain.h"
#include "ofxTinyMidiSoundFont.h"
#include "tml.h"

class ofxTinyMidiPlayer {
public:

private:
	tml_message* midi_ = nullptr;			// Start of MIDI data represented as a linked list 
	tml_message* message_ = nullptr;		// Next message to be played
	double msec_;              // Current playback time

};
