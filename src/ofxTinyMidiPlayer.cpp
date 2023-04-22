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
	if (!loaded_) {
		return;
	}
	if (playing_) {
		return;
	}
	ofxTinyMidiLock lock(mutex_);		// Lock own resources
	playing_ = true;
	player_message_ = midiPlayer_;	
	player_msec_ = 0;
}

//--------------------------------------------------------------
void ofxTinyMidiPlayer::stop(ofxTinyMidiSoundFont& soundFont)
{
	if (!loaded_) {
		return;
	}
	if (playing_) {
		return;
	}
	ofxTinyMidiLock lock(mutex_);		// Lock own resources
	playing_ = false;

	//Stop all notes
	soundFont.stopAllNotes();			// Locks sound font resources
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

	if (output.getNumChannels() != channels_) {
		cout << "ofxTinyMidiPlayer::audioOut error: expected " << channels_ << " channels in the buffer" << endl;
		output.set(0);
		return;
	}

	if (output.size() == 0) return;

	ofxTinyMidiLock lock(mutex_);		// Lock own resources

	if (!playing_) {
		return;
	}

	ofxTinyMidiLock lockSoundFont(soundFont);	// Lock sound font resources

	// Generate audio by chunks
	int sampleCount = output.size() / channels_;
	float* data = output.getBuffer().data();

	int sampleBlock;
	for (sampleBlock = chunkSizePerMIDIEvents_; sampleCount > 0; 
		sampleCount -= sampleBlock, data += sampleBlock * channels_)
	{
		//We progress the MIDI playback and then process TSF_RENDER_EFFECTSAMPLEBLOCK samples at once
		if (sampleBlock > sampleCount) sampleBlock = sampleCount;

		//Loop through all MIDI messages which need to be played up until the current playback time
		auto& msg = player_message_;
		for (player_msec_ += sampleBlock * (1000.0 / 44100.0); msg && player_msec_ >= msg->time; msg = msg->next)
		{
			if (!msg) {
				cout << "ofxTinyMidiPlayer::audioOut internal error, mgs == nullptr" << endl;
				return;
			}
			switch (msg->type)
			{
			case TML_PROGRAM_CHANGE: //channel program (preset) change (special handling for 10th MIDI channel with drums)
				soundFont.channelSetProgramUnsafe(msg->channel, msg->program);
				break;
			case TML_NOTE_ON: //play a note
				soundFont.noteOnUnsafe(msg->channel, msg->key, msg->velocity / 127.0f);
				break;
			case TML_NOTE_OFF: //stop a note
				soundFont.noteOffUnsafe(msg->channel, msg->key);
				break;
			case TML_PITCH_BEND: //pitch wheel modification
				soundFont.pitchBendUnsafe(msg->channel, msg->pitch_bend);
				break;
			case TML_CONTROL_CHANGE: //MIDI controller messages
				soundFont.controlChangeUnsafe(msg->channel, msg->control, msg->control_value);
				break;
			}
		}

		// Render the block of audio samples in float format
		soundFont.renderFloatUnsafe(output.getBuffer().data(), sampleCount, flagMixing);
	}

}

//--------------------------------------------------------------
void ofxTinyMidiPlayer::setAudioChunkSize(int chunkSizePerMIDIEvents)
{
	ofxTinyMidiLock lock(mutex_);		// Lock own resources
	chunkSizePerMIDIEvents_ = chunkSizePerMIDIEvents;
}

//--------------------------------------------------------------
int ofxTinyMidiPlayer::audioChunkSize()
{
	return chunkSizePerMIDIEvents_;
}

//--------------------------------------------------------------
