/*
   OledGraph
*/
#define RASTER_MASK     0b00001111
#define GRAPH_BAR       0
#define GRAPH_LINE      1
#define GRAPH_DOTS      2

#define DRAW_OVERFLOW   0b10000000
#define DRAW_RASTER     0b01000000
#define TOGGLE_FLAG     0b00100000

#include "OledGraph.h"

OledGraph::OledGraph(byte x, byte y, byte width, byte height, int maxValue, byte mode)
  : m_x(x), m_y(y), m_width(width), m_height(height)
{
  // set default maxValue
  m_maxValue = maxValue;

  // set shared count and mode byte
  m_modeAndCount = 0;
  m_modeAndCount |= mode;

  // just for trying
  //m_modeAndCount |= DRAW_OVERFLOW;
  //m_modeAndCount |= DRAW_RASTER;
}

void OledGraph::drawFrame() {
  display.drawRect(m_x - 1, m_y - 1, m_width + 2, m_height + 2, WHITE);
}

void OledGraph::drawScale() {
  byte basePos = m_x + m_width + 1;
  display.drawLine(basePos, m_y, basePos, m_y - 1 + m_height, WHITE);
  display.drawPixel(basePos + 1, m_y, WHITE);
  display.drawPixel(basePos + 1, m_y + m_height - 1, WHITE);
}

void OledGraph::drawValue() {

  byte printStartX = m_x + m_width + 5;
  byte printStartY = m_y + m_height - 7;

  byte digits;
  if (m_maxValue < 10) {
    digits = 1;
  } else if (m_maxValue < 100) {
    digits = 2;
  } else if (m_maxValue < 1000) {
    digits = 3;
  } else if (m_maxValue < 10000) {
    digits = 5;
  } else {
    digits = 5;
  }

  display.fillRect(printStartX, printStartY, digits*5 + digits, 8, BLACK);
  display.setCursor(printStartX, printStartY);
  display.print(m_trueValue);
}

void OledGraph::addBar(int value) {

  this->add(value, GRAPH_BAR);
}

void OledGraph::addLine(int value) {

  this->add(value, GRAPH_LINE);
}

void OledGraph::addDots(int value) {

  this->add(value, GRAPH_DOTS);
}

void OledGraph::add(int value, const byte style)
{
  // helping values
  byte newValuePos = m_x + m_width - 1;
  byte bottomLine = m_y + m_height - 1;

  //Serial.println("add: " + String(value));

  // save before true value
  m_trueValue = value;

  // check for overflows
  if (value > m_maxValue) {
    if (m_modeAndCount & DRAW_OVERFLOW) {
      display.drawPixel(newValuePos, m_y, WHITE);
      display.drawPixel(newValuePos, bottomLine, WHITE);
      value = value % (m_maxValue + 1);
    } else {
      value = m_maxValue;
    }
  }

  // check for raster
  if (m_modeAndCount & DRAW_RASTER) {
    // count
    Serial.println("cnt: " + String(m_modeAndCount & RASTER_MASK));
    if ((m_modeAndCount & RASTER_MASK) < RASTER_MASK - 1 ) {
      m_modeAndCount++;
    } else {
      m_modeAndCount &= 0xff ^ RASTER_MASK;
    }
    // actual raster drawing
    for (byte i = 0; i < m_height; i++) {
      if (i % RASTER_MASK == 0) {
        if ((m_modeAndCount & RASTER_MASK) % RASTER_MASK == 0) {
          display.drawPixel(newValuePos, bottomLine - i , INVERSE);
        }
      }
    }
  }

  // paint the dualstate, ignore dots, dots make no sense
  if (m_maxValue == 1) {

    if (value != m_lastValue && style == GRAPH_LINE) {
      // draw flag once a value changed
      display.drawLine(newValuePos, bottomLine - 1, newValuePos, m_y + 1, INVERSE);

    } else {
      if (value == 0) {
        display.drawPixel(newValuePos, bottomLine, WHITE);

      } else {
        if (style == GRAPH_BAR) {
          display.drawLine(newValuePos, bottomLine, newValuePos, m_y, INVERSE);
        } else {
          display.drawPixel(newValuePos, m_y, INVERSE);
        }
      }
    }
  } else {
    // calculate new value
    byte mappedValue = map(value, 0, m_maxValue, 0, m_height - 1);
    //Serial.println("mappedValue: " + String(mappedValue) + "bottomLine: " + String(bottomLine));

    // paint the value
    switch (style) {
      case GRAPH_BAR:
        display.drawLine(newValuePos, bottomLine, newValuePos, bottomLine - mappedValue, INVERSE);
        break;
      case GRAPH_LINE:
        display.drawPixel(newValuePos, bottomLine - mappedValue , INVERSE);
        break;
      case GRAPH_DOTS:
        // print only every second dot
        if (m_modeAndCount&TOGGLE_FLAG)
          display.drawPixel(newValuePos, bottomLine - mappedValue, INVERSE);

        if (m_modeAndCount&TOGGLE_FLAG) {
          // set toggleFlag to 0, AND with all 1s expect the toggle_flag
          m_modeAndCount =  m_modeAndCount&(~TOGGLE_FLAG);
        } else {
          // set toggleFlag to 1
          m_modeAndCount = m_modeAndCount|TOGGLE_FLAG;          
        }
        
        break;

      default:
        break;
    }
  }

  m_lastValue = value;

  drawValue();
}

void OledGraph::tickRect(const byte n_x, const byte n_y, const byte n_width, const byte n_height) {
	// save all the important values 
	byte tmp_x = m_x;
	byte tmp_y = m_y;
	byte tmp_width = m_width;
	byte tmp_height = m_height;
	
	// set new values for the size
	m_x = n_x;
	m_y = n_y;
	m_width = n_width;
	m_height = n_height;	
	
	// tick with new values
	tick();
	
	// restore values
	m_x = tmp_x;
	m_y = tmp_y;
	m_width = tmp_width;
	m_height = tmp_height;	
}

void OledGraph::tick() {
  /* is simple but very slow... :(
  // go from graph start to the newest position
  for (byte _x = m_x + 1; _x < m_x + m_width; _x++) {

    // copy line one pixel left
    for (byte _y = m_y; _y < m_y + m_height; _y++) {

      // shifts pixels one left
      display.drawPixel(_x - 1, _y, display.getPixel(_x, _y));
    }
  }
  */
    byte newValuePos = m_x + m_width - 1;

  // go from graph start to the newest position
  for (byte j = m_x + 1; j < newValuePos + 1; j++) {

    byte hByte = (m_y + m_height) / 8;
    if ((m_y + m_height) % 8 != 0)
      hByte++;

    //Serial.println("hByte is: " + String(hByte));

    // copy line one pixel left
    for (byte i = m_y / 8; i < hByte; i++) {
      byte newByte;
      if (i == m_y / 8 && m_y % 8 != 0) {

        // save upper part of the current byte
        byte maskByte = 0x00;
        for (byte k = 0; k < m_y % 8; k++) {
          maskByte = maskByte << 1;
          maskByte |= 1;
        }
        //Serial.println("safe upper part, mask: " + String(maskByte));

        newByte = maskByte & display.readBuffer(j - 1 + SSD1306_LCDWIDTH * i);
        maskByte = ~maskByte;
        newByte |= maskByte & display.readBuffer(j + SSD1306_LCDWIDTH * i);

      } else if (i == hByte - 1 && (m_y + m_height) % 8 != 0) {

        // save lower part of the current byte
        byte maskByte = 0xff;
        for (byte k = 0; k < (m_y + m_height) % 8; k++) {
          maskByte = maskByte << 1;
        }
        //Serial.println("safe lower part, mask: " + String(maskByte));

        newByte = maskByte & display.readBuffer(j - 1 + SSD1306_LCDWIDTH * i);
        maskByte = ~maskByte;
        newByte |= maskByte & display.readBuffer(j + SSD1306_LCDWIDTH * i);

      } else {
        //Serial.println("safe full part");
        // get the byte
        newByte = display.readBuffer(j + SSD1306_LCDWIDTH * i);
      }

      // read byte from Buffer and shift one Pixel left
      display.writeBuffer(j - 1 + SSD1306_LCDWIDTH * i, newByte);
    }
  }
  
  // clear one line for the next values
  //byte newValuePos = m_x + m_width - 1;
  display.drawLine(newValuePos, m_y, newValuePos, m_y + m_height, BLACK);
}
