void menu_FinalizeMenuEntry() {

  selectButtonPressed = false;
  
  if (menuIndex == 0) {

    accessPoints = 0;
    locked = false;
    appState = 2;
    Serial.println("s,gps scanning|");

  } else if (menuIndex == 1) {

    WiFi.mode(WIFI_STA);
    accessPoints = 0;
    locked = false;
    appState = 3;
    Serial.println("s,gps wifi scanning|");
    
  }
}

void menu_FinalizeDataEntry() {
  
}

void menu_Update() {

  input_update();
  display_UpdateMenuDisplay();
}
