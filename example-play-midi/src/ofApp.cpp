#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	// Load sound font
	soundFont_.load("gm.sf2");

	// Load MIDI file
	midiPlayer_.load("Joy Division - Transmission.mid");

	soundFont_.channelSetProgramUnsafe(0, 0);
	soundFont_.noteOnUnsafe(0, 50, 100);

	// Start audio stream
	setupSoundStream();

	// Start playing MIDI
	midiPlayer_.play();
}

//--------------------------------------------------------------
void ofApp::setupSoundStream()
{
	cout << "Starting audio output" << endl;
	// Api: 
	// DEFAULT
	// MS_WASAPI /*!< The Microsoft WASAPI API. */
	// MS_ASIO   /*!< The Steinberg Audio Stream I/O API. */
	// MS_DS     /*!< The Microsoft Direct Sound API. */

	ofSoundDevice::Api API = //ofSoundDevice::Api::MS_ASIO  // Use this for ASIO sound card for the better responsibilty, SR 44100, buffer 64, buffers 2
		ofSoundDevice::Api::MS_DS;  // Use this for internal sound card, then need buffer 128, buffers 4

	auto devices = soundStream_.getDeviceList(API);
	//auto devices = soundStream_.getMatchingDevices("default");
	cout << devices << endl;
	if (devices.empty()) {
		cout << "No sound devices" << endl;
	}
	else {

		//settings.setInDevice(devices[0]);
		auto& device = devices[0];
		if (device.outputChannels == 0 && device.sampleRates.empty()) {
			ofSystemAlertDialog("Can't initialize output device " + device.name);
			_exit(0);
		}

		ofSoundStreamSettings settings;
		settings.setApi(API);
		settings.setOutDevice(device);

		settings.setOutListener(this);
		settings.sampleRate = 44100;
		settings.numOutputChannels = device.outputChannels;
		settings.numInputChannels = 0; //device.inputChannels;
		settings.bufferSize = 512;
		settings.numBuffers = 4;
		soundStream_.setup(settings);
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	midiPlayer_.release();
	soundStream_.close();
	soundFont_.release();
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& output) {
	// expected 2 channels
	if (output.getNumChannels() != 2) {
		cout << "ofApp::audioOut - expected stereo buffer" << endl;
		return;
	}

	int flagMixing = 0;
	//midiPlayer_.audioOut(output, soundFont_, flagMixing);
	soundFont_.audioOut(output, flagMixing);
}

//--------------------------------------------------------------
void ofApp::update() {


}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(64);
	ofSetColor(255);

	ofDrawBitmapString("Playing MIDI", 30, 30);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
