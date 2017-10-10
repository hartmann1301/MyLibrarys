#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OledGraph.h>

#define OLED_RESET 4
Adafruit_SSD1306 display = Adafruit_SSD1306(OLED_RESET);

OledGraph graph0 = OledGraph(0, 0, 64, 7, 2);
OledGraph graph1 = OledGraph(0, 9, 64, 7, 1);
OledGraph graph2 = OledGraph(0, 18, 64, 7, 3);
OledGraph graph3 = OledGraph(0, 27, 64, 64-27, 220);

void setup()   {
  Serial.begin(115200);
  Serial.print(F("Init:"));

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F(" Oled Test"));

  graph0.drawScale();
  graph1.drawScale();
  graph2.drawScale();
  graph3.drawScale();
  
  display.display();

  Serial.println(F("Start:"));
}

byte cnt = 0;
float floatCnt = 0;

void loop() {
  
  ++cnt;
  floatCnt = 0.01 * cnt;

  //Serial.println(sin(2*PI*floatCnt ));
  byte tmpVar = sin(4 * PI * floatCnt) * 100 + 128;

  long beforeTime = micros();

  graph0.addLine(tmpVar/100);
  graph1.addLine(tmpVar>30);
  graph2.addLine(tmpVar>60);
  graph3.addDots(cnt);
  graph3.addLine(tmpVar);

  graph0.tickRect(0, 0, 64, 64);
/*
  graph0.tick();
  graph1.tick();
  graph2.tick();  
  graph3.tick();
  */

  //display.display();
  display.updateRect(0, 0, 88, 64);

  long printTime = micros() - beforeTime;
  Serial.println(printTime);
}
