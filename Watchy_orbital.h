#ifndef WATCHY_ORBITAL_H
#define WATCHY_ORBITAL_H

#include <Watchy.h>

class WatchyOrbital : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        void drawWatchTime();
    private:
        void fillArc2(float, float, unsigned int, unsigned int, unsigned int);
};

#endif