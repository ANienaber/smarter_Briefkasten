//Blynk Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial
//Variablen zur Verbindung mit Blynk
char auth[] = "j_VuyJY24SRmAzo7RVOjfWb3cR2vgpGk";
char ssid[] = "BN";
char pass[] = "strenggeheim";

//Standardwert vom Abstandssensor, wenn kein Paket im Briefkasten ist
const int MAX_DISTANCE = 1000;

//Hilfsvariable damit Pushnachricht nur einmal geschickt wird
boolean package = false;

void setup() {
    Serial.begin(115200);
    //mit Blynk verbinden
    Blynk.begin(auth, ssid, pass);
    delay(500);
    //LichtschrankenPin als Ouput definieren
    pinMode(A0, INPUT);
}

void loop() {
  while(Blynk.connected() == false){
    Blynk.connect();
    Serial.print(".");
  }
  Blynk.run();
  if(packageDetected()){
    delay(2000);
    Blynk.notify("Paket");
    Serial.println("Paket");
  }
}

  /* Nutzt Abstandsensor, um Paket zu erkennen.
   * Sendet eine Nachricht via Blynk, falls ein neues Paket geliefert wurde.
   */
  boolean packageDetected(){
    //Wert den der Abstandssensor ausgibt
    int distance = analogRead(A0);
    //Delay, da auslesen des A0 Pins sonst WLAN Verbindung stört
    delay(100);
    //Wenn ein Paket vorliegt und Nachricht noch nicht gesendet wurde
    if(distance <= MAX_DISTANCE && package == false){
      package = true;
      return true;
      /*Serial.println("Du hast ein Paket erhalten");
      delay(500);
      //Blynk.notify("Du hast ein Paket erhalten");*/
    }
    //Wenn Paket entnommen wurde, dann setze alles auf Anfang, um neues Paket erkennen zu können
    if(distance > MAX_DISTANCE){
      package = false;
      return false;
    }

    return false;

}
