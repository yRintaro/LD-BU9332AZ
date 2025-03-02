/*
simpleと同じカウントアップをしながらそれぞれ用意されているアイコンを明滅します。
repeatと[1]は個人的に需要が無かったので実装していませんが必要なら簡単に実装できると思います。

*/
#include <ld_bu9332.h>
ld_bu9332 lcd=ld_bu9332();

void setup() {
  uint32_t coms[] = {PA1, PA2, PA3};
  uint32_t segs[] = {PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PA4, PA5, PA6, PA7};
  lcd.initLCD(coms,segs);
}
uint32_t lastDisp = 0;
uint16_t sec = 0;
void loop() {
  lcd.polling();
  if (millis() - lastDisp > 1000) {
    lastDisp = millis();
    lcd.showNumber(sec);
    lcd.battery(sec %3);
    lcd.playMark(sec %2);
    lcd.colon((sec+1) %2);
    sec++;
    if (sec == 10000) {
      sec = 0;
    }
  }
}
