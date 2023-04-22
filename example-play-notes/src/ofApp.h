#pragma once

#include "ofMain.h"
#include "ofxTinyMidiPlayer.h"


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void audioOut(ofSoundBuffer& output);

private:
	ofSoundStream soundStream_;
	void setupSoundStream();

	ofxTinyMidiSoundFont soundFont_;
	// ofxTinyMidiPlayer midi_;

	void setInstrument(int instr);
	void noteOn(int note);
	void noteOff(int note);

	string instrName;

	vector<int> pressedNotes_; // Flags to prevent multiple presses of a note
};
