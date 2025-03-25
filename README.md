# Food Dispenser System

## Overview

This project focuses on the development of an automated food dispenser using Arduino technology with an **ESP32** development board and the **Blynk** application for remote control and monitoring. The system is designed to monitor the quantity of food in a container and dispense additional food when needed. It also includes alert and notification features, ensuring timely refilling and efficient pet feeding.

## Features

- **Automatic Food Dispensing**: Dispenses food automatically using a servo motor when the detected weight falls below a threshold.
- **Manual Control**: Allows users to manually dispense food using a virtual button in the Blynk app.
- **Weight Monitoring**: Uses an HX711 load cell to measure food weight, with live data displayed in the Blynk app.
- **Audible & Visual Alerts**: LED and buzzer activate during dispensing to notify the user.
- **Auto-Shutdown**: Automatically closes the dispenser and resets indicators after 5 seconds or when sufficient food is dispensed.

## Components

1. **ESP32 Development Board**: Handles Wi-Fi/Bluetooth communication and system control.
2. **HX711 Load Cell Module**: Measures the weight of the food. Amplifies and reads load cell data.
3. **SG90 Servo Motor**: Visual indicator during food dispensing.
4. **Piezoelectric Buzzer**: Audible indicator during dispensing.

## System Behavior

- When the weight is **below 150g**, the system **automatically opens** the servo and starts dispensing food.
- The **LED and buzzer** are activated while food is being dispensed.
- After **5 seconds** or when **~150g of food is measured**, the servo closes automatically.
- Users can also **manually dispense food** using the Blynk app.

## Getting Started

1. Connect your ESP32 to the hardware components as described.
2. Flash the provided Arduino code to your ESP32.
3. Connect to Wi-Fi using the `ssid` and `pass` values. Both the microcontroller and the device on which the Blynk platform is accessed, need to do this.
4. Set up your Blynk app using:
   - `BLYNK_TEMPLATE_ID`
   - `BLYNK_TEMPLATE_NAME`
   - `BLYNK_AUTH_TOKEN`
5. Monitor and control the system remotely via Blynk.
