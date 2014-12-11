#include "ofApp.h"
#include <string>
//--------------------------------------------------------------
void ofApp::setup(){
	// listen on the given port

    //cout << receiver.messages.max_size(); << endl;
    background == false;
    acceptIncomingOSC = true;
	ofSetBackgroundAuto(background);
	ofBackground(127);
	ofEnableAlphaBlending();
	receiver.setup(PORT);

    //for(int i=0; i<imageArray.size();i++) imageArray[i] =

	editControlPoint = 0;
    for(int i=0; i<cp.size(); i++) cp[i] = 1.0;

	instArray[0] = Keys("debussy");
	instArray[1] = Keys("pipe");

	current_msg_string = 0;

    lineStartBase = ofGetWindowWidth() / 3;
    lineEndBase = ofGetWindowWidth() / 3 * 2;
    lineMult = ofGetWindowWidth() / 3;
    //lineSpeed = 0.00025;
    lineSpeed = 0.01;
}

//--------------------------------------------------------------
void ofApp::update(){

    ofSetBackgroundAuto(background);
	// hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        string msg_string;

		array<string, 100> messageAddress = splitOSCMessageAddress(m.getAddress());
		int instNum = stoi(messageAddress[0]);

        if(instNum != -1) {
            for (int i=0; i<instArray.size(); i++) {
                if (i == instNum) {
                    if(messageAddress[1] == "keys"){
                        int pitch = m.getArgAsInt32(0);
                        int velocity = m.getArgAsInt32(1);
                        if (velocity > 0) increaseLineLength();
                        instArray[i].setVel(pitch, velocity);
                    }
                    else if (messageAddress[1] == "audio") {
                        instArray[i].setAudio(m.getArgAsFloat(0), m.getArgAsFloat(0));
                    }
                    else if (messageAddress[1] == "audioBuffer") {
                        //cout << "num of args is " << m.getNumArgs() << endl;
                        if(acceptIncomingOSC) for(int j=0; j<Keys::BUFFER_SIZE; j++) instArray[i].setAudioBufferIndex(j, m.getArgAsFloat(j));
                    }
                    else if (messageAddress[1] == "pedal"){
                        bool pedal = m.getArgAsInt32(0) > 0 ? true : false;
                        instArray[i].setPedal(pedal);
                    }
                }
            }
        }
        else if(messageAddress[0] == "cc") {
            ccArray[m.getArgAsInt32(0)] = m.getArgAsInt32(1);
            cout << "cc#: " << m.getArgAsInt32(0) << " value:" << m.getArgAsInt32(1) << endl;
            cout << (ofGetWindowHeight() / Keys::LENGTH) * ccArray[1] << "The fuakjn is";
        }

		{
			// unrecognized message: display on the bottom of the screen

			//msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
			current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
		}
	}
    lineStart = lineStartBase + (sin(lineStartTheta) * lineMult);
    lineEnd = lineEndBase + (sin(lineEndTheta) * lineMult);
}


//--------------------------------------------------------------
void ofApp::draw(){


//    ofSetColor(ofColor(200));
//    ofFill();
//    ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

	string buf;
	buf = "listening for osc messages on port " + ofToString(PORT);
	ofDrawBitmapString(buf, 10, 20);

//	for(int i = 0; i < NUM_MSG_STRINGS; i++){
//		//ofDrawBitmapString(msg_strings[i], 10, 40 + 15 * i);
//	}


    float verticalSpacing = ofGetWindowHeight() / Keys::LENGTH;



    for (int pitch=0; pitch<Keys::LENGTH; pitch++) {
        for (int i=0; i<instArray.size(); i++) {
            int velocity = instArray[i].getVelWithDecay(pitch);
            float audio = instArray[i].getAudioMono();

            if(i == 0) {
                if (velocity > 0) {
                    ofColor color;
                    color.setHsb(pitch, 127, velocity, 50);
                    ofSetColor(color);
                    ofFill();
                    glLineWidth(1 + (velocity*instArray[3].getAudioMono()));
                    //float velTriggeredAngle = (velocity / 127) * 50;
                    ofLine(lineStart * cp[0], pitch * verticalSpacing * cp[1], lineEnd + cp[2], pitch * verticalSpacing + cp[3]);

                    ofNoFill();
                    glLineWidth(1);
                    ofSetColor(0);
                    ofLine(lineStart * cp[0], pitch * verticalSpacing * cp[1], lineEnd + cp[2], pitch * verticalSpacing + cp[3]);
                }
            }
            else if(i == 1) {
                if (pitch == 0){
                   instArray[i].numOfPressedKeys();
                   //cout <<  instArray[i].PRESSED_KEYS << endl;


                    //ofBeginShape();
                }

                    ofColor color;
                    color.setHsb(pitch + 30, 127, 100, 50);
                    ofSetColor(color);
                    ofNoFill();
                    glLineWidth(4*cp[2]);
                    //for(int vertex=0; vertex<instArray[i].PRESSED_KEYS; vertex++) {
                if(velocity > 0) {
//                    float mag = pitch / Keys::LENGTH;
//                    ofVec2f v1(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()));
//                    ofVec2f v2(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()));
//                    ofVec2f v3 = v1.getInterpolated(v2, mag);
//                    ofVec2f v4 = v2.getInterpolated(v1, mag);
//                    ofLine(v3.x, v3.y, v4.x, v4.y);
//                    cout << "x-axis:" << pitch << endl;
//                    cout << "y-axis:" << pitch << endl;
                    ofCircle(ofGetWindowWidth()/4*cp[2], ofGetWindowHeight()/4, pitch * 4 + velocity * cp[2]);
                }
            }
            else if(i == 2) {

                if(velocity > 0) {
                    ofColor color;
                    float pitch2Col = ofMap(pitch, 35, 44, 0, 360);

                    if(pitch == 36) { //main kick
                        color.setHsb(0, 100, 52, 50);
                        ofSetColor(color);
                        ofFill();
                        glLineWidth(40);
                        ofCircle(ofGetWindowWidth()/2*cp[6], ofGetWindowHeight()/2*cp[7], 200);
                    }
                    else if (pitch == 37){ //side kick
                        color.setHsb(pitch2Col, 127, 100, 50);
                        ofSetColor(color);
                        ofFill();
                        glLineWidth(20);
                        int pitch2Radius = ofMap(pitch, 35, 44, 50, 200);
                        ofCircle(ofGetWindowWidth()/2*cp[5], ofGetWindowHeight()/2, pitch2Radius*cp[7]);
                    }
                    else if (pitch == 41){ //clap
                        color.setHsb(pitch2Col*cp[0], 127, 100, 50);
                        ofSetColor(color);
                        ofFill();
                        glLineWidth(20);
                        int pitch2Radius = ofMap(200, 35, 44, 50, 200);
                        ofCircle(ofGetWindowWidth()/4*3*cp[4], ofGetWindowHeight()/2, 300);
                    }
                    else if (pitch == 44){ //hihat
                        color.setHsb(255*cp[2], 127, 100, 50);
                        ofSetColor(color);
                        ofFill();
                        glLineWidth(20);
                        ofCircle(ofGetWindowWidth()/4*3*cp[0], ofGetWindowHeight()/3*cp[1], 100);
                    }
                }
            }
            else if(i == 3) {
                //cout << "audio is: " << audio << endl;
                ofColor color;
                color.setHsb(audio*100, 127, 100, 50);
                ofSetColor(color);
                ofFill();

                //glLineWidth(20);
                //ofCircle(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 250*audio*cp[4]);
                ofNoFill();
                ofSetColor(0);
                ofCircle(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 250*audio*cp[4]);
            }
        }
    }

    float verticalSpacingAudio = ofGetWindowHeight() / (float) Keys::BUFFER_SIZE;
    float horizontalSpacingAudio = ofGetWindowWidth() / (float) Keys::BUFFER_SIZE;
    glLineWidth(mouseY);
    float audioVisMul = (float) mouseX;

    for (int bufferPos=0; bufferPos<Keys::BUFFER_SIZE; bufferPos++) {
        //ofFill();

        //ofSetColor(255);
        float audio = instArray[3].getBuffer(bufferPos);
        //if (ofGetFrameNum() % 60 == 0) cout << "audio is: " << audio << endl;
        //if(audio > 1 || audio < -1) cout << "audio is: " << audio << endl;
//        if (audio > 0) ofSetColor(0);
//        else ofSetColor(255);
        float audioColor = (audio + 1) * 0.5;


        ofSetColor(audioColor * 255);
        //ofLine(0, verticalSpacingAudio * bufferPos, ofGetWindowWidth()/2, verticalSpacingAudio * bufferPos);
        ofLine(horizontalSpacingAudio * bufferPos,
                ofGetWindowHeight()/2 + (audio * audioVisMul),
                horizontalSpacingAudio * (bufferPos+1),
                ofGetWindowHeight()/2 + (audio * audioVisMul));
        //cout << "audio is: " << audio << endl;
//        cout << "buff size  is: " << Keys::BUFFER_SIZE << endl;
//        cout << "VERTICAL SPACEING: " << verticalSpacingAudio << endl;
    }
}

void ofApp::increaseLineLength(){
    lineStartTheta -= lineSpeed;
	lineEndTheta += lineSpeed;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'o') acceptIncomingOSC = !acceptIncomingOSC;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    cp[editControlPoint] = (float)x / ofGetWindowWidth() * 4;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //for my mouse: left = 0, right = 2
    if (button == 0) editControlPoint = (editControlPoint + 1) % cp.size();
    else if(button == 2) background = !background;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    lineStartBase = ofGetWindowWidth() / 3;
    lineEndBase = ofGetWindowWidth() / 3 * 2;
    lineMult = ofGetWindowWidth() / 3;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

array<string, 100> ofApp::splitOSCMessageAddress(string s) {
     array<string, 100> returnArray;

    int pos, i = 0;
    std::string token;
    std::string delimiter = "/";
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        returnArray[i] = token;
        s.erase(0, pos + delimiter.length());
        i++;
    }
    returnArray[i] = s; //

    return returnArray;
}

int ofApp::stoi(string s) {
    int num;
    if ( ! (istringstream(s) >> num) ) num = -1;
    return num;
}
