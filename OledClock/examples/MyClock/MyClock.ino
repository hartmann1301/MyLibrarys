#include <OledClock.h>
#include <MillisTimer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void initDisplay() {
  // set Pins for GND and 5v
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(F("MyClock!"));
  display.display();
}

OledClock myClock(96, 32, 30);
MillisTimer timerTask;

void setup()
{
  initDisplay();

  myClock.drawClock();
  display.display();
}

void loop()
{
  if (timerTask.getTime() == 0) {
    // reset Timer, with the next timeout
    timerTask.resetTo(10);

    // do Task
    myClock.tickPointer();
    myClock.updateClock();
  }
}
