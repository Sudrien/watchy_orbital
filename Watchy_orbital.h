#ifndef WATCHY_ORBITAL_H
#define WATCHY_ORBITAL_H

#include <Watchy.h>
#include <Fonts/FreeMono9pt7b.h>

class WatchyOrbital : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        void drawWatchTime();
        void drawBattery();
        void drawMoon();
    private:
        void fillArc2(float, float, unsigned int, unsigned int, unsigned int, float);
        unsigned int DaysPerMonth(unsigned int, unsigned int);
  };
#endif