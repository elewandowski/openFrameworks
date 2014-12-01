#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Keys.h"

// listen on port 12345
#define PORT 12000
#define NUM_MSG_STRINGS 20
#define KEY_NAME_1 "debussy"

class ofApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofTrueTypeFont font;
		ofxOscReceiver receiver;

		int current_msg_string;
		string msg_strings[NUM_MSG_STRINGS];
		float timers[NUM_MSG_STRINGS];
		array<float, 17> cp;
		int editControlPoint;

        array<int, 9> ccArray;
        array<Keys, 5> instArray;
        void increaseLineLength();
        array<string, 100> splitOSCMessageAddress(string s);
        int stoi(string s);

        float lineStart, lineEnd, lineStartTheta, lineEndTheta, lineStartBase, lineEndBase, lineSpeed, lineMult;
};
