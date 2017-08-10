/*
 * Simple Counter class
 */
class MillisTimer {

  public:
    MillisTimer();
    int getTime();
    void resetTo(int resetValue);

  private:
	// is called in getTime
    void countDown();
	
	// saves the last time this function was called
    unsigned long m_resetTime;
	
	// saves the millisecond when the timer expires next
    int m_cntValue;
};
