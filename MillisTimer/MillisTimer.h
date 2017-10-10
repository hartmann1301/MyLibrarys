/*
 * Simple Counter class
 */
class MillisTimer {

  public:
    MillisTimer();
    int getTime();
    void resetTo(int resetValue);
    void raiseTo(int resetValue);
	
  private:
	// is called in getTime
    void countDown();
	
	// own millis function for narcoleptic support
	unsigned long getMillis();
	
	// saves the last time this function was called
    unsigned long m_resetTime;
	
	// saves the milliseconds when the timer expires next, int: max=2^15ms ~ 32,7s 
    int m_cntValue;
};
