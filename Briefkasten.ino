//Blynk Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "oO0of2TjCAU4Drwc10bgT3nD1DROd9D_";
char ssid[] = "";
char pass[] = "";

//Pins festelegen
#define LightSensor A0

//Hilfsvariable damit Pushnachricht nur einmal geschickt wird
boolean package = false;
//Standardwert vom Abstandssensor, wenn kein Paket im Briefkasten ist
const int MAX_DISTANCE = 200;

void setup(){
	Serial.begin(115200);
	//mit Blynk verbinden
	Blynk.begin(auth, ssid, pass, "iot.informatik.uni-oldenburg.de", 8080);;
	pinMode(A0, INPUT);

}


void loop(){
	Blynk.run();
	packageDetected();
	if(/*Signal RFID*/){
		//Motor 1 und 2 ansteuern
	}
}



void packageDetected(){
  //Wert den der Abstandssensor ausgibt
  int distanz = analogRead(A0);
  //Wenn ein Paket vorliegt und Nachricht noch nicht gesendet wurde
  if(distanz <= MAX_DISTANCE && package == false){
    package = true;
    Blynk.notify("Du hast ein Paket erhalten");
  }else package = false; // sonst kein Paket vorhanden

}
