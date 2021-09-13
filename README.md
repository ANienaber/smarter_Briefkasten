Der smarte Briefkasten, ermöglicht es Pakete von Personen zu empfangen, die einen Autorisierten RFID-Tag besitzen.

Für den Zusammenbau werden MDF Holzbretterund Acrylglas, die mit einem Lasercutter geschnitten werden, ein 3D-Drucker. Hiermit werden die Modelle, die in dem Ordner Modellierung zu finden sind, gefertigt. Diese Modelle werden zusammengesteckt und geklebt
Zu dieser Hardware gehören, zwei ESP8266 D1 Mini Mikrocontroller, ein Taster, eine Tripler Base, ein Motorshield, zwei DC-Motoren, ein RFID-Sensor, und ein Abstandssensor.
Die beiden Dateien, die in dem Programmieren Ordner zu finden sind, müssen jeweils auf einen Mikrocontroller hochgeladen werden. 
Weitere Angaben zum Zusammenbau des Briefkasten können auf dem zugehörigen Blog gelesen werden. https://wp.uni-oldenburg.de/soft-skills-und-technische-kompetenz-wise20202021-sosepg-4/

Der Briefkasten wird wie folgt genutzt:
Bei erstmaliger Nutzung muss die Blynk App verbunden werden. Hierfür müssen im Programmcode die Wlan-Daten und der Authentifizierungstoken ergänzt werden. 
Außerdem sollte darauf geachtet werden, dass vor dem Start die Türen geschlossen sind. Ansonsten kann es sein, dass sich die Türen zu weit drehen. 
Die vorgesehene Bedienung des Briefkastens sieht wie folgt aus:

1. Den RFID Chip vor dem RFID Sensor halten, damit sich die Türen öffnen
2. Das Paket bzw. ein Gegenstand in den Briefkasten legen. Dann wird über die Blynk App eine Push Nachricht gesendet
3. Den Knopf betätigen, sodass sich die Türen wieder schließen
4. Mit dem RFID Chip die Türen wieder öffnen und das Paket entnehmen
5. Mit dem Knopf die Türen wieder schließen

Bekannte Probleme:
Türen lassen sich nicht mit dem RFID Chip öffnen:
Die Stromversorgung  muss neu eingeschaltet werden, bis die Türen wieder funktionieren. Bei erstmaliger Durchführung funktioniert es meist wieder.
Wenn der Briefkasten längere Zeit nicht genutzt wird, kann dieses Problem auftreten. 
