#include "Keys.h"

Keys::Keys() {
    for (int i=0; i<LENGTH; i++) {
      keys[i] = 0;
      keysWithPedal[i] = 0;
      keysWithPedalAndDecay[i] = 0;
    }
    bufferReadHead, bufferWriteHead = 0;
}

Keys::Keys(string _name) {
    NAME = _name;
    for (int i=0; i<LENGTH; i++) {
      keys[i] = 0;
      keysWithPedal[i] = 0;
      keysWithPedalAndDecay[i] = 0;
    }
    bufferReadHead, bufferWriteHead = 0;
}

int Keys::getVel(int pitch) {
    return keysWithPedal[pitch];
}

int Keys::getVelWithDecay(int pitch) {
    //in order to visually display decay, we have to reduce the value of velocity each time it's called
    return keysWithPedalAndDecay[pitch] = reduceVel(keysWithPedalAndDecay[pitch]);
}

void Keys::setVel(int pitch, int velocity) {
    keys[pitch] = velocity;
    keysWithPedalAndDecay[pitch] = keysWithPedal[pitch] = pedal(keysWithPedal[pitch], velocity);
    BASS_NOTE = findBassNote(pitch);
    LAST_PLAYED_KEY = pitch;
}

void Keys::setPedal(bool state) {
    if (state == false && PEDAL_STATE == true) silenceKeys();
    PEDAL_STATE = state;
}

int Keys::pedal(int currentVelocity, int newVelocity) {
    return PEDAL_STATE && newVelocity == 0 ? currentVelocity : newVelocity;
}

int Keys::reduceVel(int vel){
    if (ofGetFrameNum() % 20 == 0) {
      if (vel > 0) vel -= 1;
    }
    return vel;
}

void Keys::silenceKeys() {
    for (int i=0; i<keysWithPedal.size(); i++) {
      if (keys[i] == 0) {
        keysWithPedal[i] = keysWithPedalAndDecay[i] = 0;
      }
    }
}

int Keys::findBassNote(int currentNote){
    int returnBassNote = BASS_NOTE;

    if (keysWithPedal[currentNote] != 0 && (keysWithPedal[BASS_NOTE] == 0 || currentNote < BASS_NOTE)) {
        returnBassNote = currentNote;
      //println("the new bass note is: " + returnBassNote);
    }
    return returnBassNote;
}

int Keys::numOfPressedKeys() {
    PRESSED_KEYS = 0;
    for(int i=0; i<LENGTH; i++) {
        if (keysWithPedal[i] > 0) PRESSED_KEYS++;
    }
    return PRESSED_KEYS;
}

void Keys::setAudio(float audioInL, float audioInR) {
    //cout << "the audio in here is: " << audioIn << endl;
    audioL[bufferWriteHead] = audioInL;
    audioR[bufferWriteHead] = audioInR;
    bufferWriteHead = (bufferWriteHead + 1) % BUFFER_SIZE;
}

float Keys::getAudioMono() {
    //cout << "the audio out here is: " << audioL[0] << endl;
    float output = (audioL[(bufferWriteHead-1)%BUFFER_SIZE] + audioR[(bufferWriteHead-1)%BUFFER_SIZE]) * 0.5;
    //bufferReadHead = (bufferReadHead + 1) % BUFFER_SIZE;
    return output;
}

void Keys::setAudioBufferIndex(int index, float value) {
    audioL[index] = value;
    audioR[index] = value;
}

float Keys::getBuffer(int i) {
    //cout << "the audio out here is: " << audioL[0] << endl;
    float output = (audioL[i] + audioR[i]) * 0.5;
    //bufferReadHead = (bufferReadHead + 1) % BUFFER_SIZE;
    return output;
}

array<float, 2> Keys::getAudioStereo() {
    //cout << "the audio out here is: " << audioL[0] << endl;
    array<float, 2> output = {audioL[bufferReadHead], audioR[bufferReadHead]};
    bufferReadHead = (bufferReadHead + 1) % BUFFER_SIZE;
    return output;
}
