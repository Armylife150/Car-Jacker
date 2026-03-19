# 👣 Cyber-Jacker: Sasquatch Edition 👾

An elite vehicle signal scanning simulation for the **Flipper Zero**, featuring the legendary Cyber-Squatch. This app is designed to run on the **Momentum Firmware** (v0.1.2+) but is compatible with most custom Flipper forks.

## 🚀 Features
* **Cyber-Scanner:** Simulated high-end and "silly" vehicle detection.
* **Legendary Icon:** Custom 10x10 Cyber-Squatch pixel art.
* **Stability:** Thread-safe rendering using `FuriMutex` to prevent system crashes.
* **True Randomness:** Utilizes the Flipper's hardware RNG for unpredictable results.

## 🛠️ Installation

### 1. Requirements
* A Flipper Zero running **Momentum** or a similar custom firmware.
* The firmware source code on your computer.

### 2. Setup
1. Clone this repository or download the files.
2. Navigate to your firmware's `applications_user` directory.
3. Create a folder named `car_jacker`.
4. Drop `carjacker.c` and `application.fam` into that folder.

### 3. Compile & Launch
Connect your Flipper via USB and run:
```bash
fbt launch_app APPID=car_jacker
 Controls
OK: Toggle the scanner ON/OFF.
BACK: Exit the application and return to the home screen.
UP/DOWN: Scroll through your list of "captured" vehicles.
🛡️ Stability Notes
This app has been refactored to use constant pointers for vehicle names, significantly reducing heap fragmentation and preventing the "Out of Memory" errors common in long-running simulation apps.
Created by RocketGod 🚀
