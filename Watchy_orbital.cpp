#include "Watchy_orbital.h"

void WatchyOrbital::drawWatchFace() {
  display.fillScreen(GxEPD_WHITE);
  sun.setPosition(last_lat, last_lon, settings.gmtOffset / 3600);
  display.setTextColor(GxEPD_BLACK);
  drawMoon();
  drawWatchTime();
  drawBattery();
  drawNightTime();

  //display.setFont(&FreeSansBold9pt7b);
  //display.setCursor(10, 10);
  //display.println(esp_get_free_heap_size());
}


void WatchyOrbital::drawWatchTime() {
  drawWatchMinute();
  drawWatchHour();
  drawWatchDay();
  drawWatchMonth();

  display.setFont(&FreeSansBold9pt7b);
  display.setCursor(174, 175);

  char buffer[3];
  sprintf(buffer, "%+03d", settings.gmtOffset / 3600);

  display.println(buffer);
  display.setCursor(160, 191);
  display.println(tmYearToCalendar(currentTime.Year));

  //drawStopwatch();
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
  } else {
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
  } else {
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
  } else {
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

  /*

  
    float VBAT = getBatteryVoltage();
    if(VBAT > 4.1){
      display.fillTriangle(165, 15, 200, -10, 180, -10, GxEPD_BLACK);
      }
    if(VBAT > 3.95){
      display.fillTriangle(175, 25, 210, 0, 190, 0, GxEPD_BLACK);
      }
    if(VBAT > 3.80){
      display.fillTriangle(185, 35, 220, 10, 200, 10, GxEPD_BLACK);
      }
 */
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
    //display.fillCircle(moonCenterX - ((((moonphase - 1) / 28.0) - 0.5) * radius * 2), moonCenterY, radius, GxEPD_WHITE);
    fillEllipse(moonCenterX - ((((moonphase - 1) / 28.0) - 0.5) * radius * 2), moonCenterY, (15 / 14) * (moonphase - 1) + 1, radius, GxEPD_WHITE);
  }
  // 0 new, 29 & 30 new, 15 full
  if (moonphase > 15 && moonphase < 29) {
    //display.fillCircle(moonCenterX - ((((moonphase - 1) / 28.0) - 0.5) * radius * 2), moonCenterY, radius, GxEPD_WHITE);
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

    //display.drawLine(cx1, cy1, cx2, cy2, colour);
    display.fillTriangle(cx1, cy1, cx2, cy2, cx3, cy3, colour);
    display.fillTriangle(cx2, cy2, cx3, cy3, cx4, cy4, colour);
  }
}

unsigned int WatchyOrbital::DaysPerMonth(unsigned int year, unsigned int month) {
  if (month == 2) {
    if (year % 4 != 0 || (year % 100 == 0 && year % 400 != 0)) {
      return 28;
    } else {
      return 29;
    }
  } else if (month == 4 || month == 6 || month == 9 || month == 11) {
    return 30;
  } else {
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

/*

// this does not seem to be being called
void WatchyOrbital::handleButtonPress() {

    display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);

  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
  // Menu Button
  if (wakeupBit & MENU_BTN_MASK) {
    if (guiState == WATCHFACE_STATE) {  // enter menu state if coming from watch face
      showMenu(menuIndex, false);
    } else if (guiState == MAIN_MENU_STATE) {  // if already in menu, then select menu item
      switch (menuIndex) {
        case 0:
          showAbout();
          break;
        case 1:
          showBuzz();
          break;
        case 2:
          showAccelerometer();
          break;
        case 3:
          setTime();
          break;
        case 4:
          setupWifi();
          break;
        case 5:
          showUpdateFW();
          break;
        case 6:
          showSyncNTP();
          break;
        default:
          break;
      }
    } else if (guiState == FW_UPDATE_STATE) {
      updateFWBegin();
    }
  }
  // Back Button
  else if (wakeupBit & BACK_BTN_MASK) {
    if (guiState == MAIN_MENU_STATE) {  // exit to watch face if already in menu
      RTC.read(currentTime);
      showWatchFace(false);
    } else if (guiState == APP_STATE) {
      showMenu(menuIndex, false);  // exit to menu if already in app
    } else if (guiState == FW_UPDATE_STATE) {
      showMenu(menuIndex, false);  // exit to menu if already in app
    } else if (guiState == WATCHFACE_STATE) {

      toggleStopwatch();
      return;
    }
  }
  // Up Button
  else if (wakeupBit & UP_BTN_MASK) {
    if (guiState == MAIN_MENU_STATE) {  // increment menu index
      menuIndex--;
      if (menuIndex < 0) {
        menuIndex = MENU_LENGTH - 1;
      }
      showMenu(menuIndex, true);
    } else if (guiState == WATCHFACE_STATE) {
      return;
    }
  }
  // Down Button
  else if (wakeupBit & DOWN_BTN_MASK) {
    if (guiState == MAIN_MENU_STATE) {  // decrement menu index
      menuIndex++;
      if (menuIndex > MENU_LENGTH - 1) {
        menuIndex = 0;
      }
      showMenu(menuIndex, true);
    } else if (guiState == WATCHFACE_STATE) {
      return;
    }
  }


  bool timeout = false;
  long lastTimeout = millis();
  pinMode(MENU_BTN_PIN, INPUT);
  pinMode(BACK_BTN_PIN, INPUT);
  pinMode(UP_BTN_PIN, INPUT);
  pinMode(DOWN_BTN_PIN, INPUT);
  while (!timeout) {
    if (millis() - lastTimeout > 5000) {
      timeout = true;
    } else {
      if (digitalRead(MENU_BTN_PIN) == 1) {
        lastTimeout = millis();
        if (guiState == MAIN_MENU_STATE) {  // if already in menu, then select menu item
          switch (menuIndex) {
            case 0:
              showAbout();
              break;
            case 1:
              showBuzz();
              break;
            case 2:
              showAccelerometer();
              break;
            case 3:
              setTime();
              break;
            case 4:
              setupWifi();
              break;
            case 5:
              showUpdateFW();
              break;
            case 6:
              showSyncNTP();
              break;
            default:
              break;
          }
        } else if (guiState == FW_UPDATE_STATE) {
          updateFWBegin();
        }
      } else if (digitalRead(BACK_BTN_PIN) == 1) {
        lastTimeout = millis();
        if (guiState == MAIN_MENU_STATE) {  // exit to watch face if already in menu
          RTC.read(currentTime);
          showWatchFace(false);
          break;  // leave loop
        } else if (guiState == APP_STATE) {
          showMenu(menuIndex, false);  // exit to menu if already in app
        } else if (guiState == FW_UPDATE_STATE) {
          showMenu(menuIndex, false);  // exit to menu if already in app
        }
      } else if (digitalRead(UP_BTN_PIN) == 1) {
        lastTimeout = millis();
        if (guiState == MAIN_MENU_STATE) {  // increment menu index
          menuIndex--;
          if (menuIndex < 0) {
            menuIndex = MENU_LENGTH - 1;
          }
          showFastMenu(menuIndex);
        }
      } else if (digitalRead(DOWN_BTN_PIN) == 1) {
        lastTimeout = millis();
        if (guiState == MAIN_MENU_STATE) {  // decrement menu index
          menuIndex++;
          if (menuIndex > MENU_LENGTH - 1) {
            menuIndex = 0;
          }
          showFastMenu(menuIndex);
        }
      }
    }
  }
}


void WatchyOrbital::drawStopwatch() {
  display.setFont(&FreeSansBold9pt7b);

  if (stopwatch_running) {
display.setCursor(0, 12);
display.println((long long)makeTime(currentTime));
    long long stopwatch_now = (long long)makeTime(currentTime);
    long long stopwatch_diff = stopwatch_now = stopwatch_start;

    int s = stopwatch_diff % 60;
    int m = ((stopwatch_diff - s) / 60) % 60;
    int h = (stopwatch_diff - s - m * 60) / 3600;


    display.setCursor(0, 12);
    char buffer[8];
    sprintf(buffer, "%02d:%02d:%02d", h, m, s);

    display.println(buffer);
    display.setCursor(0, 24);
    display.println("Run!"); 
  } else if (stopwatch_end > 0) {
display.setCursor(0, 12);
display.println(stopwatch_end);

    long long stopwatch_diff = stopwatch_end = stopwatch_start;

    int s = stopwatch_diff % 60;
    int m = ((stopwatch_diff - s) / 60) % 60;
    int h = (stopwatch_diff - s - m * 60) / 3600;


    display.setCursor(0, 12);
    char buffer[8];
    sprintf(buffer, "%02d:%02d:%02d", h, m, s);

    display.println(buffer);
    display.setCursor(0, 24);
    display.println("Stop"); 
  } else {
    display.setCursor(12, 12);
    display.println("W");

  }
}

void WatchyOrbital::toggleStopwatch() {
    display.setFullWindow();
  display.fillScreen(GxEPD_BLACK);
  if (stopwatch_running) {
    stopwatch_end = (long long)makeTime(currentTime);
    stopwatch_running = false;
    display.setCursor(0, 12);
    display.println(stopwatch_end);
    }
  else {
    stopwatch_start = (long long)makeTime(currentTime);
    stopwatch_running = true;
    display.setCursor(12, 12);
    display.println(stopwatch_start);
    }
  drawWatchFace();
}



void WatchyOrbital::init(String datetime) {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause(); // get wake up reason
  Wire.begin(SDA, SCL);                         // init i2c
  RTC.init();

  // Init the display here for all cases, if unused, it will do nothing
  display.epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0)); // Set SPI to 20Mhz (default is 4Mhz)
  display.init(0, displayFullInit, 10,
               true); // 10ms by spec, and fast pulldown reset
  display.epd2.setBusyCallback(displayBusyCallback);

  switch (wakeup_reason) {
  case ESP_SLEEP_WAKEUP_EXT0: // RTC Alarm
    RTC.read(currentTime);
    switch (guiState) {
    case WATCHFACE_STATE:
      showWatchFace(true); // partial updates on tick
      if (settings.vibrateOClock) {
        if (currentTime.Minute == 0) {
          // The RTC wakes us up once per minute
          vibMotor(75, 4);
        }
      }
      break;
    case MAIN_MENU_STATE:
      // Return to watchface if in menu for more than one tick
      if (alreadyInMenu) {
        guiState = WATCHFACE_STATE;
        showWatchFace(false);
      } else {
        alreadyInMenu = true;
      }
      break;
    }
    break;
  case ESP_SLEEP_WAKEUP_EXT1: // button Press
    handleButtonPress();
    break;
  default: // reset
    RTC.config(datetime);
    _bmaConfig();
    gmtOffset = settings.gmtOffset;
    RTC.read(currentTime);
    RTC.read(bootTime);
    showWatchFace(false); // full update on reset
    vibMotor(75, 4);
    break;
  }
  deepSleep();
}

*/