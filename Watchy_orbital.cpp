#include "Watchy_orbital.h"

#define DEG2RAD 0.0174532925

void WatchyOrbital::drawWatchFace() {
  display.fillScreen(GxEPD_WHITE);  // Fill the screen with white color
  drawWatchTime();
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
  fillArc2(0.0, minuteAngle, 86, 16, GxEPD_BLACK);
  fillArc2(0.0, hourAngle, 66, 16, GxEPD_BLACK);
  fillArc2(0.0, dayAngle, 46, 16, GxEPD_BLACK);
  fillArc2(0.0, monthAngle, 26, 16, GxEPD_BLACK);
  }


void WatchyOrbital::fillArc2(float start_angle, float end_angle, unsigned int radius, unsigned int width, unsigned int colour){
  unsigned int center_x = 100;
  unsigned int center_y = 100;

  float w = width / 2;

  float cx = center_x + radius * cos((start_angle - 90 + w) * DEG2RAD);
  float cy = center_y + radius * sin((start_angle - 90 + w) * DEG2RAD);

  display.fillCircle(cx, cy, w, colour);

  // Draw colour blocks every inc degrees
  for (int i = start_angle; i <= end_angle; i += 3) {

    cx = center_x + radius * cos((i - 90 + w) * DEG2RAD);
    cy = center_y + radius * sin((i - 90 + w) * DEG2RAD);

    display.fillCircle(cx, cy, w, colour);

  }
}