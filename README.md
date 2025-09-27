# IBM A22M Password Hacking - EEPROM Restore Tool

This project provides an Arduino-based tool to read and write the AT24RF08 EEPROM chip found in IBM ThinkPad A22M laptops. This EEPROM contains the BIOS supervisor password that can be extracted and decoded.

## Hardware Information

### AT24RF08 EEPROM Chip Pinouts

![AT24RF08 Pinout](24rf08%20pinout.gif)

#### 8-Pin AT24RF08 Package (SOIC-8)
```
Pin 4: GND     - Ground connection
Pin 5: SDA     - I2C Data line
Pin 6: SCL     - I2C Clock line
Pin 7: WP      - Write Protect (connect to GND for write operations)
```

#### 14-Pin AT24RF08 Package (SOIC-14)
```
Pin 6:  GND    - Ground connection
Pin 8:  SDA    - I2C Data line
Pin 10: SCL    - I2C Clock line
Pin 11: WP     - Write Protect (connect to GND for write operations)
```

### Arduino Connection

Connect the EEPROM to your Arduino as follows:
- **SDA** → Arduino SDA (Pin A4 on Uno/Nano, Pin 20 on Mega)
- **SCL** → Arduino SCL (Pin A5 on Uno/Nano, Pin 21 on Mega)
- **GND** → Arduino GND
- **WP** → GND (for write operations) or leave floating (for read-only)

**⚠️ IMPORTANT:** Do NOT connect VCC through the Arduino! The power supply comes from the ThinkPad itself when operational.

## Setup Instructions

1. **Hardware Connection**
   - Connect Arduino to the AT24RF08 EEPROM chip (without VCC connection)
   - Ensure WP pin is connected to GND if you plan to write data
   
2. **ThinkPad Operation**
   - Power on the ThinkPad (this provides power to the EEPROM)
   - The EEPROM is now ready for read/write operations

3. **Software Setup**
   - Upload the Arduino sketch to your board
   - Open Serial Monitor at 115200 baud rate

## Usage Instructions

### Available Commands

Once the Arduino is connected and the sketch is running, you can use the following commands in the Serial Monitor:

#### Read Operations
- `r1` - Read Block 1 (I2C address 0x54)
- `r2` - Read Block 2 (I2C address 0x55)
- `r3` - Read Block 3 (I2C address 0x56)
- `r4` - Read Block 4 (I2C address 0x57)
- `ra` - Read All blocks (1-4)

#### Write Operations
- `w1` - Write Block 1
- `w2` - Write Block 2
- `w3` - Write Block 3
- `w4` - Write Block 4
- `wa` - Write All blocks (includes 5-second warning)

### Example Usage

1. Start by reading all blocks to backup current data:
   ```
   ra
   ```

2. Read a specific block:
   ```
   r1
   ```

3. Write to a specific block (after modifying write_buffer in code):
   ```
   w1
   ```

### Data Format

The data is displayed in hexadecimal format, with 16 bytes per line:
```
0x00: 3F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0x10: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
...
```

## Password Decoding

### IBMPASS Tool

Use the **IBMPASS** tool to decode the BIOS supervisor password from the extracted EEPROM data.

1. Extract the EEPROM data using this Arduino tool
2. Locate the password data in the hex dump
3. Use IBMPASS to decode the supervisor password
4. The decoded password can be used to access BIOS settings

## Technical Details

- **EEPROM Type:** AT24RF08 (8Kbit/1KB total, organized as 4 blocks of 256 bytes each)
- **I2C Addresses:** 0x54, 0x55, 0x56, 0x57
- **Block Size:** 256 bytes per block
- **Total Capacity:** 1024 bytes (4 × 256 bytes)
- **Communication Protocol:** I2C (TWI)

## Safety Notes

⚠️ **Important Safety Information:**
- Always backup original EEPROM content before writing
- Incorrect data can potentially damage the ThinkPad's BIOS
- Test read operations first before attempting writes
- Keep original data backup in multiple locations
- The 5-second warning for write-all operations is intentional - use it wisely

## Troubleshooting

### Common Issues
1. **No response from EEPROM:** Check I2C connections (SDA/SCL)
2. **Write operations fail:** Ensure WP pin is connected to GND
3. **Inconsistent reads:** Check power supply and ground connections
4. **Arduino not responding:** Verify Serial Monitor baud rate (115200)

### Verification
- Always read back written data to verify successful writes
- Compare checksums before and after operations
- Use the `ra` command to get complete dumps for verification

## License

This project is for educational and recovery purposes only. Use responsibly and in accordance with local laws and regulations.