#ifndef CORE_H
#define CORE_H
#include "Visual.h"

class Core : public Visual
{
    public:
        Core();
        Core(ofVec3f center, float size);
        void draw();
        void update();
        virtual ~Core();
    protected:
    private:
};

#endif // CORE_H
