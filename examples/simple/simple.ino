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
    sec++;
    if (sec == 10000) {
      sec = 0;
    }
  }
}
