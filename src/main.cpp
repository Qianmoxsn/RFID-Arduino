#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

/* For task 1_2 */
#define NEW_UID {0x11, 0x45, 0x14, 0xAA}

// Instance of the class and key
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;


/**
 * Helper function to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte* buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  Serial.print("\n");
}


/**
 * Helper function to dump a byte array as hex values to a string.
 */
String buf2str(byte* buffer, byte bufferSize) {
  // Return the string of the buffer in HEX
  String str = "";
  for (byte i = 0; i < bufferSize; i++) {
    str += buffer[i] < 0x10 ? " 0" : " ";
    str += String(buffer[i], HEX);
  }
  // Convert the string to uppercase
  str.toUpperCase();
  // Serial.print("DEBUG>");
  // Serial.println(str);
  return str;
}


/**
 * reads the UID again but does not display the ID to the serial monitor
 * checks if the ID is param uid.
*/
void get_UID(String uid) {
  // Check if the UID is the same as the target
  // Verify if the UID has been readed
  Serial.print("Checking UID >> ");

  if (uid.compareTo(buf2str(rfid.uid.uidByte, rfid.uid.size)) == 0) {
    Serial.println("Correct tag !!");
  }
  else {
    Serial.println("Wrong tag !!");
  }

}

void task1setup() {
  Serial.begin(9600); // Init serial port
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  delay(100);

  // Write the default key to the key buffer(FF FF FF FF FF FF)
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("Task 1 - Tag UID"));
  Serial.print(F("Using key in HEX:"));
  dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println(F("====================LOOP===================="));
}
void task1loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!rfid.PICC_IsNewCardPresent())
    return;
  // Verify if the UID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;

  // Print Card uid
  Serial.print(F(">> Card UID:"));
  dump_byte_array(rfid.uid.uidByte, rfid.uid.size);

  // Print PICC type
  Serial.print(F(">> PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Not of type MIFARE Classic.\n"));
    return;
  }
  else {
    Serial.println(rfid.PICC_GetTypeName(piccType));
    Serial.println();
  }

  String uid = " 63 B0 15 13";
  get_UID(uid);

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  Serial.println(F("--------------------"));
}


void task1_2setup() {
  Serial.begin(9600);  // Initialize serial communications with the PC
  while (!Serial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();         // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522 card
  Serial.println(F("Warning: this example overwrites the UID of your UID changeable card, use with care!"));

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}
void task1_2loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle. And if present, select one.
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Now a card is selected. The UID and SAK is inrfid.uid.

  // Dump UID
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Dump PICC type
//  MFRC522::PICC_Type piccType =rfid.PICC_GetType(rfid.uid.sak);
//  Serial.print(F("PICC type: "));
//  Serial.print(rfid.PICC_GetTypeName(piccType));
//  Serial.print(F(" (SAK "));
//  Serial.print(rfid.uid.sak);
//  Serial.print(")\r\n");
//  if (  piccType != MFRC522::PICC_TYPE_MIFARE_MINI 
//    &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
//    &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
//    Serial.println(F("This sample only works with MIFARE Classic cards."));
//    return;
//  }

  // Set new UID
  byte newUid[] = NEW_UID;
  if (rfid.MIFARE_SetUid(newUid, (byte)4, true)) {
    Serial.println(F("Wrote new UID to card."));
  }

  // Halt PICC and re-select it so DumpToSerial doesn't get confused
  rfid.PICC_HaltA();
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Dump the new memory contents
  Serial.println(F("New UID and contents:"));
  rfid.PICC_DumpToSerial(&(rfid.uid));

  delay(2000);
}


void task2setup() {

}
void task2loop() {

}


void setup() {
  // task1setup();
  task1_2setup();
  // task2setup();
}

void loop() {
  // task1loop();
  task1_2loop();
  // task2loop();
}
