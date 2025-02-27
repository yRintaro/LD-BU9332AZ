/*
simpleと同じカウントアップをしながらそれぞれ用意されているアイコンを明滅します。
repeatと[1]は個人的に需要が無かったので実装していませんが必要なら簡単に実装できると思います。

*/
#include <ld_bu9332.h>
void setup() {
  uint32_t coms[] = {PA1, PA2, PA3};
  uint32_t segs[] = {PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PA4, PA5, PA6, PA7};
  initLCD(coms,segs);
}
uint32_t lastDisp = 0;
uint16_t sec = 0;
void loop() {
  ledPolling();
  if (millis() - lastDisp > 1000) {
    lastDisp = millis();
    showNumber(sec);
    battery(sec %3);
    playMark(sec %2);
    colon((sec+1) %2);
    sec++;
    if (sec == 10000) {
      sec = 0;
    }
  }
}
