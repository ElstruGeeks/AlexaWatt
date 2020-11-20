#define BLYNK_PRINT Serial
 
#include "EmonLib.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
 
EnergyMonitor emon;
//#define vCalibration 106.8
#define currCalibration 29.0322
BlynkTimer timer;
 
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "_gbGRcAHB3N5V1QOhkFIwo66NOinmT8b";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "velassery";
char pass[] = "manjummel";

#define switchone 26
 
float kWh = 0;
float Power = 0;
unsigned long lastmillis = millis();
 
void myTimerEvent() {
    emon.calcVI(20, 2000);
   // Serial.print("Vrms: ");
   // Serial.print(emon.Vrms, 2);
   // Serial.print("V");
   // Blynk.virtualWrite(V0, emon.Vrms);
    Serial.print("\tIrms: ");
    Serial.print(emon.Irms, 4);
    Serial.print("mA");
    Blynk.virtualWrite(V1, emon.Irms);
    Serial.print("\tPower: ");
    Serial.print(Power, 4);
    Serial.print("W");
    Power = emon.Irms * 2.40;
    Blynk.virtualWrite(V2, Power);
    Serial.print("\tkWh: ");
    kWh = kWh + Power*(millis()-lastmillis)/3600000000.0;
    Serial.print(kWh, 4);
    Serial.println("kWh");
    lastmillis = millis();
    Blynk.virtualWrite(V3, kWh);
}
 
void setup() {
  Serial.begin(115200);
  pinMode(switchone,OUTPUT);
  digitalWrite(switchone,LOW);
 // emon.voltage(35, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(34, currCalibration); // Current: input pin, calibration.
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, myTimerEvent);
}
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  digitalWrite(switchone,pinValue);
  }
void loop() 
{
  
  Blynk.run();
  timer.run();
}
