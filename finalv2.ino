#include <BLEDevice.h>
#include "heltec.h"
#include "dht11.h"
#define DHT11PIN 21
#define DEVICE_NAME         "Carson BLE"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;
String message = "";

const int pump1 = 13; //yellow
const int pump2 = 12; //green


bool response = false;
const int dry = 2400;
const int wet = 1350;
int moisture = 0;
bool justWatered = false;
dht11 DHT11;
float light;
int waterSensorPin = 17;
bool waterInResevoir;



void printToScreen(String s) {
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, s);
  Heltec.display->display();
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      printToScreen("BLE client connected.");
    };

    void onDisconnect(BLEServer* pServer) {
      printToScreen("BLE client disconnected.");
      BLEDevice::startAdvertising();
    }
};

class MyCharacteristicCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *characteristic) {
    message = String(characteristic->getValue().c_str());
    printToScreen("Received:\n" + message);
  }
};

void setup() {
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(waterSensorPin, INPUT);

  Heltec.begin(true /*display*/, false /*LoRa*/, true /*Serial*/);
  printToScreen("Starting BLE!");

  BLEDevice::init(DEVICE_NAME);

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
  pCharacteristic->setValue("Init");

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();
}

void loop() {
  int chk = DHT11.read(DHT11PIN);
  float temperature = (DHT11.temperature * 9/5)+32;
  float humidity = DHT11.humidity;
  moisture = analogRead(A0);
  float volts = analogRead(37) * 5.0 / 1024.0;
  float amps = volts / 100000.0;
  float microamps = amps * 1000000;
  float lux = microamps * 2; 
  waterInResevoir = digitalRead(17);

   if (moisture >= dry){
    if (String(pCharacteristic->getValue().c_str()) == "update") {
    message = "Moisture: " + String(moisture) + ", Temperature: " + String(temperature) + ", Humidity: " + String(humidity) + ", JustWatered: " + String(justWatered) + ", Light: " + String(lux) + ", Resevoir: " + String(waterInResevoir);
    message.toUpperCase();
    pCharacteristic->setValue(message.c_str());
    justWatered = false;
    delay(250);
  }
    printToScreen("watering");
    justWatered = true;
    //start pump
    digitalWrite(pump1, HIGH);
    digitalWrite(pump2, LOW);
    delay(5000);
    printToScreen("not watering");
    digitalWrite(pump1, LOW);
    digitalWrite(pump2, HIGH);
  }

  if (String(pCharacteristic->getValue().c_str()) == "update") {
    message = "Moisture: " + String(moisture) + ", Temperature: " + String(temperature) + ", Humidity: " + String(humidity) + ", JustWatered: " + String(justWatered) + ", Light: " + String(lux) + ", Resevoir: " + String(waterInResevoir);
    message.toUpperCase();
    pCharacteristic->setValue(message.c_str());
    justWatered = false;
    delay(250);
  }
 
}
