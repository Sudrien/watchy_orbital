#include "watchy_orbital.h"

void WatchyOrbital::drawWatchFace() {
  display.fillScreen(GxEPD_WHITE);
  sun.setPosition(settings.lat.toFloat(), settings.lon.toFloat(), settings.gmtOffset / 3600);
  display.setTextColor(GxEPD_BLACK);
  drawMoon();
  drawWatchTime();
  drawBattery();
  drawNightTime();
}


void WatchyOrbital::drawWatchTime() {
  drawWatchMinute();
  drawWatchHour();
  drawWatchDay();
  drawWatchMonth();

  display.setFont(&FreeSansBold9pt7b);


  display.setCursor(0, 12);
  display.println(dayShortStr(currentTime.Wday));

//  weatherData currentWeather = getWeatherData();
//  display.setCursor(0, 24);
//  display.println(String(currentWeather.sunrise.Hour) + ':' + String(currentWeather.sunrise.Minute) + ':' + String(currentWeather.sunrise.Second));

  display.setCursor(174, 182);
  char buffer[3];
  sprintf(buffer, "%+03d", settings.gmtOffset / 3600);
  display.println(buffer);

  display.setCursor(160, 198);
  display.println(tmYearToCalendar(currentTime.Year));
  }


void WatchyOrbital::drawWatchMinute() {
  float minuteAngle = 360.0 * (currentTime.Minute / 60.0);
  int tick = 360 / 60;

  // 60 minutes to 1, no empty
  if (currentTime.Minute == 0) {
    minuteAngle = 360.0;
  }

  fillArc3(0.0, minuteAngle, 86, 13, GxEPD_BLACK, 1);
  if (minuteAngle == 360.0) {
    fillArc2(tick, minuteAngle, 86, 13, GxEPD_WHITE, tick);
  } else {
    fillArc2(tick, minuteAngle - tick, 86, 13, GxEPD_WHITE, tick);
    fillArc2(minuteAngle + tick, 360 - tick, 83, 7, GxEPD_BLACK, tick);
  }
}


void WatchyOrbital::drawWatchHour() {
  float hourAngle = 360.0 * (currentTime.Hour / 24.0);
  int tick = 360 / 24;

  // 24 hours to 1, no empty
  if (currentTime.Hour == 0) {
    hourAngle = 360.0;
    }

  // Draw arcs: black arc, white seperator, black seperators
  fillArc3(0.0, hourAngle, 68, 15, GxEPD_BLACK, 1);
  if (hourAngle == 360.0) {
    fillArc2(tick, hourAngle, 68, 15, GxEPD_WHITE, tick);
    }
  else {
    fillArc2(tick, hourAngle - tick, 68, 15, GxEPD_WHITE, tick);
    fillArc2(hourAngle + tick, 360 - tick, 64, 8, GxEPD_BLACK, tick);
    }
  }


void WatchyOrbital::drawWatchDay() {
  float dayAngle = 360.0 * (currentTime.Day / 31.0);
  int tick = 360 / DaysPerMonth(currentTime.Year, currentTime.Month);

  // Draw arcs: black arc, white seperator, black seperators
  fillArc3(0.0, dayAngle, 40, 17, GxEPD_BLACK, 2);
  if (dayAngle == 360.0) {
    fillArc2(tick, dayAngle, 40, 17, GxEPD_WHITE, tick);
    }
  else {
    fillArc2(tick, dayAngle - tick, 40, 17, GxEPD_WHITE, tick);
    fillArc2(dayAngle + tick, 360 - tick, 39, 9, GxEPD_BLACK, tick);
    }
  }


void WatchyOrbital::drawWatchMonth() {
  float monthAngle = 360.0 * (currentTime.Month / 12.0);
  int tick = 360 / 12;

  // Draw arcs: black arc, white seperator, black seperators
  fillArc3(0.0, monthAngle, 18, 19, GxEPD_BLACK, 2);
  if (monthAngle == 360.0) {
    fillArc2(tick, monthAngle, 18, 19, GxEPD_WHITE, tick);
    }
  else {
    fillArc2(tick, monthAngle - tick, 18, 19, GxEPD_WHITE, tick);
    fillArc2(monthAngle + tick, 360 - tick, 17, 10, GxEPD_BLACK, tick);
    }
  }


void WatchyOrbital::drawBattery() {
  display.setCursor(158, 12);
  display.setFont(&FreeSansBold9pt7b);
  display.println(analogReadMilliVolts(BATT_ADC_PIN) * 2);
  display.setCursor(170, 28);
  display.println("mV");
  }


void WatchyOrbital::drawMoon() {
  long long unix_time = (long long)makeTime(currentTime);
  sun.setCurrentDate(tmYearToCalendar(currentTime.Year), currentTime.Month, currentTime.Day);
  sun.setTZOffset(settings.gmtOffset / 3600);
  int moonphase = sun.moonPhase(unix_time);

  int radius = 16;
  int moonCenterX = 17;
  int moonCenterY = 183;

  display.fillCircle(moonCenterX, moonCenterY, radius, GxEPD_BLACK);

  // 0 new, 29 & 30 new, 15 full
  if (moonphase > 0 && moonphase <= 15) {
    fillEllipse(moonCenterX - ((((moonphase - 1) / 28.0) - 0.5) * radius * 2), moonCenterY, (15 / 14) * (moonphase - 1) + 1, radius, GxEPD_WHITE);
    }
  // 0 new, 29 & 30 new, 15 full
  if (moonphase > 15 && moonphase < 29) {
    fillEllipse(moonCenterX - ((((moonphase - 1) / 28.0) - 0.5) * radius * 2), moonCenterY, (-15 / 13) * (moonphase - 15) + 16, radius, GxEPD_WHITE);
    }
  display.drawCircle(moonCenterX, moonCenterY, radius, GxEPD_BLACK);
  }


void WatchyOrbital::drawNightTime() {
  sun.setCurrentDate(tmYearToCalendar(currentTime.Year), currentTime.Month, currentTime.Day);
  sun.setTZOffset(settings.gmtOffset / 3600);

  //minutes past midnight. Apparently.
  float sunrise = sun.calcSunrise() / 1440 * 360 + 360;
  float sunset = sun.calcSunset() / 1440 * 360;

  fillArc3(sunset, sunrise, 55, 2, GxEPD_BLACK, 1);
  }


void WatchyOrbital::fillArc2(float start_angle, float end_angle, unsigned int radius, unsigned int width, unsigned int colour, float step) {
  unsigned int center_x = 100;
  unsigned int center_y = 100;

  float w = width / 2;

  // Draw colour blocks every inc degrees
  for (float i = start_angle; i <= end_angle; i += step) {

    float cx1 = center_x + (radius - width / 2) * cos((i - 90) * DEG2RAD);
    float cy1 = center_y + (radius - width / 2) * sin((i - 90) * DEG2RAD);
    float cx2 = center_x + (radius + width / 2) * cos((i - 90) * DEG2RAD);
    float cy2 = center_y + (radius + width / 2) * sin((i - 90) * DEG2RAD);

    display.drawLine(cx1, cy1, cx2, cy2, colour);
    }
  }


//the triangles version
void WatchyOrbital::fillArc3(float start_angle, float end_angle, unsigned int radius, unsigned int width, unsigned int colour, float step) {
  unsigned int center_x = 100;
  unsigned int center_y = 100;

  float w = width / 2;

  // Draw colour blocks every inc degrees
  for (float i = start_angle; i <= end_angle - step; i += step) {
    float cx1 = center_x + (radius - width / 2) * cos((i - 90) * DEG2RAD);
    float cy1 = center_y + (radius - width / 2) * sin((i - 90) * DEG2RAD);
    float cx2 = center_x + (radius + width / 2) * cos((i - 90) * DEG2RAD);
    float cy2 = center_y + (radius + width / 2) * sin((i - 90) * DEG2RAD);
    float cx3 = center_x + (radius - width / 2) * cos((i - 90 + step) * DEG2RAD);
    float cy3 = center_y + (radius - width / 2) * sin((i - 90 + step) * DEG2RAD);
    float cx4 = center_x + (radius + width / 2) * cos((i - 90 + step) * DEG2RAD);
    float cy4 = center_y + (radius + width / 2) * sin((i - 90 + step) * DEG2RAD);

    display.fillTriangle(cx1, cy1, cx2, cy2, cx3, cy3, colour);
    display.fillTriangle(cx2, cy2, cx3, cy3, cx4, cy4, colour);
    }
  }

unsigned int WatchyOrbital::DaysPerMonth(unsigned int year, unsigned int month) {
  if (month == 2) {
    if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)) {
      return 28;
      }
    else {
      return 29;
      }
    }
  else if (month == 4 || month == 6 || month == 9 || month == 11) {
    return 30;
    }
  else {
    return 31;
    }
  }


// https://github.com/adafruit/Adafruit-GFX-Library/pull/217 code by Soyunr - contributed under BSD license
void WatchyOrbital::drawEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color) {
  int16_t Rx2 = rx * rx;
  int16_t Ry2 = ry * ry;
  int16_t twoRx2 = 2 * Rx2;
  int16_t twoRy2 = 2 * Ry2;
  int16_t p;
  int16_t x = 0;
  int16_t y = ry;
  int16_t px = 0;
  int16_t py = twoRx2 * y;
  display.startWrite();
  display.writePixel(x0 + x, y0 + y, color);
  display.writePixel(x0 - x, y0 + y, color);
  display.writePixel(x0 + x, y0 - y, color);
  display.writePixel(x0 - x, y0 - y, color);
  //region1
  p = (int16_t)(Ry2 - Rx2 * ry + 0.25 * Rx2);
  while (px < py) {
    x++;
    px += twoRy2;
    if (p < 0)
      p += Ry2 + px;
    else {
      y--;
      py -= twoRx2;
      p += Ry2 + px - py;
    }
    display.writePixel(x0 + x, y0 + y, color);
    display.writePixel(x0 - x, y0 + y, color);
    display.writePixel(x0 + x, y0 - y, color);
    display.writePixel(x0 - x, y0 - y, color);
  }
  //region2
  p = (int16_t)(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
  while (y > 0) {
    y--;
    py -= twoRx2;
    if (p > 0)
      p += Rx2 - py;
    else {
      x++;
      px += twoRy2;
      p += Rx2 - py + px;
    }
    display.writePixel(x0 + x, y0 + y, color);
    display.writePixel(x0 - x, y0 + y, color);
    display.writePixel(x0 + x, y0 - y, color);
    display.writePixel(x0 - x, y0 - y, color);
  }
  display.endWrite();
}

// gpt-4 conversion
void WatchyOrbital::fillEllipse(int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color) {
  int16_t Rx2 = rx * rx;
  int16_t Ry2 = ry * ry;
  int16_t twoRx2 = 2 * Rx2;
  int16_t twoRy2 = 2 * Ry2;
  int16_t p;
  int16_t x = 0;
  int16_t y = ry;
  int16_t px = 0;
  int16_t py = twoRx2 * y;

  display.startWrite();

  // Draw initial vertical line at center
  display.writeFastVLine(x0, y0 - y, 2 * y, color);

  // Region 1
  p = (int16_t)(Ry2 - Rx2 * ry + 0.25 * Rx2);
  while (px < py) {
    x++;
    px += twoRy2;
    if (p < 0)
      p += Ry2 + px;
    else {
      y--;
      py -= twoRx2;
      p += Ry2 + px - py;
    }
    // Draw vertical lines instead of individual pixels
    display.writeFastVLine(x0 + x, y0 - y, 2 * y, color);
    display.writeFastVLine(x0 - x, y0 - y, 2 * y, color);
  }

  // Region 2
  p = (int16_t)(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
  while (y > 0) {
    y--;
    py -= twoRx2;
    if (p > 0)
      p += Rx2 - py;
    else {
      x++;
      px += twoRy2;
      p += Rx2 - py + px;
    }
    // Draw vertical lines instead of individual pixels
    display.writeFastVLine(x0 + x, y0 - y, 2 * y, color);
    display.writeFastVLine(x0 - x, y0 - y, 2 * y, color);
  }

  display.endWrite();
}
