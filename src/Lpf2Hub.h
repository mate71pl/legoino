/*
 * Lpf2Hub.h - Arduino base class for controlling Powered UP and Boost controllers
 * 
 * (c) Copyright 2019 - Cornelius Munz
 * Released under MIT License
 * 
*/

#ifndef Lpf2Hub_h
#define Lpf2Hub_h

#include "Arduino.h"
#include "NimBLEDevice.h"
#include <functional>
using namespace std::placeholders;

#define LPF2_UUID "00001623-1212-efde-1623-785feabcd123"
#define LPF2_CHARACHTERISTIC "00001624-1212-efde-1623-785feabcd123"

#define LPF2_VOLTAGE_MAX 9.6
#define LPF2_VOLTAGE_MAX_RAW 3893

#define LPF2_CURRENT_MAX 2444
#define LPF2_CURRENT_MAX_RAW 4095

//#define LOGGING_ENABLED

#ifdef LOGGING_ENABLED
  #define LOGLINE(...) Serial.println(__VA_ARGS__)
  #define LOG(...) Serial.print(__VA_ARGS__)
#else
  #define LOGLINE(...) 
  #define LOG(...)
#endif

typedef void (*ButtonCallback)(bool isPressed);

typedef enum HubType
{
    UNKNOWN = 0,
    WEDO2_SMART_HUB = 1,
    BOOST_MOVE_HUB = 2,
    POWERED_UP_HUB = 3,
    POWERED_UP_REMOTE = 4,
    DUPLO_TRAIN_HUB = 5,
    CONTROL_PLUS_HUB = 6
};

typedef enum BLEManufacturerData {
    DUPLO_TRAIN_HUB_ID = 32, //0x20
    BOOST_MOVE_HUB_ID = 64, //0x40
    POWERED_UP_HUB_ID = 65, //0x41
    POWERED_UP_REMOTE_ID = 66, //0x42
    CONTROL_PLUS_LARGE_HUB_ID = 128 //0x80
};

typedef enum DeviceType
{
  UNDEFINED = 0,
  BASIC_MOTOR = 1,
  TRAIN_MOTOR = 2,
  LED_LIGHTS = 8,
  BOOST_LED = 22,
  BOOST_DISTANCE = 37,
  BOOST_TACHO_MOTOR = 38,
  BOOST_MOVE_HUB_MOTOR = 39,
  BOOST_TILT = 40,
  POWERED_UP_REMOTE_BUTTON = 55
};

typedef struct Device
{
    byte PortNumber;
    byte DeviceType;
};

typedef enum Color
{
  BLACK = 0,
  PINK = 1,
  PURPLE = 2,
  BLUE = 3,
  LIGHTBLUE = 4,
  CYAN = 5,
  GREEN = 6,
  YELLOW = 7,
  ORANGE = 8,
  RED = 9,
  WHITE = 10,
  NONE = 255
};

 static const char *COLOR_STRING[] = {
    "black", "pink", "purple", "blue", "lightblue", "cyan", "green", "yellow", "orange", "red", "white"
};


class Lpf2Hub
{
private:

  // Notification callbacks
  ButtonCallback _buttonCallback = nullptr;
  

public:
  enum Port
  {
    A = 0x37,
    B = 0x38,
    AB = 0x39,
    C = 0x01,
    D = 0x02,
    TILT = 0x3A
  };

  Lpf2Hub();
  void init();
  void init(std::string deviceAddress);
  void initConnectedDevices(Device devices[], byte deviceNumbers);
  bool connectHub();
  bool isConnected();
  bool isConnecting();

  void setHubName(char name[]);
  void shutDownHub();
  byte getDeviceTypeForPortNumber(byte portNumber);
  void setLedColor(Color color);
  void setLedRGBColor(char red, char green, char blue);
  void setLedHSVColor(int hue, double saturation, double value);

  void registerButtonCallback(ButtonCallback buttonCallback);
  void WriteValue(byte command[], int size);
  static byte MapSpeed(int speed);
  static  byte *Int16ToByteArray(int16_t x);
  static  byte *Int32ToByteArray(int32_t x);
  static  unsigned char ReadUInt8(uint8_t *data, int offset);
  static  signed char ReadInt8(uint8_t *data, int offset);
  static  unsigned short ReadUInt16LE(uint8_t *data, int offset);
  static  signed short ReadInt16LE(uint8_t *data, int offset);
  static  unsigned int ReadUInt32LE(uint8_t *data, int offset);
  static  signed int ReadInt32LE(uint8_t *data, int offset);
  void parseDeviceInfo(uint8_t *pData);
  void parsePortMessage(uint8_t *pData);
  void parseSensorMessage(uint8_t *pData);
  static  void parseBoostDistanceAndColor(uint8_t *data);
  static  void parseBoostTachoMotor(uint8_t *data);
  static  void parseBoostHubMotor(uint8_t *pData);
  static  void parseBoostTiltSensor(uint8_t *data);
  static void parsePoweredUpRemote(uint8_t *pData);
  void parsePortAction(uint8_t *pData);
  static  byte getModeForDeviceType(byte deviceType);
  void registerPortDevice(byte portNumber, byte deviceType);
  void deregisterPortDevice(byte portNumber);
  void activatePortDevice(byte portNumber, byte deviceType);
  void activatePortDevice(byte portNumber);
  void deactivatePortDevice(byte portNumber, byte deviceType);
  void deactivatePortDevice(byte portNumber);
  void activateButtonReports();
  void notifyCallback(NimBLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify);
  void activateHubUpdates();
  int getTachoMotorRotation();
  double getDistance();
  int getColor();
  int getRssi();
int getBatteryLevel();
double getHubVoltage();
double getHubCurrent();
int getBoostHubMotorRotation();
int getTiltX(); 
int getTiltY();
int getFirmwareVersionBuild();
int getFirmwareVersionBugfix();
int getFirmwareVersionMajor();
int getFirmwareVersionMinor();
int getHardwareVersionBuild();
int getHardwareVersionBugfix();
int getHardwareVersionMajor();
int getHardwareVersionMinor();
HubType getHubType();
bool isButtonPressed();
bool isLeftRemoteUpButtonPressed();
bool isLeftRemoteDownButtonPressed();
bool isLeftRemoteStopButtonPressed();
bool isLeftRemoteButtonReleased();
bool isRightRemoteUpButtonPressed();
bool isRightRemoteDownButtonPressed();
bool isRightRemoteStopButtonPressed();
bool isRightRemoteButtonReleased();
  BLEUUID _bleUuid;
  BLEUUID _charachteristicUuid;
  BLEAddress *_pServerAddress;
  BLEAddress *_requestedDeviceAddress = nullptr;
  BLERemoteCharacteristic *_pRemoteCharacteristic;

  boolean _isConnecting;
  boolean _isConnected;
  HubType _hubType;
};

#endif