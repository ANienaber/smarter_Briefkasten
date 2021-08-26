//Blynk Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "oO0of2TjCAU4Drwc10bgT3nD1DROd9D_";
char ssid[] = "BN";
char pass[] = "strenggeheim";

void setup(){
  Serial.begin(115200);
  //mit Blynk verbinden
  Blynk.begin(auth, ssid, pass, "iot.informatik.uni-oldenburg.de", 8080);
}


void loop(){
  Blynk.run();
  if(/*Signal LightSensor*/){
    Blynk.notify("Hey, ein neues Paket ist gekommen!");
  }
  if(/*Signal RFID*/){
    //Motor 1 und 2 ansteuern
  }
}
