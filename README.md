# Smart Plant Watering System
## Overview
The Smart Plant Watering System is an Arduino-based project that measures the moisture level
of a plant's soil. When the soil is dry, the system uses a pump to move water from a reservoir
through a tube and out of a shower head to water the plant. Additionally, the system tracks
ambient light, temperature, and humidity, displaying the data on a web app using Bluetooth
Low Energy (BLE).

## Arduino Code
### Description
The Arduino code is written for the Heltec ESP32 Lora V2 board. It includes functionalities
to read data from various sensors, control water pumps, and communicate with a BLE client.

### Installation
1. Install the Arduino IDE if not already installed.
2. Open the Arduino IDE and install the necessary libraries:
- BLEPeripheral
- DHT Sensor Library
- Heltec ESP32 Dev-Boards
3. Connect the Heltec ESP32 Lora V2 board to your computer.
4. Open the Arduino sketch (`finalv2.ino`) and upload it to the board.
  
### Usage
1. Connect the moisture sensor, DHT11 sensor, and water level sensor to the appropriate pins on
the Heltec ESP32 Lora V2 board.
2. Power on the system and monitor the status of the pump(watering or not watering).
3. Connect the web app to the device via Bluetooth to view real-time sensor data and control the
system remotely.

## Web App (JavaScript)
### Description
The web app is built using Vue.js and communicates with the Arduino board through BLE to receive
sensor data and control the watering system.

### Usage
1. Open the index.html file in a web browser.
2. Enter the name of your plant in the provided input field and click "Save Name."
3. Click the "Connect to Device" button to establish a BLE connection with the Arduino board.
4. Use update stats button to receive current readings from the arduino.
### Contributing
Contributions are welcome! If you find any issues or have suggestions for improvement, please
open an issue or submit a pull request.
