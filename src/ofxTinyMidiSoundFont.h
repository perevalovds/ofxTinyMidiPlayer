#pragma once

#include "ofMain.h"

#include "tsf.h"

//#include <mutex>                    // std::mutex, std::lock_guard

class ofxTinyMidiSoundFont {
public:
	void setup(bool useMutex = true); // Disable mutex if you are setting notes and generate audio from the same thread
	void load(string sf2_file_name);

	void release();	// Free resources

	// Generating audio
	// By default mixing if off, so "replacing" (if 1 then "adding")
	void audioOut(ofSoundBuffer& output, int flagMixing = 0);

	// Midi events
	void channelSetProgram(int channel /*0..15*/, int program);
	void noteOn(int channel, int key, int velocity /*0..127*/);
	void noteOff(int channel, int key);
	void pitchBend(int channel, int pitchBend);
	void controlChange(int channel, int control, int controlValue);

private:
	bool useMutex_ = true;
	bool loaded_ = false;

	tsf* soundFont_ = nullptr;

	static const int channels_ = 2;

	mutex mutex_;

	// Conditional locker, enables mutex if required
	// Works in its visibility area until destroying
	struct CondLock {
		CondLock(mutex &mut, bool useMutex) {
			if (useMutex) {
				lock_ = new lock_guard<mutex>(mut);
			}
		}
		~CondLock() {
			if (lock_) {
				delete lock_;
			}
		}
		lock_guard<mutex>* lock_ = nullptr;
	};


};

