/*
 * Simple Counter/Timer class, uses Millis() Timer
 */
#include <MillisTimer.h>
#include <Arduino.h>

#ifdef USE_SAVEPOWER
#include <Narcoleptic.h>
#endif	

/*
 * Constructor initiate the attributes with 0, no need to get Millis because the Timer is not set yet
 */
MillisTimer::MillisTimer()
  : m_resetTime(0), m_cntValue(0)
{
}


unsigned long MillisTimer::getMillis() {
#ifdef USE_SAVEPOWER
  return Narcoleptic.millis() + millis();
#else
  return millis();
#endif	
}

/*
 * this Method sets/resets the Timer, when callings this function it expires after >resetValue< milliseconds
 */
void MillisTimer::resetTo(int resetValue) {	
  // save the current time when the times starts
  m_resetTime =  getMillis(); 
  
  // stores values when it should expire
  m_cntValue = resetValue;
}

void MillisTimer::raiseTo(int resetValue) {

  if (m_cntValue < resetValue)
    m_cntValue = resetValue;
  
  resetTo(resetValue);
}

/*
 * this Method musst be called to check if the Timer has expired, if it is check too late it causes a delay
 */
int MillisTimer::getTime() {
  // check if time passed since last call
  countDown();

  // returns 0 if Timer is expired
  return m_cntValue;
}

void MillisTimer::countDown() {
  // return if timer is set
  if (m_cntValue == 0)
    return;
  
  // get current time
  unsigned long timeNow = getMillis();

  // decrement the count value
  m_cntValue -= (timeNow - m_resetTime);

  // save the current time
  m_resetTime = timeNow;

  // correct overflow
  if (m_cntValue < 0)
    m_cntValue = 0;
}
