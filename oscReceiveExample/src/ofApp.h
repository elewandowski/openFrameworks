#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Keys.h"
//#include "Visual.h"
#include "Core.h"


// listen on port 12345
#define PORT 12000
#define NUM_MSG_STRINGS 20
#define NUM_OF_POINTS 100

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
		array<float, 8> cp;
		int editControlPoint;

        array<ofImage, 10> imageArray;
        //ofMesh
        array<int, 9> ccArray;
        array<Keys, 5> instArray;
        array<float, 44100> completeBuffer;
        void increaseLineLength();
        array<string, 100> splitOSCMessageAddress(string s);
        int stoi(string s);

        array<Visual *, 5> visArray;

        ofShader shader;
        vector<ofPoint> formPoints;
        bool background, acceptIncomingOSC;

        float r = 50;
        float theta = 0;
        int num = 25;
        int buffUpdateCount = 0;
        static const int randomthing = ofGetWidth();
        static const int numOfBuffers = 10; //TODO find a way to initialize the audioArray with length as this variable
        array<array<float, Keys::BUFFER_SIZE>, numOfBuffers> audioArray;

        float lineStart, lineEnd, lineStartTheta, lineEndTheta, lineStartBase, lineEndBase, lineSpeed, lineMult;
};
