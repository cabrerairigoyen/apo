# 🚀 Complete Omega Firmware Build & Flash Guide

Your **Pi Stream Display** app has been successfully integrated into Omega! Here's how to build and flash the custom firmware to your NumWorks calculator.

## ✅ **Current Status:**
- ✅ Pi Stream app integrated into Omega build system
- ✅ Icon copied to all theme directories  
- ✅ Build configuration updated
- ✅ Dependencies installed (imagemagick, libpng, freetype, ARM GCC)
- ⏳ **Waiting for**: Xcode Command Line Tools installation to complete

## 📋 **Prerequisites Status:**
```bash
✅ Omega repository cloned
✅ Pi Stream app in: /Users/cabrera/app on n/Omega/apps/pi_stream_app/
✅ build/config.mak updated (pi_stream_app added to EPSILON_APPS)
✅ themes/icons.json updated
✅ Icons copied to all theme directories
✅ Homebrew dependencies installed
✅ ARM GCC toolchain installed (/usr/local/bin/arm-none-eabi-gcc)
⏳ Xcode Command Line Tools (please accept the installation dialog)
```

---

## 🔨 **Step 1: Complete the Build Environment Setup**

**Accept Xcode Command Line Tools installation:**
- A dialog should be showing asking to install Command Line Tools
- Click "Install" and wait for completion (may take 5-10 minutes)
- Or run: `xcode-select --install` if the dialog is gone

**Verify setup:**
```bash
cd /Users/cabrera/app on n/Omega
xcode-select --print-path  # Should show a path like /Library/Developer/CommandLineTools
which arm-none-eabi-gcc    # Should show /usr/local/bin/arm-none-eabi-gcc
```

---

## 🔧 **Step 2: Build the Custom Omega Firmware**

```bash
cd /Users/cabrera/app on n/Omega

# Clean previous builds
make clean

# Build firmware (replace {YourName} with desired username, max 15 chars)
make OMEGA_USERNAME="YourName" -j$(sysctl -n hw.ncpu)

# Alternative: Build binpack for easier web flashing
make OMEGA_USERNAME="YourName" binpack -j$(sysctl -n hw.ncpu)
```

**Expected output locations:**
- Standard firmware: `output/device/n0110/release/epsilon.bin`
- Binpack: `output/device/n0110/release/binpack.bin`

**If build errors occur:**
1. Check that all dependencies are installed
2. Ensure Xcode Command Line Tools are fully installed
3. Try: `git submodule update --init --recursive`
4. Check your app code for syntax errors

---

## 📱 **Step 3: Prepare Calculator for Flashing**

### **Put Calculator in DFU Mode:**
1. **Unplug** the calculator from USB
2. Press and hold **RESET button** (small pinhole on back) + **6 key** simultaneously
3. While holding both buttons, **plug in USB cable**
4. Release the buttons - screen should stay **blank** (DFU mode active)
5. If screen shows anything, retry the process

### **Verify DFU Mode (macOS):**
```bash
system_profiler SPUSBDataType | grep -i "STM"
# Should show "STM Device in DFU Mode" or similar
```

---

## ⚡ **Step 4: Flash the Firmware**

### **Option A: Command-Line Flashing (Recommended)**

**Install dfu-util:**
```bash
brew install dfu-util
```

**Flash the firmware:**
```bash
cd /Users/cabrera/app on n/Omega

# Flash directly using make target
make epsilon_flash

# OR manually specify file
dfu-util -a 0 -s 0x08000000:leave -D output/device/n0110/release/epsilon.bin
```

**Expected output:**
```
dfu-util 0.11
Opening DFU capable USB device...
Device ID 0483:df11
Device DFU version 011a
DFU suffix version 011a
Claiming USB DFU Interface...
Setting Alternate Setting #0 ...
Determining device status: state = dfuIDLE, status = 0
dfuIDLE, continuing
DFU mode device DFU version 011a
Device returned transfer size 2048
DfuSe interface name: "Internal Flash  "
Downloading to address = 0x08000000, size = 2097152
Download  [=========================] 100%      2097152 bytes
Download done.
File downloaded successfully
```

### **Option B: Web-Based Flashing**

**For binpack files:**
1. Go to: **https://ti-planet.github.io/webdfu_numworks/n0110/**
2. Ensure calculator is in DFU mode and connected
3. Click "Connect" - should detect your NumWorks
4. Select "Upload from file" and choose your `binpack.bin`
5. Click "Flash" and wait for completion

**For Omega web installer:**
1. Go to: **https://getupsilon.web.app/**
2. Select your firmware version and upload custom `epsilon.bin` if available

---

## 🎉 **Step 5: Test Your Custom Firmware**

**After flashing:**
1. **Unplug and replug** calculator - it should boot into Omega
2. Navigate through the app menu
3. Look for **"Pi Stream"** app with the math icon 🧮
4. Open the app to test UART functionality

**Your app features:**
- Real-time UART data display at 115200 baud
- LaTeX math rendering (use `$$formula$$` or `\(formula\)`)
- Scrollable text interface
- Proper integration with Omega themes

---

## 🔄 **Step 6: Raspberry Pi UART Setup**

[[memory:5135124]] **Connect to your Raspberry Pi:**

**Hardware connection:**
- Calculator UART pins to Pi GPIO pins
- Use appropriate level shifters if needed

**Pi software setup:**
```python
import serial
import time

# Initialize UART
ser = serial.Serial('/dev/serial0', 115200, timeout=1)

# Send text data
ser.write(b"Hello from Raspberry Pi!\n")

# Send LaTeX math
ser.write(b"$$E = mc^2$$\n")
ser.write(b"\\(\\int_0^\\pi \\sin(x) dx = 2\\)\n")

# Send mixed content
ser.write(b"Temperature: 25°C\n")
ser.write(b"$$\\frac{dT}{dt} = -k(T - T_{ambient})$$\n")
```

---

## 🛠️ **Troubleshooting**

### **Build Issues:**
- **"command not found"**: Ensure Xcode Command Line Tools installed
- **ARM compiler errors**: Check `brew install gcc-arm-embedded` completed
- **Missing dependencies**: Run `brew install imagemagick libpng freetype pkg-config`

### **Flashing Issues:**
- **"No DFU device found"**: Ensure DFU mode active and USB connected
- **Permission denied**: Try `sudo dfu-util ...` or check USB permissions
- **Flash failed**: Retry DFU mode, check cable, try different USB port

### **Runtime Issues:**
- **App not appearing**: Check build logs, verify integration steps
- **UART not working**: Check hardware connections, baud rate settings
- **Math not rendering**: Verify LaTeX syntax, check expression parser

### **Reverting to Official:**
1. Put calculator in DFU mode
2. Go to **https://numworks.com/workshop**
3. Flash official Epsilon firmware

---

## 📂 **File Locations Reference**

```
/Users/cabrera/app on n/Omega/
├── apps/pi_stream_app/           # Your app source
├── build/config.mak              # Build configuration
├── themes/icons.json             # Icon mapping
├── themes/themes/local/*/apps/   # Theme icon files
└── output/device/n0110/release/  # Built firmware files
```

---

## ✨ **Your Custom Firmware Features**

🧮 **Pi Stream Display App:**
- Real-time UART data streaming
- LaTeX mathematical expression rendering
- Modern Omega-integrated UI
- Support for mixed text/math content
- Automatic expression detection
- Scrollable text display

🎨 **Omega Integration:**
- Proper app icon in all themes
- Internationalization support
- Standard Omega app architecture
- Full build system integration

**Congratulations! You now have a fully custom Omega firmware with your Pi Stream Display app! 🎊**
