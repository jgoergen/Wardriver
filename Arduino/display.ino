// oled lib https://github.com/sparkfun/SparkFun_Micro_OLED_Arduino_Library/blob/master/src/SFE_MicroOLED.cpp

void display_Init() {

  Serial.println("s,oled init|");
  oled.begin();
  oled.clear(ALL);  // Clear the display's memory (gets rid of artifacts)
  oled.setFontType(0);  // Set font to type 0
  oled.display();
  oled.flipVertical(true);
  oled.flipHorizontal(true);
  delay(100);
  oled.clear(PAGE); // Clear the buffer.
  Serial.println("s,oled init complete|");
}

void display_Reset() {

  oled.clear(PAGE);
  oled.setCursor(0, 0);
}

void display_Print(char data[]) {

  oled.print(data);
}

void display_DrawProgressBar(int y, int percentage) {

  oled.line(2, y, 60, y);
  oled.line(2, y + 4, 60, y + 4);
  oled.line(2, y, 2, y + 4);
  oled.line(60, y, 60, y + 4);

  int barWidth = int(float(60) * (float(percentage) / 100.0f));
  
  oled.line(3, y + 1, 3 + barWidth, y + 1);
  oled.line(3, y + 2, 3 + barWidth, y + 2);
  oled.line(3, y + 3, 3 + barWidth, y + 3);

  oled.display();
}

// ACTUAL SCREEN CODE /////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void display_UpdateMenuDisplay() {

  // if nothing has changed, don't update display
  if (menuIndex == lastMenuIndex)
    return;
    
  switch(menuIndex) {

    case 0: // gps data logger

      oled.clear(PAGE);
      oled.drawBitmap(BMP_GPS);
      break;

    case 1: // gps + WIFI data logger

      oled.clear(PAGE);
      oled.drawBitmap(BMP_GPSWIFI);
      break;
  }

  oled.display();
  lastMenuIndex = menuIndex;
}

void display_UpdateLoadingDisplay(char data[], int percentage) {

  // draw text
  oled.clear(PAGE);
  oled.setCursor(2, 2);
  oled.print(data);

  // draw progress bar
  display_DrawProgressBar(10, percentage); 
}

// END ACTUAL SCREEN CODE /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

