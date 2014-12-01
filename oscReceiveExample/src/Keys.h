#include "ofMain.h"
#include <array>

class Keys {

    public:
        static const int LENGTH = 128;
        array<int, LENGTH> keys;
        array<int, LENGTH> keysWithPedal;
        array<int, LENGTH> keysWithPedalAndDecay;
        bool PEDAL_STATE = false;
        int LAST_PLAYED_KEY = 0;
        int BASS_NOTE = 0;
        int PRESSED_KEYS = 0;
        string NAME;

        Keys();
        Keys(string name);
        int getVel(int pitch);
        int getVelWithDecay(int pitch);
        void setVel(int pitch, int velocity);
        void setPedal(bool state);
        int numOfPressedKeys();

    private:
        int pedal(int currentVelocity, int newVelocity);
        int reduceVel(int vel);
        void silenceKeys();
        int findBassNote(int currentNote);
};
