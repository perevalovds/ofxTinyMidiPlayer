#include "ofxTinyMidiSoundFont.h"

// Putting TinySoundFont implementation here (can't do it in h-file):
#define TSF_IMPLEMENTATION

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::setup(bool useMutex)
{
	useMutex_ = useMutex;
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::load(string sf2_file_name)
{
	// Load the SoundFont from a file
	sf2_file_name = ofToDataPath(sf2_file_name);
	auto* soundFont = tsf_load_filename(sf2_file_name.c_str());
	loaded_ = (soundFont != nullptr);
	if (loaded_) {
		soundFont_ = soundFont;
	}
	else {
		cout << "Could not load SoundFont " << sf2_file_name << endl;
	}
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::release()
{
	if (loaded_) {
		loaded_ = false;
		tsf_close(soundFont_);
		soundFont_ = nullptr;
	}
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::audioOut(ofSoundBuffer& output, int flagMixing)
{
	if (!loaded_) {
		// Fill the output buffer by zeros to avoid random clicks
		if (!flagMixing) {
			output.set(0);
		}
		return;
	}
	
	CondLock locker(mutex_, useMutex_);	// Lock resources if required
	int sampleCount = output.size() / channels_; 
	tsf_render_float(soundFont_, output.getBuffer().data(), sampleCount, flagMixing);
}

//--------------------------------------------------------------
// Midi events
void ofxTinyMidiSoundFont::channelSetProgram(int channel, int program)
{
	if (!loaded_) {
		return;
	}
	CondLock locker(mutex_, useMutex_);	// Lock resources if required

	// "== 9" is a special check for drum channel
	tsf_channel_set_presetnumber(soundFont_, channel, program, (channel == 9));
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::noteOn(int channel, int key, int velocity)
{
	if (!loaded_) {
		return;
	}
	CondLock locker(mutex_, useMutex_);	// Lock resources if required
	tsf_channel_note_on(soundFont_, channel, key, velocity / 127.0f);
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::noteOff(int channel, int key)
{
	if (!loaded_) {
		return;
	}
	CondLock locker(mutex_, useMutex_);	// Lock resources if required
	tsf_channel_note_off(soundFont_, channel, key);
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::pitchBend(int channel, int pitchBend)
{
	if (!loaded_) {
		return;
	}
	CondLock locker(mutex_, useMutex_);	// Lock resources if required
	tsf_channel_set_pitchwheel(soundFont_, channel, pitch_bend);
}

//--------------------------------------------------------------
void ofxTinyMidiSoundFont::controlChange(int channel, int control, int controlValue)
{
	if (!loaded_) {
		return;
	}
	CondLock locker(mutex_, useMutex_);	// Lock resources if required
	tsf_channel_midi_control(soundFont_, channel, control, control_value);
}

//--------------------------------------------------------------

/*
// Holds the global instance pointer
static tsf* soundFont_;

// A Mutex so we don't call note_on/note_off while rendering audio samples
static SDL_mutex* g_Mutex;

static void AudioCallback(void* data, Uint8* stream, int len)
{
	// Render the audio samples in float format
	int SampleCount = (len / (2 * sizeof(float))); //2 output channels
	SDL_LockMutex(g_Mutex); //get exclusive lock
	tsf_render_float(soundFont_, (float*)stream, SampleCount, 0);
	SDL_UnlockMutex(g_Mutex);
}

int main(int argc, char* argv[])
{
	int i, Notes[7] = { 48, 50, 52, 53, 55, 57, 59 };

	// Define the desired audio output format we request
	SDL_AudioSpec OutputAudioSpec;
	OutputAudioSpec.freq = 44100;
	OutputAudioSpec.format = AUDIO_F32;
	OutputAudioSpec.channels = 2;
	OutputAudioSpec.samples = 4096;
	OutputAudioSpec.callback = AudioCallback;

	// Initialize the audio system
	if (SDL_AudioInit(TSF_NULL) < 0)
	{
		fprintf(stderr, "Could not initialize audio hardware or driver\n");
		return 1;
	}

	// Load the SoundFont from a file
	soundFont_ = tsf_load_filename("florestan-subset.sf2");
	if (!soundFont_)
	{
		fprintf(stderr, "Could not load SoundFont\n");
		return 1;
	}

	// Set the SoundFont rendering output mode
	tsf_set_output(soundFont_, TSF_STEREO_INTERLEAVED, OutputAudioSpec.freq, 0);

	// Create the mutex
	g_Mutex = SDL_CreateMutex();

	// Request the desired audio output format
	if (SDL_OpenAudio(&OutputAudioSpec, TSF_NULL) < 0)
	{
		fprintf(stderr, "Could not open the audio hardware or the desired audio output format\n");
		return 1;
	}

	// Start the actual audio playback here
	// The audio thread will begin to call our AudioCallback function
	SDL_PauseAudio(0);

	// Loop through all the presets in the loaded SoundFont
	for (i = 0; i < tsf_get_presetcount(soundFont_); i++)
	{
		//Get exclusive mutex lock, end the previous note and play a new note
		printf("Play note %d with preset #%d '%s'\n", Notes[i % 7], i, tsf_get_presetname(soundFont_, i));
		SDL_LockMutex(g_Mutex);
		tsf_note_off(soundFont_, i - 1, Notes[(i - 1) % 7]);
		tsf_note_on(soundFont_, i, Notes[i % 7], 1.0f);
		SDL_UnlockMutex(g_Mutex);
		SDL_Delay(1000);
	}

	// We could call tsf_close(soundFont_) and SDL_DestroyMutex(g_Mutex)
	// here to free the memory and resources but we just let the OS clean up
	// because the process ends here.
	return 0;
}*/

//--------------------------------------------------------------
