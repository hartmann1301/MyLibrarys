#include <MillisTimer.h>

MillisTimer timerTask;

void setup() {
  Serial.begin(9600);
}

void task() {
  Serial.print(millis());
  Serial.print(" ms: start Task");

  // wait for something for an unkown time
  delay(random(1, 999));
}

void loop() {
  // ask if timerTask1 expires
  if (timerTask.getTime() == 0) {
    // reset Timer, with the next timeout
    timerTask.resetTo(1000);

    // do Task
    task();
  } 
}
