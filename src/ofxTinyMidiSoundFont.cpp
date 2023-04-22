#include "ofxTinyMidiSoundFont.h"

// Putting TinySoundFont implementation here (can't do it in h-file):
#define TSF_IMPLEMENTATION
#include "tsf.h"

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::load(string sf2_file_name, int sampleRate, float volumeDb)
{
	if (loaded_) {
		release();
	}

	ofxTinyMidiLock locker(mutex_);	// Lock resources

	sf2_file_name = ofToDataPath(sf2_file_name);
	// Load the SoundFont from a file
	auto* soundFont = tsf_load_filename(sf2_file_name.c_str());
	loaded_ = (soundFont != nullptr);
	if (loaded_) {
		cout << "Loaded SoundFont '" << sf2_file_name << "'" << endl;
		soundFont_ = soundFont;

		// Set the SoundFont rendering output mode and volume		
		tsf_set_output(soundFont_, TSF_STEREO_INTERLEAVED, sampleRate, volumeDb);
	}
	else {
		cout << "Could not load SoundFont " << sf2_file_name << endl;
	}
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::release()
{
	if (loaded_) {
		ofxTinyMidiLock locker(mutex_);	// Lock resources
		loaded_ = false;
		tsf_close(soundFont_);
		soundFont_ = nullptr;
	}
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::audioOut(ofSoundBuffer& output, int flagMixing)
{
	ofxTinyMidiLock locker(mutex_);	// Lock resources - for checking "loaded_" and further

	if (!loaded_) {
		// Fill the output buffer by zeros to avoid random clicks
		if (!flagMixing) {
			output.set(0);
		}
		return;
	}

	auto& buffer = output.getBuffer();
	if (buffer.empty()) return;

	if (output.getNumChannels() != channels_) {
		cout << "ofxTinyMidiSoundFont::audioOut error: expected " << channels_ << " channels in the buffer" << endl;
		output.set(0);
		return;
	}

	float* data = buffer.data();
	int n = buffer.size();
	int nframes = n / output.getNumChannels();
	
	int sampleCount = output.size() / channels_; 
	tsf_render_float(soundFont_, output.getBuffer().data(), sampleCount, flagMixing);
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::channelSetProgram(int channel, int program)
{
	if (!loaded_) {
		return;
	}
	// "== 9" is a special check for drum channel
	tsf_channel_set_presetnumber(soundFont_, channel, program, (channel == 9));
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::noteOn(int channel, int key, int velocity)
{
	if (!loaded_) {
		return;
	}
	tsf_channel_note_on(soundFont_, channel, key, velocity / 127.0f);
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::noteOff(int channel, int key)
{
	if (!loaded_) {
		return;
	}
	tsf_channel_note_off(soundFont_, channel, key);
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::pitchBend(int channel, int pitchBend)
{
	if (!loaded_) {
		return;
	}
	tsf_channel_set_pitchwheel(soundFont_, channel, pitchBend);
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::controlChange(int channel, int control, int controlValue)
{
	if (!loaded_) {
		return;
	}
	tsf_channel_midi_control(soundFont_, channel, control, controlValue);
}

//--------------------------------------------------------------
