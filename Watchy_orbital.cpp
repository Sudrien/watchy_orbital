#include "Watchy_orbital.h"

#define DEG2RAD 0.0174532925

void WatchyOrbital::drawWatchFace() {
  display.fillScreen(GxEPD_WHITE);

  display.setTextColor(GxEPD_BLACK);
  drawMoon();
  drawWatchTime();
  drawBattery();
}


void WatchyOrbital::drawWatchTime(){
  int16_t centerX = display.width() / 2;
  int16_t centerY = display.height() / 2;

  // Calculate arc end points
  float minuteAngle = 360.0 * (currentTime.Minute / 60.0);
  float hourAngle = 360.0 * (currentTime.Hour / 24.0);
  float dayAngle = 360.0 * (currentTime.Day / 31.0);
  float monthAngle = 360.0 * (currentTime.Month / 12.0);

  // Draw arcs
  fillArc2(0.0, monthAngle, 26, 19, GxEPD_BLACK, 0.0611);
  fillArc2(0.0, monthAngle, 26, 19, GxEPD_WHITE, 360 / 12);

  fillArc2(0.0, dayAngle, 48, 17, GxEPD_BLACK, 0.0611);
  fillArc2(0.0, dayAngle, 48, 17, GxEPD_WHITE, 360 / DaysPerMonth(currentTime.Year, currentTime.Month) );

  fillArc2(0.0, hourAngle, 68, 15, GxEPD_BLACK, 0.0611);
  fillArc2(0.0, hourAngle, 68, 15, GxEPD_WHITE, 360 / 24);

  fillArc2(0.0, minuteAngle, 86, 13, GxEPD_BLACK, 0.0611);
  fillArc2(0.0, minuteAngle, 86, 13, GxEPD_WHITE, 360 / 60);

  display.setCursor(154, 185);
  display.setFont(&FreeMono9pt7b);
  display.println(tmYearToCalendar(currentTime.Year));
  }

void WatchyOrbital::drawBattery(){
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
 
  }

void WatchyOrbital::drawMoon(){

  long long unix_time = (long long)makeTime(currentTime);  
  long long known_new_moon = 1684515180; // may 19 2023
  
  long long lunation = 2551707; // 29.5306 days

  float phase = ((unix_time - known_new_moon) % 2551707) / 2551707.0;

  //display.setCursor(0, 185);
  //display.setFont(&FreeMono9pt7b);
  //display.println(phase);

  if(phase < 0.03) {
    //new
    display.fillCircle(17, 183, 16, GxEPD_BLACK);
    }
  else if(phase < 0.5) {
    // waxing
    display.fillCircle(17, 183, 16, GxEPD_BLACK);
    display.fillRect(17, 167, 16, 32, GxEPD_WHITE);
    }
  else if(phase < 0.75) {
    // full
    }
  else {
    //waning
    display.fillCircle(17, 183, 16, GxEPD_BLACK);
    display.fillRect(1, 167, 16, 32, GxEPD_WHITE);
    }
  display.drawCircle(17, 183, 16, GxEPD_BLACK);
  }




void WatchyOrbital::fillArc2(float start_angle, float end_angle, unsigned int radius, unsigned int width, unsigned int colour, float step){
  unsigned int center_x = 100;
  unsigned int center_y = 100;

  float w = width / 2;

  // Draw colour blocks every inc degrees
  for (float i = start_angle; i <= end_angle; i += step) {

    float cx1 = center_x + (radius - width / 2) * cos((i - 90 ) * DEG2RAD);
    float cy1 = center_y + (radius - width / 2) * sin((i - 90 ) * DEG2RAD);
    float cx2 = center_x + (radius + width / 2) * cos((i - 90 ) * DEG2RAD);
    float cy2 = center_y + (radius + width / 2) * sin((i - 90 ) * DEG2RAD);

    display.drawLine(cx1, cy1, cx2, cy2, colour);

    }
  }

unsigned int WatchyOrbital::DaysPerMonth(unsigned int year, unsigned int month){
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