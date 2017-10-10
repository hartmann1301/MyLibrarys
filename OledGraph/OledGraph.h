/*
 * OledGraph
 */
 
#ifndef OledGraph_h
#define OledGraph_h

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

class OledGraph
{
  public:
    OledGraph(byte x, byte y, byte width, byte height, int maxValue, byte mode = 0);

    void 
      addBar(int value),
      addLine(int value),   
      addDots(int value);
      
    void tick();
	void tickRect(const byte n_x, const byte n_y, const byte n_width, const byte n_height);
    void setMode(byte mode) {m_modeAndCount = mode;};

    void drawFrame();
    void drawScale();
    void drawValue();
    
    void setMaxValue(byte maxValue) { m_maxValue = maxValue; };
    byte maxValue() { return m_maxValue; };
    
  private:
    void add(int value, const byte type);
	
	Adafruit_SSD1306* m_disp;
	
    byte m_x;
    byte m_y;
    byte m_width;
    byte m_height;

    int m_lastValue;
    int m_maxValue;
    int m_trueValue;
    
    byte m_modeAndCount;
};

#endif

