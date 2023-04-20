#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


}

//--------------------------------------------------------------
void ofApp::exit() {
	
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
	ofxTinyMidiLock lock(soundFont_);	// Lock resources
	soundFont_.noteOn(0, note, 127);
}

//--------------------------------------------------------------
void ofApp::noteOff(int note)
{
	ofxTinyMidiLock lock(soundFont_);	// Lock resources
	soundFont_.noteOff(0, note);
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
