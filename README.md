# Pi Stream External App

A NumWorks calculator external app for displaying streaming data from a Raspberry Pi via UART connection.

## Features

- 📡 **Real-time UART data display** from Raspberry Pi
- 🧮 **Mathematical expression rendering** for formulas
- 📊 **Sensor data visualization** (temperature, CPU usage)
- 🎮 **Interactive controls** (OK to clear, Back to exit)
- 🎨 **Clean interface** with header and status lines

## Installation

### Prerequisites
- **NumWorks N0110 calculator** with **Upsilon firmware**
- **Google Chrome browser** (for WebUSB support)

### Steps
1. Download the latest `pi_stream.nwa` from [GitHub Releases](https://github.com/cabrerairigoyen/apo/actions)
2. Open [Upsilon External Apps Installer](https://upsilonnumworks.github.io/Upsilon-External/)
3. Connect your calculator via USB
4. Select and upload the `pi_stream.nwa` file
5. Click "Install" and wait for completion
6. The app will appear in your calculator's external apps menu

## Hardware Setup

### Raspberry Pi Configuration
Connect your Raspberry Pi to the calculator's UART pins:

```bash
# Enable UART on Raspberry Pi
sudo raspi-config
# Interface Options -> Serial Port -> Enable

# Send test data
echo "Hello from Pi!" > /dev/serial0
echo "Temperature: 25.3°C" > /dev/serial0
echo "$$E = mc^2$$" > /dev/serial0  # LaTeX math
```

### UART Settings
- **Baud Rate:** 115200
- **Data Bits:** 8
- **Stop Bits:** 1
- **Parity:** None

## App Controls

- **Back Button:** Exit the app
- **OK Button:** Clear the display
- **Auto-refresh:** Updates every 20ms

## Build from Source

### Requirements
- ARM GCC toolchain (`gcc-arm-none-eabi`)
- Node.js and npm
- NumWorks `nwlink` tool

### Commands
```bash
# Clone the repository
git clone https://github.com/cabrerairigoyen/apo.git
cd apo

# Build the external app
make clean
make build

# The .nwa file will be in output/pi_stream.nwa
```

## Development

This app uses the NumWorks EADK (External Application Development Kit):

- **Entry point:** `src/main.c`
- **Icon:** `src/icon.png` (automatically converted to NWI format)
- **External data:** `src/input.txt`
- **Build system:** `nwlink` + ARM GCC

## Technical Details

### EADK Structure
- **App name:** Stored in `.rodata.eadk_app_name` section
- **API level:** Stored in `.rodata.eadk_api_level` section  
- **Icon:** Generated from PNG and stored in `.rodata.eadk_app_icon`
- **Entry point:** Standard `main()` function

### Display System
- **Screen resolution:** 320×240 pixels
- **Color format:** RGB565
- **Rendering:** Direct pixel manipulation via EADK API

## Raspberry Pi Integration

The app is designed to receive data from a Raspberry Pi connected via UART. Example Python sender:

```python
import serial
import time

# Configure UART
ser = serial.Serial('/dev/serial0', 115200)

# Send sensor data
while True:
    ser.write(b"Temperature: 24.5°C\n")
    ser.write(b"CPU: 35%\n")
    ser.write(b"$$\\pi \\approx 3.14159$$\n")  # Math formula
    time.sleep(1)
```

## License

This project is open source. NumWorks is a registered trademark.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test on actual hardware
5. Submit a pull request

---

**Note:** This app requires Upsilon firmware on N0110 calculators. It will not work on N0100 models or with standard Epsilon firmware.
