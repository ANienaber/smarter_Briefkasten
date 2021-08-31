//Blynk Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//RFID Sensor Libraries
#include <SPI.h>
#include <MFRC522.h>


//Pins festelegen
//RFID
#define SS_PIN 15       //Pin D8
#define RST_PIN 0 		//Pin D3


//Hilfsvariable damit Pushnachricht nur einmal geschickt wird
boolean package = false;
//Standardwert vom Abstandssensor, wenn kein Paket im Briefkasten ist
const int MAX_DISTANCE = 200;
//Variable um ID des RFID Chips speichern zu können
long chipID;
long grantedChipID;
//Variablen zur Verbindung mit Blynk
char auth[] = "oO0of2TjCAU4Drwc10bgT3nD1DROd9D_";
char ssid[] = "BN";
char pass[] = "strenggeheim";


// MFRC522-Instanz erstellen
MFRC522 mfrc522(SS_PIN, RST_PIN);


	void setup(){
		Serial.begin(115200);

		//mit Blynk verbinden
		Blynk.begin(auth, ssid, pass, "iot.informatik.uni-oldenburg.de", 8080);

		//LichtschrankenPin als Ouput definieren
		pinMode(IRSensor, OUTPUT);

		// SPI-Bus und MFRC522initialisieren
		SPI.begin();
		mfrc522.PCD_Init();
		delay(10);

		// Details vom MFRC522 RFID READER / WRITER ausgeben
		mfrc522.PCD_DumpVersionToSerial();
	}


	void loop(){
		Blynk.run();
		packageDetected();

		if(RFIDDetected()){
			openDoor();
		}
	}



	/* Nutzt Abstandsensor, um Paket zu erkennen.
	 * Sendet eine Nachricht via Blynk, falls ein neues Paket geliefert wurde.
	 */
	void packageDetected(){
	  //Wert den der Abstandssensor ausgibt
	  int distanz = analogRead(A0);
	  //Wenn ein Paket vorliegt und Nachricht noch nicht gesendet wurde
	  if(distanz <= MAX_DISTANCE && package == false){
		package = true;
		Blynk.notify("Du hast ein Paket erhalten");
	  }else package = false; // sonst kein Paket vorhanden

	}


	/* Nutzt RFID Sensor, um neu gelesenen Chip mit angegebener ChipID zu vergleichen.
	 * Wenn der passende Chip gelesen wird, wird true zurückgegeben, sonst false.
	 */
	boolean RFIDDetected(){
		if (mfrc522.PICC_IsNewCardPresent()){
			 //ChipID wird resettet
			 chipID = 0;

			 // Neue ChipID in Variable laden
			 for (byte i = 0; i < mfrc522.uid.size; i++){
				 chipID=((chipID+mfrc522.uid.uidByte[i])*10);
			 }

			 //ChipID wird mit der zugelassenen ChipID verglichen
			 if (chipID == grantedChipID){
				 Serial.println("Zugang gewährt!");
				 return true;
			 } else {
				 Serial.println("Chip hat keien Zugangsrechte!");
				 return false;
			 }
		}
	}

	//Öfnnet die Türen des Briefkasten
	void openDoor(){

	}

	//Schließt die Türen des Briefkastens
	void closeDoor(){

	}
