/*
0..9に続き、AbcdEF を表示するサンプル
*/
#include <ld_bu9332.h>
void setup() {
  uint32_t coms[] = {PA1, PA2, PA3};
  uint32_t segs[] = {PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PA4, PA5, PA6, PA7};
  initLCD(coms,segs);
  digs[0]=0;
  digs[1]=0;
  digs[2]=0;
  digs[3]=0;
}
uint32_t lastDisp = 0;
uint16_t sec = 0;
void loop() {
  ledPolling();
  if (millis() - lastDisp > 1000) {
    lastDisp = millis();
    byte sg[]={0,0,0,0};
    sg[3]=number[sec];
    setSegments(sg);
    sec++;
    if (sec == 16) {      
      sec = 0;
    }
  }
}