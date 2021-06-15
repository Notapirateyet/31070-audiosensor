// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char THING_ID[] = "ddb5b299-3f31-4cd1-9061-4f4462e501dc";

const char SSID[] = SECRET_SSID; // Network SSID (name)
const char PASS[] = SECRET_PASS; // Network password (use for WPA, or use as key for WEP)

void onDBBoundaryChange();
void onDBReadChange();
void onTest1Change();
void onTest2Change();

int dB_boundary;
int dB_read;
String test1;
CloudAcceleration test2;

void initProperties()
{

  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(dB_boundary, READWRITE, ON_CHANGE, onDBBoundaryChange);
  ArduinoCloud.addProperty(dB_read, READWRITE, 5 * SECONDS, onDBReadChange);
  ArduinoCloud.addProperty(test1, READWRITE, ON_CHANGE, onTest1Change);
  ArduinoCloud.addProperty(test2, READWRITE, ON_CHANGE, onTest2Change);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
