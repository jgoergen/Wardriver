bool clientConnectedToAP = false;

void wifi_Init() {

  Serial.println("s,wifi init|");
  WiFi.persistent(false);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP_STA);
  
  delay(100);
  Serial.println("s,wifi init complete|");
}

void wifi_ScanRouters() {

  int networksFound = WiFi.scanNetworks();
  wifi_ShowScanResult(networksFound);
}

void wifi_ShowScanResult(int numberOfNetworks) {

  accessPoints = numberOfNetworks;
  
  for (int i = 0; i < numberOfNetworks; i++) {
        
    memset(outputBuffer, 0, sizeof outputBuffer);
    memset(encryptionType, 0, sizeof encryptionType);
    memset(rssi, 0, sizeof rssi);

    dtostrf(WiFi.encryptionType(i), 3, 0, encryptionType);
    dtostrf(WiFi.RSSI(i), 5, 0, rssi);
    
    sprintf(
      outputBuffer, 
      "w,%s,%s,%s|", 
      WiFi.SSID(i).c_str(), 
      encryptionType, 
      rssi);

    // save data to card
    if (!disableSDCard) {
  
      File dataFile = 
        SD
        .open(
          "gpslog.txt", 
          FILE_WRITE);

      if (dataFile) {
        
        dataFile.print(outputBuffer);
        dataFile.close();
      } else {

        Serial.println("s,sd card write error|");
        disableSDCard = true;
      }
    }
      
    Serial.println(outputBuffer);
  }

  if (!disableSDCard) {

    File dataFile = 
        SD
        .open(
          "gpslog.txt", 
          FILE_WRITE);

    if (dataFile) {
      
      dataFile.print(outputBuffer);
      dataFile.close();        
      
    } else {
  
      Serial.println("s,sd card write error|");
      disableSDCard = true;
    }
  }
}
