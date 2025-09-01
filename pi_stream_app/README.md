# Pi Stream Display App for Omega

A custom app for the Omega calculator that displays streaming data from a Raspberry Pi via UART, with support for both text and LaTeX mathematical expressions.

## Features

- 📡 Real-time UART data streaming from Raspberry Pi
- 🧮 LaTeX math expression rendering (using `$$math$$` or `\(math\)` delimiters)
- 📜 Scrollable text display
- 🎨 Modern UI following Omega design patterns

## Files Structure

```
pi_stream_app/
├── pi_stream_app.h              # Main app header
├── pi_stream_app.cpp            # Main app implementation
├── pi_stream_controller.h       # Controller header
├── pi_stream_controller.cpp     # Controller implementation
├── pi_stream_icon.h             # Icon header file
├── pi_stream_icon.png           # App icon (16x16)
├── base.en.i18n                 # English localization
├── Makefile                     # Build configuration
└── README.md                    # This file
```

## Installation

1. **Copy to Omega source:**
   ```bash
   cp -r pi_stream_app /path/to/Omega/apps/
   ```

2. **Add to build configuration:**
   Edit `/path/to/Omega/build/config.mak` and add `pi_stream_app` to `EPSILON_APPS`:
   ```makefile
   EPSILON_APPS ?= ... pi_stream_app
   ```

3. **Add icon to themes:**
   - Copy `pi_stream_icon.png` to each theme directory in `/path/to/Omega/themes/icons/`
   - Edit `/path/to/Omega/themes/icon.json` and add:
   ```json
   "apps/pi_stream_app/pi_stream_icon.png": "apps/pi_stream_icon.png",
   ```

4. **Build Omega:**
   ```bash
   cd /path/to/Omega
   make clean
   make
   ```

## Usage

### Raspberry Pi Setup

[[memory:5135124]] Configure your Raspberry Pi to send data via UART:

1. **Enable UART:**
   ```bash
   sudo raspi-config
   # Interface Options -> Serial Port -> Enable
   ```

2. **Send data:**
   ```python
   import serial
   import time

   ser = serial.Serial('/dev/serial0', 115200)
   
   # Send text
   ser.write(b"Hello from Pi!\n")
   
   # Send LaTeX math
   ser.write(b"$$E = mc^2$$\n")
   ser.write(b"\\(\\int_0^\\pi \\sin(x) dx = 2\\)")
   ```

### Calculator Usage

1. Navigate to the Pi Stream app on your Omega calculator
2. Connect UART cable between Pi and calculator
3. Data will appear in real-time
4. Math expressions in `$$` or `\(\)` will be rendered as formulas

## Technical Details

### UART Communication
- Uses `Ion::Console` API (115200 baud, 8N1)
- Non-blocking polling every 50ms
- 1024-byte circular buffer

### Math Rendering
- Supports LaTeX subset via Poincare engine
- Automatic parsing and layout generation
- Falls back to text for invalid expressions

### UI Components
- `StackViewController` for view management
- `TextView` with `ScrollableView` for text display
- `ExpressionView` for mathematical expressions

## API Corrections from Original

✅ **Fixed Serial API:**
- `Ion::Serial::*` → `Ion::Console::*`
- `#include <ion/console.h>`

✅ **Fixed ExpressionView:**
- `apps/shared/expression_view.h` → `escher/expression_view.h`
- `Shared::ExpressionView` → `ExpressionView`

✅ **Proper Omega Structure:**
- Added namespace `PiStream`
- Proper `App::Descriptor` and `App::Snapshot`
- MVC pattern implementation
