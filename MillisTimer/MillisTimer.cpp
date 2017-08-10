/*
 * Simple Counter class
 */
#include <MillisTimer.h>
#include <Arduino.h>

MillisTimer::MillisTimer()
  : m_resetTime(0), m_cntValue(0)
{
}

void MillisTimer::resetTo(int resetValue) {
  m_cntValue = resetValue;
  m_resetTime =  millis(); 
}

int MillisTimer::getTime() {

  countDown();

  return m_cntValue;
}

void MillisTimer::countDown() {
  if (m_cntValue == 0)
    return;
  
  // get current time
  unsigned long timeNow = millis();

  m_cntValue -= (timeNow - m_resetTime);
   
  m_resetTime = timeNow;

  if (m_cntValue < 0)
    m_cntValue = 0;
}
