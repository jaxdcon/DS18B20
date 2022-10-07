#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = D3;
bool deviceFound = false;
String ss;
OneWire oneWire(oneWireBus);
DallasTemperature DS18B20(&oneWire);
DeviceAddress deviceAddress;

void findDevice(DeviceAddress deviceAddress){
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) {
      deviceFound = false;
      continue;
    }
    deviceFound = true;
    ss += String(deviceAddress[i], HEX);
  }
}

String getTempratureCode(float temp) {
  String tempratureCode;
  if (temp < 10)
  {
    tempratureCode = "Cold!";
  }
  else if (temp <= 15)
  {
    tempratureCode = "Cool";
  }
  else if (temp <= 25)
  {
    tempratureCode = "Perfect";
  }
  else if (temp <= 30)
  {
    tempratureCode = "Warm";
  }
  else if (temp <= 35)
  {
    tempratureCode = "Hot";
  }
  else
  {
    tempratureCode = "Too Hot!";
  }
  return tempratureCode;
}

void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  DS18B20.begin();
  DS18B20.getAddress(deviceAddress, 0);

  findDevice(deviceAddress);

  //Exit Early
  if(!deviceFound) {
    Serial.println("No temprature sensor was found, process exiting");
    reboot();
  }

  Serial.println("Found DS18B20 sensor with address: " + ss);
}

void loop() {
  float fTemp;

  DS18B20.requestTemperatures();
  fTemp = DS18B20.getTempCByIndex(0);

  Serial.println("Current temprature is: " + String(fTemp) + "° C or " + String(getTempratureCode(fTemp)));

  delay(5000);
}
