#define GPSBaud 9600

void gps_Init() {

  yield();

  Serial.println("s,gps init|");

  display_Print("Initializing GPS.");
  
  // this pin is naturally pullup on the wemos d1, so we must switch it to be floating
  pinMode(GPS_RX_PIN, INPUT);

  // we communicate with the gps module using typical serial communications. 
  // we want to keep the built in serial open for debugging and possibly external interfacing later so we're using software serial for this.
  ss.begin(GPSBaud);
  
  Serial.println("s,gps init complete|");
}

void gps_Scan_Update() {

  if (input_readExitState() == true) {

    selectButtonPressed = false;
    lastMenuIndex = -1;
    appState = 1;
    Serial.println("s,gps scanning complete|");
    return;
  }

  oled.clear(PAGE);
   
  if (scanFrame == 0) {
    
    oled.drawBitmap(BMP_SCAN_FRAME_1);
    
  } else if (scanFrame == 1) {
    
    oled.drawBitmap(BMP_SCAN_FRAME_2);
    
  } else if (scanFrame == 2) {
    
    oled.drawBitmap(BMP_SCAN_FRAME_3);
    
  } else if (scanFrame == 3) {
    
    oled.drawBitmap(BMP_SCAN_FRAME_4);
  }
  
  if (locked) {
    
    oled.setCursor(1, 1);
    oled.print("GPS");
  }
  
  oled.display();
  
  scanFrame ++;
  if (scanFrame > 3)
    scanFrame = 0;
  
  run_gps_Scan(false);
}

void gps_WIFI_Scan_Update() {

  if (input_readExitState() == true) {

    selectButtonPressed = false;
    lastMenuIndex = -1;
    appState = 1;
    return;
  }
  
  oled.clear(PAGE);
   
  if (scanFrame == 0) {
    
    oled.drawBitmap(BMP_SCAN_FRAME_1);
    
  } else if (scanFrame == 1) {
    
    oled.drawBitmap(BMP_SCAN_FRAME_2);
    
  } else if (scanFrame == 2) {
    
    oled.drawBitmap(BMP_SCAN_FRAME_3);
    
  } else if (scanFrame == 3) {
    
    oled.drawBitmap(BMP_SCAN_FRAME_4);
  }

  if (locked) {
    
    oled.setCursor(1, 1);
    oled.print("GPS");
  }

  if (accessPoints > 0) {

    oled.setCursor(1, 35);
    oled.print(accessPoints);
  }
  
  oled.display();
  
  scanFrame ++;
  if (scanFrame > 3)
    scanFrame = 0;
  
  run_gps_Scan(true);
}

void run_gps_Scan(bool wifiScan) {

  // on first run, need to fire sd_WriteLine("START");
  // wait for a certain amount of time to elapse while polling the gps device.

  start = millis();
  
  do 
  {
    while (ss.available())
      gps.encode(ss.read());

    yield();
    
  } while (millis() - start < (2 * 1000)); // 10 * 1000 or every 10 seconds

  // is the current location valid?
  if (!gps.location.isValid()) {

    locked = false;
    return;
  }

  locked = true;

  // clear out buffers
  memset(outputBuffer, 0, sizeof outputBuffer);
  memset(lat, 0, sizeof lat);
  memset(lng, 0, sizeof lng);
  memset(dat, 0, sizeof dat);
  memset(tim, 0, sizeof tim);

  // convert floats to character arrays, then copy to buffers
  dtostrf(gps.location.lat(), 9, 5, lat);
  dtostrf(gps.location.lng(), 9, 5, lng);
  sprintf(dat, "%02d-%02d-%02d", gps.date.month(), gps.date.day(), gps.date.year());
  sprintf(tim, "%02d-%02d-%02d", gps.time.hour(), gps.time.minute(), gps.time.second());

  // concatenate all character arrays into final entry for log
  sprintf(outputBuffer, "g,%s,%s,%s,%s|", lat, lng, dat, tim);
  Serial.println(outputBuffer);

  if (!disableSDCard) {

    // save data to card
    File dataFile = 
      SD
      .open(
        "gpslog.txt", 
        FILE_WRITE);
     
    if (dataFile) {
  
      dataFile.print(outputBuffer);
      dataFile.close(); 
    } else {

      disableSDCard = true;
      Serial.println("s,sd card write error|");
    }
  }

  display_Reset();

  if (wifiScan == true) {

    Serial.println("s,wifi scanning|");
    wifi_ScanRouters();
    Serial.println("s,wifi scanning complete|");
  }
}
