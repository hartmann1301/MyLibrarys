/*
   OledClock
*/

#ifndef OledClock_h
#define OledClock_h

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

#define POINTER_FAKTOR_SHORT 0.5
#define POINTER_FAKTOR_LONG 0.75

enum {LONG = 0, SHORT};

class clockPointer
{
  public:
    clockPointer(const bool l, const float f);
    float getRadiant(const int8_t steps);

    // vector data
    int8_t pos;
    int8_t x;
    int8_t y;

    // display area data
    int8_t xMin, xMax, yMin, yMax;

    const bool len;
    const float lenFactor;
};

class OledClock
{
  public:
    OledClock(const int8_t centerX, const int8_t centerY, const int8_t radius);

    void drawClock();
    void tickPointer();
    void updateClock();

  private:
    void drawPointer(const clockPointer &p, int8_t const color);
    void calcVectors(clockPointer &p);
    void increment(clockPointer &p);

    const int8_t mCenterX;
    const int8_t mCenterY;
    const int8_t mRadius;
    const int8_t mPositiosInClock;

    clockPointer pointerShort = clockPointer(SHORT, POINTER_FAKTOR_SHORT);
    clockPointer pointerLong = clockPointer(LONG, POINTER_FAKTOR_LONG);
};

#endif

