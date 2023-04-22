#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	pressedNotes_.resize(128);

	// Load sound font
	soundFont_.load("gm.sf2");

	// Choose some instrument
	setInstrument(0);

	// Start audio stream
	setupSoundStream();
}

//--------------------------------------------------------------
void ofApp::setupSoundStream()
{
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
	soundStream_.close();
	soundFont_.release();
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& output) {
	// expected 2 channels
	if (output.getNumChannels() != 2) {
		cout << "audioOut - expected stereo buffer to fill" << endl;
		return;
	}

	auto& buffer = output.getBuffer();
	if (buffer.empty()) return;

	float* data = buffer.data();
	int n = buffer.size();
	int nframes = n / output.getNumChannels();

	int flagMixing = 0;
	soundFont_.audioOut(output, flagMixing);
	// Apply compression and final volume
	// for (int k = 0; k < n; k++) {
	//	 float& v = data[k];
	//	 v *= Volume;
	// }

}

//--------------------------------------------------------------
void ofApp::update(){
	

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(64);
	
}

//--------------------------------------------------------------
void ofApp::setInstrument(int instr)
{
	ofxTinyMidiLock lock(soundFont_);	// Lock resources
	soundFont_.channelSetProgram(0, instr);

	// TODO read instrName
}

//--------------------------------------------------------------
void ofApp::noteOn(int note)
{
	if (!pressedNotes_[note])
	{
		pressedNotes_[note] = 1;
		ofxTinyMidiLock lock(soundFont_);	// Lock resources
		soundFont_.noteOn(0, note, 127);
	}
}

//--------------------------------------------------------------
void ofApp::noteOff(int note)
{
	if (pressedNotes_[note])
	{
		pressedNotes_[note] = 0;
		ofxTinyMidiLock lock(soundFont_);	// Lock resources
		soundFont_.noteOff(0, note);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// 1-9 - change instrument
	// q..p - play note
	if (key == '1') setInstrument(0);
	if (key == '2') setInstrument(1);
	if (key == '3') setInstrument(2);
	if (key == '4') setInstrument(3);
	if (key == '5') setInstrument(4);
	if (key == '6') setInstrument(5);
	if (key == '7') setInstrument(6);
	if (key == '8') setInstrument(7);
	if (key == '9') setInstrument(8);
	if (key == 'q') noteOn(50);
	if (key == 'w') noteOn(52);
	if (key == 'e') noteOn(54);
	if (key == 'r') noteOn(55);
	if (key == 't') noteOn(57);
	if (key == 'y') noteOn(59);
	if (key == 'u') noteOn(61);
	if (key == 'i') noteOn(62);
	if (key == 'o') noteOn(64);
	if (key == 'p') noteOn(66);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'q') noteOff(50);
	if (key == 'w') noteOff(52);
	if (key == 'e') noteOff(54);
	if (key == 'r') noteOff(55);
	if (key == 't') noteOff(57);
	if (key == 'y') noteOff(59);
	if (key == 'u') noteOff(61);
	if (key == 'i') noteOff(62);
	if (key == 'o') noteOff(64);
	if (key == 'p') noteOff(66);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
