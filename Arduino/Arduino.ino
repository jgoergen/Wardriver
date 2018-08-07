#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <TinyGPS++.h>      // https://github.com/mikalhart/TinyGPSPlus
#include <SFE_MicroOLED.h>  // https://github.com/sparkfun/SparkFun_Micro_OLED_Arduino_Library
#include <Wire.h> 
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

// PIN DEFINITIONS /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#define SD_CHIP_SELECT_PIN  16  // Wimos pin D0
#define SD_SCK_PIN          14  // Wimos pin D5
#define SD_MISO_PIN         12  // Wimos pin D6
#define SD_MOSI_PIN         13  // Wimos pin D7
#define GPS_TX_PIN          255 // Not used
#define GPS_RX_PIN          0   // Wimos pin D3
#define LEFT_BUTTON_PIN     2   // Wimos pin D4
#define RIGHT_BUTTON_PIN    A0  // Wimos pin D8
#define SELECT_BUTTON_PIN   15  // Wimos pin A0
#define TFT_SCL_PIN         5   // Wimos pin D1
#define TFT_SDA_PIN         4   // Wimos pin D2

const char COMMAND_BEGIN_CHAR = '<';
const char COMMAND_END_CHAR = '>';
const char* COMMAND_DIVIDER_CHAR = ":";
const byte COMMAND_LENGTH = 32;

bool disableSDCard = false;
char receivedChars[COMMAND_LENGTH];
boolean newData = false;
char receivedNewData[COMMAND_LENGTH];
bool leftButtonPressed = false;
int lastLeftButtonVal = 1;
bool rightButtonPressed = false;
int lastRightButtonVal = 0;
bool selectButtonPressed = false;
int lastSelectButtonVal = 0;
int menuIndex = 0;
int lastMenuIndex = 1;
int dataEntryDelta = 0;
bool menuDataEntry = false;
unsigned long start = 0;
bool locked = false;
char outputBuffer[200]; 
char lat[12];
char lng[12];
char dat[12];
char tim[12];
char encryptionType[3];
char rssi[5];
bool error = false;
int appState = 0;
int bootPhase = 0;
int scanFrame = 0;
int accessPoints = 0;
int getDataPhase = 0;

TinyGPSPlus gps;
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);
MicroOLED oled(255, 0); 
ESP8266WebServer server(80);

void setup()
{
  // intentionally left empty. 
  // ESP8266 / ESP32 chips don't like delays ( they will reset if anything takes too long before a yield / delay / etc. ) 
  // so I'm using a boot sequence instead.
}

void loop()
{
  switch(appState) {

    case 0:
      boot_Update();
      break;
      
    case 1: 
      menu_Update();
      checkSerialData();
      break;

    case 2: 
      gps_Scan_Update();
      break;

    case 3: 
      gps_WIFI_Scan_Update();
      break;
  }
}

void processSerialCommand(char* command, char* param) {

  // commands should be in the format "<COMMAND:PARAMER>"

  if (strcmp(command, "menu") == 0) {
    
    menuIndex = atoi(param);
    Serial.println("s,ok|");
    menu_FinalizeMenuEntry();
    
  } else if (strcmp(command, "setsd") == 0) {

    if (param == "0") {

      disableSDCard = true;
    } else {

      disableSDCard = false;
    }
    
    Serial.println("s,ok|");
  }
}

void boot_Update() {

  switch(bootPhase) {

    case 0: 
      Serial.begin(115200);
      Serial.println("s,boot init|");
      bootPhase = 1;
      break;

     case 1:
      display_Init();
      display_UpdateLoadingDisplay("DISPLAY", 15);
      bootPhase = 2;
      break;

    case 2:
      display_UpdateLoadingDisplay("INPUTS", 39);
      input_Init();
      bootPhase = 3;
      break;
        
    case 3:
      display_UpdateLoadingDisplay("GPS", 45);
      gps_Init();
      bootPhase = 4;
      break;
      
    case 4:
      display_UpdateLoadingDisplay("SD CARD", 60);
      // init sd card
      Serial.println("s,sd init begin|");
      if (SD.begin(SD_CHIP_SELECT_PIN)){
         Serial.println("s,sd init complete|");
      } else {
        Serial.println("s,sd init error|");
      }
      bootPhase = 5;
      break;
      
    case 5:
      display_UpdateLoadingDisplay("WIFI", 75);
      wifi_Init();
      bootPhase = 6;
      break;

    case 6:
      display_UpdateLoadingDisplay("DONE!", 100);
      Serial.println("s,boot complete|");
      appState = 1;
      break;
  }
}

void checkSerialData() {
  
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char rc;
  
  while (Serial.available() > 0 && newData == false) {
    
    rc = Serial.read();

    if (recvInProgress == true) {
      
        if (rc != COMMAND_END_CHAR) {
          
            receivedChars[ndx] = rc;
            ndx++;
            
            if (ndx >= COMMAND_LENGTH)              
                ndx = COMMAND_LENGTH - 1;
        } else {
          
            receivedChars[ndx] = '\0'; // terminate the string
            recvInProgress = false;
            ndx = 0;
            newData = true;
        }
    }

    else if (rc == COMMAND_BEGIN_CHAR)      
        recvInProgress = true;
  }

  if (newData == true) {

    newData = false;
    memcpy(
      receivedNewData, 
      receivedChars, 
      COMMAND_LENGTH * sizeof(char));
      
  } else {

    receivedNewData[0] = 0;
    receivedNewData[1] = 0;
    receivedNewData[2] = 0;
  }

  if (receivedNewData[0] != 0 && receivedNewData[1] != 0 && receivedNewData[2] != 0) {
  
    // seperate the command from the parameter and process it
    
    char* command = strtok(receivedNewData, COMMAND_DIVIDER_CHAR);
    char* param = strtok(NULL, COMMAND_DIVIDER_CHAR);
    processSerialCommand(command, param);
  }
}
