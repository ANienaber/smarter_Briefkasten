//RFID Sensor Libraries
#include <SPI.h>
#include <MFRC522.h>
//Motor Library
#include <WEMOS_Motor.h>
//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30,_MOTOR_A, 1000);//Motor A
Motor M2(0x30,_MOTOR_B, 1000);//Motor B

//Pins festelegen
//RFID
#define SS_PIN 15       //Pin D8
#define RST_PIN 0     //Pin D3

//Drehgeschwindigkeit der Motoren
const int SPEED = 50;
//Eingabe vom Taster
int taster;
//Hilfsvariable damit sich Türen nicht überdrehen
boolean isOpen = false;

//Variable um ID des RFID Chips speichern zu koennen
long chipID;
long grantedChipIDs[] = {913060, 1346750};
// MFRC522-Instanz erstellen
MFRC522 mfrc522(SS_PIN, RST_PIN);


  void setup(){ 
    //Build in LED zum Testend er Memory
    pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
    
    // Taster Pin
    pinMode(D2, INPUT); 
    
    // SPI-Bus und MFRC522initialisieren
    SPI.begin();
    mfrc522.PCD_Init();
    delay(10);
    // Details vom MFRC522 RFID READER / WRITER ausgeben
    mfrc522.PCD_DumpVersionToSerial();
  }


  void loop(){
    //Serial.begin(115200);
    button();
    if(RFIDDetected()){
      openDoor();
      //Serial.println("loop nach open");
    }
    delay(100);
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
       //Serial.println(chipID);

       //gelesene ChipID mit allen erlaubten ChipIDs vergleichen
       boolean grant = false;
       for(int i = 0; i < sizeof(grantedChipIDs); i++){
         long tempChipID = grantedChipIDs[i];
         if(chipID == tempChipID){
           grant = true;
         }
       }
       delay(250);
       //Je nach Ergebnis des Vergleichs wird true oder false zur�ckgegeben
       if (grant){
         //Serial.println("Zugang gewaehrt!");
         return true;
       } else {
         //Serial.println("Chip hat keine Zugangsrechte!");
         return false;
       }
    } else {
      //Serial.println("keinen Chip erkannt");
      return false;
    }
  }

  //oefnnet die Tueren des Briefkasten
  void openDoor(){  
    //Abfrage damit Türen sich nicht überdrehen
    if(isOpen){
      //Serial.println("Tür ist bereits geoeffnet!");
      return;
    }
    delay(200);
    //Motoren drehen sich im Uhrzeigersinn mit Geschwindikeit SPEED
    M1.setmotor( _CW, SPEED);
    M2.setmotor(_CCW, SPEED);
    //Motoren drehen sich für 1500 ms
    delay(1500);
    //Motoren werden angehalten
    M1.setmotor(_STOP);
    M2.setmotor(_STOP);
    //Zustand der Tür zwischenspeichern
    isOpen = true;
  }

  //Schliesst die Tueren des Briefkastens
  void closeDoor(){
     if(!isOpen){
      //Serial.println("Tür ist bereits geschlossen!");
      return;
     }
     delay(200); 
     M1.setmotor( _CCW, SPEED);
     M2.setmotor(_CW, SPEED);
     delay(1500);
     M1.setmotor(_STOP);
     M2.setmotor(_STOP);
     isOpen = false;
  }


  void button(){
    taster = digitalRead(D0);
    //Serial.println(digitalRead(D0));
    delay(250);
    if(taster == HIGH){
    closeDoor();
    }
}
