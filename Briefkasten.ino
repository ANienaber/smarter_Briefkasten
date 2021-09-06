//Blynk Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//RFID Sensor Libraries
#include <SPI.h>
#include <MFRC522.h>
//Bibliothek zum speichern von Daten in der Flashdrive
#include <ESP_EEPROM.h>
//Motor Library
#include <WEMOS_Motor.h>
//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30,_MOTOR_A, 1000);//Motor A
Motor M2(0x30,_MOTOR_B, 1000);//Motor B

//Pins festelegen
//RFID
#define SS_PIN 15       //Pin D8
#define RST_PIN 0 		//Pin D3



//Hilfsvariable damit Pushnachricht nur einmal geschickt wird
boolean package = false;
//Standardwert vom Abstandssensor, wenn kein Paket im Briefkasten ist
const int MAX_DISTANCE = 200;
//Drehgeschwindigkeit der Motoren
const int SPEED = 50;
//Eingabe vom Taster
int taster;
//Hilfsvariable damit sich Türen nicht überdrehen
boolean isOpen = false;

//Variable um ID des RFID Chips speichern zu k�nnen
long chipID;
long grantedChipIDs[] = {913060, 1346750};
// MFRC522-Instanz erstellen
MFRC522 mfrc522(SS_PIN, RST_PIN);


//Variablen zur Verbindung mit Blynk
char auth[] = "oO0of2TjCAU4Drwc10bgT3nD1DROd9D_";
char ssid[] = "BN";
char pass[] = "strenggeheim";

	void setup(){ 
	  //Build in LED zum Testend er Memory
	  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
		Serial.begin(115200);

		//mit Blynk verbinden
		Blynk.begin(auth, ssid, pass, "iot.informatik.uni-oldenburg.de", 8080);

		//LichtschrankenPin als Ouput definieren
		pinMode(A0, INPUT);
    
    // Taster Pin
		pinMode(D2, INPUT); 
		
		// SPI-Bus und MFRC522initialisieren
		SPI.begin();
		mfrc522.PCD_Init();
		delay(10);
		// Details vom MFRC522 RFID READER / WRITER ausgeben
		mfrc522.PCD_DumpVersionToSerial();

    //EEPROM mit minimaler Größe 16 beginnen 
    EEPROM.begin(16);
    //erstmaligen Wert in EEPROM speichern
    EEPROM.put(0, isOpen);
    boolean ok1 = EEPROM.commit();
    Serial.println((ok1) ? "First commit OK" : "Commit failed");
	}


	void loop(){
		Blynk.run();
		packageDetected();
    button();

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
	 * Wenn der passende Chip gelesen wird, wird true zur�ckgegeben, sonst false.
	 */
	boolean RFIDDetected(){
    
		if (mfrc522.PICC_IsNewCardPresent()){
			 //ChipID wird resettet
			 chipID = 0;
       mfrc522.PICC_ReadCardSerial();
       
			 // Neue ChipID in Variable laden
			 for (byte i = 0; i < mfrc522.uid.size; i++){
				 chipID=((chipID+mfrc522.uid.uidByte[i])*10);
			 }
			 Serial.println(chipID);

			 //gelesene ChipID mit allen erlaubten ChipIDs vergleichen
			 boolean grant = false;
			 for(int i = 0; i < sizeof(grantedChipIDs); i++){
				 long tempChipID = grantedChipIDs[i];
				 if(chipID == tempChipID){
					 grant = true;
				 }
			 }
			 //Je nach Ergebnis des Vergleichs wird true oder false zur�ckgegeben
			 if (grant){
				 Serial.println("Zugang gewaehrt!");
				 return true;
			 } else {
				 Serial.println("Chip hat keine Zugangsrechte!");
				 return false;
			 }
		} else Serial.println("keinen Chip erkannt");
	}

	//oefnnet die Tueren des Briefkasten
	void openDoor(){
    //Wert aus EEPROM holen
    EEPROM.get(0, isOpen);
    
    //Abfrage damit Türen sich nicht überdrehen
    if(isOpen){
      Serial.println("Tür ist bereits geoeffnet!");
      //Built in LED zum Testen der Memory blinken lassen
      digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
      delay(1000);                      // Wait for a second
      digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
      delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
      return;
    }
    //Motoren drehen sich im Uhrzeigersinn mit Geschwindikeit SPEED
    M1.setmotor( _CW, SPEED);
    M2.setmotor(_CW, SPEED);
    //Motoren drehen sich für 1500 ms
    delay(1500);
    //Motoren werden angehalten
    M1.setmotor(_STOP);
    M2.setmotor(_STOP);
    //Zustand der Tür zwischenspeichern
    isOpen = true;

    //Zustand der Tür in EEPROM speichern
    EEPROM.put(0, isOpen);
    boolean ok2 = EEPROM.commit();
    Serial.println((ok2) ? "Open commit OK" : "Open Commit failed");
	}

	//Schliesst die Tueren des Briefkastens
	void closeDoor(){
     //Wert aus EEPROM holen
     EEPROM.get(0, isOpen);
     //analog zu openDoor
     if(!isOpen){
      Serial.println("Tür ist bereits geschlossen!");
      return;
     }
     M1.setmotor( _CCW, SPEED);
     M2.setmotor(_CCW, SPEED);
     delay(1500);
     M1.setmotor(_STOP);
     M2.setmotor(_STOP);
     isOpen = false;
     
     //Zustand der Tür in EEPROM speichern
     EEPROM.put(0, isOpen);
     boolean ok3 = EEPROM.commit();
     Serial.println((ok3) ? "Close commit OK" : "Close Commit failed");
	}


  void button(){
    taster = digitalRead(D2);
    Serial.println(digitalRead(D2));
    delay(250);
    if(taster == HIGH){
    closeDoor();
    }
}
