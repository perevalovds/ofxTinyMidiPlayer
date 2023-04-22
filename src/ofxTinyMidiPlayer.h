#pragma once

#include "ofMain.h"
#include "ofxTinyMidiSoundFont.h"
#include "tml.h"

class ofxTinyMidiPlayer {
public:
	void load(string mid_file_name);
	void release();	// Free resources

	void play();
	void stop();

	// Audio callback
	// By default mixing if off, it means replacing values in output (if 1 then adding to output)
	void audioOut(ofSoundBuffer& output, int flagMixing);
private:
	bool loaded_ = false;
	tml_message* midiPlayer_ = nullptr;		// Start of MIDI data represented as a linked list 
	tml_message* message_ = nullptr;	// Next message to be played
	double msec_= 0;					// Current playback time

};
