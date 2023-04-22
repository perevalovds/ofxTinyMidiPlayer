#pragma once

#include "ofMain.h"
#include "tsf.h"

class ofxTinyMidiSoundFont {
public:
	void load(string sf2_file_name, int sampleRate = 44100, float volumeDb = 0);

	void release();	// Free resources

	string instrumentName(int i);

	// Audio callback
	// This is higher-level audio generation, with resource locking
	// To generate with MIDI - use ofxTinyMidiPlayer::audioOut with 
	// By default mixing if off, it means replacing values in output (if 1 then adding to output)
	void audioOut(ofSoundBuffer& output, int flagMixing = 0);

	// Rendering audio to a buffer
	// This is lower-level audio generation, without resource locking, 
	// so lock by yourself if required
	void renderFloat(float* outputStereo, int nStereoSamples, int flagMixing = 0);

	// Events for playing sounds
	// It can be called from MIDI player or manually
	// When calling events from non-audio thread, need to lock resources:
	//    ofxTinyMidiLock lock(soundFont); 
	// Then inside scope of this "lock" object resources will be locked
	void channelSetProgram(int channel /*0..15*/, int program);
	void noteOn(int channel, int key, int velocity /*0..127*/);
	void noteOff(int channel, int key);
	void pitchBend(int channel, int pitchBend);
	void controlChange(int channel, int control, int controlValue);

	// Mutex
	mutex& mut() { return mutex_; }
private:
	bool useMutex_ = true;
	bool loaded_ = false;

	tsf* soundFont_ = nullptr;

	static const int channels_ = 2;

	mutex mutex_;
};


// Conditional locker, enables mutex if required
// Locks in its visibility area until destroying
class ofxTinyMidiLock {
public:
	ofxTinyMidiLock(mutex& mut): lock_(mut) {}
	ofxTinyMidiLock(ofxTinyMidiSoundFont& soundFont) : lock_(soundFont.mut()) {}
private:
	lock_guard<mutex> lock_;
};