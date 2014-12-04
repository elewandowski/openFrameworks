#include "ofMain.h"
#include <array>

class Keys {

    public:

        static const int LENGTH = 128;
        static const int BUFFER_SIZE = 255;
        array<int, LENGTH> keys;
        array<int, LENGTH> keysWithPedal;
        array<int, LENGTH> keysWithPedalAndDecay;
        bool PEDAL_STATE = false;
        int LAST_PLAYED_KEY = 0;
        int BASS_NOTE = 0;
        int PRESSED_KEYS = 0;
        string NAME;

        array<float, BUFFER_SIZE> audioL;
        array<float, BUFFER_SIZE> audioR;
        int bufferWriteHead, bufferReadHead;

        Keys();
        Keys(string name);
        int getVel(int pitch);
        int getVelWithDecay(int pitch);
        void setVel(int pitch, int velocity);
        void setPedal(bool state);
        int numOfPressedKeys();
        void setAudio(float audioInL, float audioInR);
        float getAudioMono();
        float getBuffer(int i);
        void setAudioBufferIndex(int index, float value);
        array<float, 2> getAudioStereo();

    private:
        int pedal(int currentVelocity, int newVelocity);
        int reduceVel(int vel);
        void silenceKeys();
        int findBassNote(int currentNote);
};
