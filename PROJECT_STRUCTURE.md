# Project Structure

This document describes the organization and structure of the IBM ThinkPad A22M EEPROM Reader/Flasher project.

## Directory Structure

```
IbmThinkpad-EEPROM-ReaderFlasher/
├── .git/                 # Git version control
├── .pio/                 # PlatformIO build files (auto-generated)
├── .vscode/              # VS Code workspace settings
│   ├── c_cpp_properties.json
│   ├── keybindings.json
│   └── tasks.json
├── docs/                 # Documentation and images
│   ├── images/
│   │   └── 24rf08-pinout.gif
│   └── README.md
├── examples/             # Example code and configurations
│   └── README.md
├── src/                  # Main source code
│   └── main.cpp
├── tools/                # External tools and utilities
│   ├── setup_ibmpass22.7z
│   └── README.md
├── .gitignore           # Git ignore rules
├── platformio.ini       # PlatformIO configuration
├── PROJECT_STRUCTURE.md # This file
└── README.md           # Main project documentation
```

## File Descriptions

### Core Files
- **`README.md`** - Main project documentation with usage instructions
- **`platformio.ini`** - PlatformIO build configuration for Arduino
- **`.gitignore`** - Excludes build files and temporary data from version control

### Source Code (`src/`)
- **`main.cpp`** - Main Arduino sketch with EEPROM read/write functionality
  - Fully documented with inline comments
  - PlatformIO compatible with forward declarations
  - Serial command interface (r1-r4, ra, w1-w4, wa)

### Documentation (`docs/`)
- **`images/24rf08-pinout.gif`** - Hardware pinout reference
- **`README.md`** - Documentation standards and organization

### Tools (`tools/`)
- **`setup_ibmpass22.7z`** - IBMPASS password decoder tool
- **`README.md`** - Tool descriptions and usage instructions

### Examples (`examples/`)
- Reserved for future code examples and configurations
- Will contain sample usage patterns and test configurations

### Development (`/.vscode/`)
- **`tasks.json`** - Build, upload, and monitor tasks
- **`c_cpp_properties.json`** - IntelliSense configuration
- **`keybindings.json`** - Custom keyboard shortcuts

## Development Workflow

1. **Edit Code:** Modify `src/main.cpp` for functionality changes
2. **Build:** Use PlatformIO build (`Ctrl+Shift+P` → "PlatformIO: Build")
3. **Upload:** Flash to Arduino (`Ctrl+Shift+P` → "PlatformIO: Upload")
4. **Monitor:** Use serial monitor for testing commands
5. **Document:** Update README and docs as needed
6. **Commit:** Use Git to track changes

## Standards

### Code
- **Language:** C++ with Arduino framework
- **Style:** Detailed inline documentation
- **Compatibility:** PlatformIO and Arduino IDE compatible

### Documentation
- **Format:** Markdown with clear structure
- **Language:** English with technical precision
- **Images:** Stored in `docs/images/` with descriptive names

### Version Control
- **Branches:** Use feature branches for major changes
- **Commits:** Clear, descriptive commit messages
- **Releases:** Tag stable versions for distribution

## Future Expansion

This structure supports future additions:
- Multiple hardware targets (ESP32, other microcontrollers)
- Additional EEPROM types and password systems
- GUI tools and utilities
- Hardware designs (PCB layouts, 3D models)
- Test suites and validation tools