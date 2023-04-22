#include "ofxTinyMidiPlayer.h"
// Putting TinySoundFont implementation here (can't do it in h-file):
#define TML_IMPLEMENTATION
#include "tml.h"

//--------------------------------------------------------------
void ofxTinyMidiPlayer::load(string mid_file_name)
{
	if (loaded_) {
		release();
	}

	mid_file_name = ofToDataPath(mid_file_name);
	// Load the MIDI from a file
	auto* midi = tml_load_filename(mid_file_name.c_str());
	loaded_ = (midi != nullptr);
	if (!loaded_)
	{
		cout << "Could not load MIDI file " << mid_file_name << endl;
		return;
	}
	cout << "Loaded MIDI file '" << mid_file_name << "'" << endl;
	midiPlayer_ = midi;

	// TODO read info
}

//--------------------------------------------------------------
void ofxTinyMidiPlayer::release()
{
	if (loaded_) {
		loaded_ = false;
		tml_free(midiPlayer_);
		midiPlayer_ = nullptr;
	}
}

//--------------------------------------------------------------
void ofxTinyMidiPlayer::play()
{

}

//--------------------------------------------------------------
void ofxTinyMidiPlayer::stop()
{

}

//--------------------------------------------------------------
void ofxTinyMidiPlayer::audioOut(ofSoundBuffer& output, ofxTinyMidiSoundFont& soundFont, int flagMixing)
{
	if (!loaded_) {
		// Fill the output buffer by zeros to avoid random clicks
		if (!flagMixing) {
			output.set(0);
		}
		return;
	}

	ofxTinyMidiLock lock(soundFont);	// Lock resources

	auto& buffer = output.getBuffer();
	if (buffer.empty()) return;

	if (output.getNumChannels() != channels_) {
		cout << "ofxTinyMidiPlayer::audioOut error: expected " << channels_ << " channels in the buffer" << endl;
		output.set(0);
		return;
	}

	float* data = buffer.data();
	int n = buffer.size();
	int nframes = n / channels_;

	int sampleCount = output.size() / channels_;
	//renderFloatNoLock(output.getBuffer().data(), sampleCount, flagMixing);
}


//--------------------------------------------------------------
