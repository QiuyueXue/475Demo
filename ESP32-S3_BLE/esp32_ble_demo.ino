#include <Wire.h>
#include <ArduinoBLE.h>
#define CHARACTERISTIC_SIZE  7// Change based on your requirement
BLEService customService("10336bc0-c8f9-4de7-b637-a68b7ef33fc9");  // 1816 is the defined UUID for cycling tech...
BLECharacteristic txCharacteristic("43336bc0-c8f9-4de7-b637-a68b7ef33fc9",  // Custom characteristic UUID
                                   BLERead | BLENotify, 
                                   CHARACTERISTIC_SIZE);  // Characteristic value length
BLEDescriptor myDescriptor("00002902-0000-1000-8000-00805f9b34fb", "0");  // Used for enabling notifications.



void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting...");

  // Initialize BLE hardware
  if (!BLE.begin()) {
    while (1) {
      Serial.println("Starting BLE failed!");
      delay(1000);
    }
  }
  
  // Set the local name and service information
  BLE.setLocalName("BLEDemo");
  BLE.setAdvertisedService(customService);
  // Add custom characteristic
  customService.addCharacteristic(txCharacteristic);
  txCharacteristic.addDescriptor(myDescriptor);
  BLE.addService(customService);
  
  // Start advertising
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}



void loop() {
  BLEDevice central = BLE.central();
  Serial.println("Waiting to connect to central.");
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected()) {
      uint8_t packet[CHARACTERISTIC_SIZE];     // initialize packet data array
      String data_string = "my ble ";
      data_string.getBytes(packet, CHARACTERISTIC_SIZE);
      txCharacteristic.writeValue(packet, CHARACTERISTIC_SIZE);
      delay(200);  // check sensor data every 100ms

      data_string = "is now ";
      data_string.getBytes(packet, CHARACTERISTIC_SIZE);
      txCharacteristic.writeValue(packet, CHARACTERISTIC_SIZE);
      delay(200);  // check sensor data every 100ms

      data_string = "working";
      data_string.getBytes(packet, CHARACTERISTIC_SIZE);
      txCharacteristic.writeValue(packet, CHARACTERISTIC_SIZE);
      delay(200);  // check sensor data every 100ms
    }
      
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
    delay(1000);
  }
}
