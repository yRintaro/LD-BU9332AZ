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
    sec++;
    if (sec == 10000) {
      sec = 0;
    }
  }
}