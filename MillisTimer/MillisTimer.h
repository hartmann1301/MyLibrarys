/*
 * Simple Counter class
 */

class MillisTimer {

  public:
    MillisTimer();
    int getTime();
    void resetTo(int resetValue);

  private:
    void countDown();
  
    unsigned long m_resetTime;
    int m_cntValue;
};
