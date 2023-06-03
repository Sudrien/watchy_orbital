#ifndef WATCHY_ORBITAL_H
#define WATCHY_ORBITAL_H

#include <Watchy.h>
#include <Fonts/FreeMono9pt7b.h>
#include <sunset.h>

#define DEG2RAD 0.0174532925

class WatchyOrbital : public Watchy{
    using Watchy::Watchy;
    public:
        SunSet sun;
         //these should be in settings
        float last_lat = 42.279594;
        float last_lon = -83.732124;

        WatchyOrbital(const watchySettings& s) : Watchy(s)  {}
        void drawWatchFace();
        void drawWatchTime();
        void drawBattery();
        void drawMoon();
        void drawNightTime();
    private:
        void fillArc2(float, float, unsigned int, unsigned int, unsigned int, float);
        unsigned int DaysPerMonth(unsigned int, unsigned int);
  };
#endif