#pragma once

#include "ofMain.h"
#include "ofxTinyMidiSoundFont.h"
#include "tml.h"

class ofxTinyMidiPlayer {
public:
	void load(string mid_file_name);
	void release();	// Free resources

	// Play loaded MIDI file
	void play();
	void stop(ofxTinyMidiSoundFont& soundFont);

	// Audio callback
	// Safe - locks own and soundFont resources.
	// By default mixing if off, it means replacing values in output (if 1 then adding to output)
	void audioOut(ofSoundBuffer& output, ofxTinyMidiSoundFont& soundFont, int flagMixing);

	// Special settings
	// Size of audio chunks on which MIDI events are sent.
	// Higher values give more stability, less values give more accurate playing.
	void setAudioChunkSize(int chunkSizePerMIDIEvents);
	int audioChunkSize();

	// Mutex
	mutex& mut() { return mutex_; }
private:
	mutex mutex_;
	bool loaded_ = false;
	tml_message* midiPlayer_ = nullptr;		// Start of MIDI data represented as a linked list 
	static const int channels_ = 2;

	bool playing_ = false;
	tml_message* player_message_ = nullptr;	// Next message to be played
	double player_msec_ = 0;					// Current playback time

	// Settings
	int chunkSizePerMIDIEvents_ = 64;
};
