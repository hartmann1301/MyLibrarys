/*
   clockPointer
*/

#include "OledClock.h"

clockPointer::clockPointer(const bool l, const float f) 
  : len(l), lenFactor(f)
{
  pos = -1;
  x = 0;
  y = 0;  
}

float clockPointer::getRadiant(const int8_t steps) 
{
  return ((PI * 2) / steps) * (steps - pos);
}

/*
   OledClock
*/

OledClock::OledClock(const int8_t centerX, const int8_t centerY, const int8_t radius)
  : mCenterX(centerX), mCenterY(centerY), mRadius(radius), mPositiosInClock(radius*4) 
{
}

void OledClock::drawClock() 
{
  // draw circle and center
  display.drawCircle(mCenterX, mCenterY, mRadius, WHITE);
  display.drawPixel(mCenterX, mCenterY, WHITE);

  // draw a Hint for the 12 clock Numbers
  for (int i = 0; i < 12; i++) {
    float currentRad = ((2 * PI) / 12) * i;
    int8_t xPos = sin(currentRad) * (mRadius - mRadius/10);
    int8_t yPos = cos(currentRad) * (mRadius - mRadius/10);

	int8_t xEnd = sin(currentRad) * (mRadius - 1);
    int8_t yEnd = cos(currentRad) * (mRadius - 1);

    display.drawLine(mCenterX + xPos, mCenterY + yPos, mCenterX + xEnd, mCenterY + yEnd, WHITE);	
    //display.drawPixel(mCenterX + xPos, mCenterY + yPos, WHITE);
  }
}

void OledClock::updateClock() 
{
  // compare pointers
  byte xMin = min(pointerShort.xMin, pointerLong.xMin);
  byte xMax = max(pointerShort.xMax, pointerLong.xMax);
  byte yMin = min(pointerShort.yMin, pointerLong.yMin);
  byte yMax = max(pointerShort.yMax, pointerLong.yMax);
  
  // calc area width and height
  byte xStart = xMin + mCenterX;
  byte yStart = yMin + mCenterY;
  byte xWidth = xMax - xMin + 1;
  byte yHeight = yMax - yMin + 1;  
  
  display.updateRect(xStart, yStart, xWidth, yHeight);   
}

void OledClock::tickPointer() 
{
  // init
  if (pointerShort.pos == -1 || pointerLong.pos == -1) {
	// because positive y coordinates starts at third quadrant...
    pointerShort.pos = mPositiosInClock/2; 
    pointerLong.pos = mPositiosInClock/2;
	
	calcVectors(pointerShort); 
	calcVectors(pointerLong); 
	
    // short pointer is behind long pointer, no need for printing both
    drawPointer(pointerLong, WHITE);	
    return;
  }

  // delete old long pointer because it always changes
  drawPointer(pointerLong, BLACK);
  
  // inc and calc new long pointer
  increment(pointerLong);  
  calcVectors(pointerLong);  

  // increment short pointer and delete it
  if (pointerLong.pos == mPositiosInClock/2) {
    // delete only if it changes
    drawPointer(pointerShort, BLACK);	
	
  // calc new short pointer
	increment(pointerShort);  
	calcVectors(pointerShort);
  }
  
  drawPointer(pointerShort, WHITE);	
  drawPointer(pointerLong, WHITE);
}

void OledClock::increment(clockPointer &p) 
{
  if (p.pos == mPositiosInClock - 1) {
    p.pos = 0;
  } else {
    p.pos++;
  }
}

void OledClock::calcVectors(clockPointer &p) 
{
  // save current coordinates
  p.xMin = p.x;
  p.xMax = p.x;
  p.yMin = p.y;
  p.yMax = p.y;
	
  // calc new vectors
  p.x = sin(p.getRadiant(mPositiosInClock))*mRadius*p.lenFactor;
  p.y = cos(p.getRadiant(mPositiosInClock))*mRadius*p.lenFactor;
  
  // calc min and max
  p.xMin = min(min(p.x, p.xMin), 0);
  p.xMax = max(max(p.x, p.xMax), 0);
  p.yMin = min(min(p.y, p.yMin), 0);
  p.yMax = max(max(p.y, p.yMax), 0);
}

void OledClock::drawPointer(const clockPointer &p, const int8_t color) 
{
  display.drawLine(mCenterX, mCenterY, mCenterX + p.x, mCenterY + p.y, color);
}
  /*	
  // update full clock area
  display.updateRect(mCenterX - mRadius, mCenterY - mRadius, mRadius*2 + 1, mRadius*2 + 1);
  
  // update only pointers area
  float drawRadius = mRadius*POINTER_FAKTOR_LONG;
  display.updateRect(mCenterX - drawRadius, mCenterY - drawRadius, drawRadius*2 + 1, drawRadius*2 + 1);  	 
  */

  /*	
  Serial.print("long:" );
  Serial.print(pointerLong.pos, DEC);
  Serial.print(" (");
  Serial.print(pointerLong.x , DEC);
  Serial.print(",");
  Serial.print(pointerLong.y, DEC);
  Serial.print(") ");
 
  Serial.print("\t short:" );
  Serial.print(pointerShort.pos, DEC);
  Serial.print(" (");
  Serial.print(pointerShort.x , DEC);
  Serial.print(",");
  Serial.print(pointerShort.y, DEC);
  Serial.print(") ");
 
  Serial.print("\t area x:" );
  Serial.print(xStart, DEC);
  Serial.print("-");
  Serial.print(xWidth, DEC);
  Serial.print(" y:");
  Serial.print(yStart , DEC);
  Serial.print("-");
  Serial.print(yHeight, DEC);
  
  Serial.print(" F: "); 
  Serial.print(xWidth*yHeight, DEC);

  Serial.println();  
  */
