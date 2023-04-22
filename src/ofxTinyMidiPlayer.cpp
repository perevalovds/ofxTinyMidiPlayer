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
void ofxTinyMidiPlayer::audioOut(ofSoundBuffer& output, int flagMixing)
{

}


//--------------------------------------------------------------
