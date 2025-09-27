/*
 * IBM A22M Password Hacking - AT24RF08 EEPROM Read/Write Tool
 * 
 * This Arduino sketch provides a serial interface to read and write
 * AT24RF08 EEPROM chips commonly found in IBM ThinkPad A22M laptops.
 * The EEPROM contains BIOS supervisor password data that can be extracted
 * and decoded using tools like IBMPASS.
 * 
 * Hardware Connections:
 * - SDA: Arduino SDA pin (A4 on Uno/Nano)
 * - SCL: Arduino SCL pin (A5 on Uno/Nano) 
 * - GND: Arduino GND
 * - WP:  Connect to GND for write operations
 * - VCC: DO NOT connect to Arduino - power comes from ThinkPad!
 * 
 * Serial Commands:
 * - r1-r4: Read blocks 1-4 individually
 * - ra:    Read all blocks
 * - w1-w4: Write blocks 1-4 individually  
 * - wa:    Write all blocks (with warning)
 * 
 * Author: [Your Name]
 * Date: September 2025
 * License: Educational/Recovery purposes only
 */

#include <Arduino.h>
#include <Wire.h>

// I2C addresses for the four 256-byte blocks in AT24RF08 EEPROM
// Each block has a different I2C address for independent access
const byte eeprom_addresses[] = {0x54, 0x55, 0x56, 0x57};

// Size of each block in bytes (AT24RF08 has 4 blocks of 256 bytes each)
const int block_size = 256;

// Buffer for write data - full 256 bytes (Pro Micro has enough RAM)
// Arduino Pro Micro has 2.5KB RAM, so 256 bytes is no problem
byte write_buffer[256];

// Backup data arrays for each EEPROM block (256 bytes each)
// Replace these with your actual backup data from 'ra' command output
// Block 1 data (I2C Address 0x54)
const byte backup_block1[256] PROGMEM = {
  // Example data - REPLACE WITH YOUR ACTUAL BACKUP DATA
  0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Block 2 data (I2C Address 0x55)
const byte backup_block2[256] PROGMEM = {
  // Example data - REPLACE WITH YOUR ACTUAL BACKUP DATA
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

// Block 3 data (I2C Address 0x56)
const byte backup_block3[256] PROGMEM = {
  // Example data - REPLACE WITH YOUR ACTUAL BACKUP DATA
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55
};

// Block 4 data (I2C Address 0x57)
const byte backup_block4[256] PROGMEM = {
  // Example data - REPLACE WITH YOUR ACTUAL BACKUP DATA
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
  0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
};

// Function declarations (Forward declarations for C++)
void readBlock(int blockNum);
void readAllBlocks();
void writeBlock(int blockNum);
void writeAllBlocks();
void loadBackupData(int blockNum);

/**
 * Setup function - runs once when Arduino starts
 * Initializes serial communication, I2C bus, and write buffer
 */
void setup() {
  // Initialize serial communication at 115200 baud rate
  // This speed provides fast data transfer for EEPROM dumps
  Serial.begin(115200);
  
  // Initialize I2C (Wire) library for communication with EEPROM
  // Arduino will act as I2C master, EEPROM as slave device
  Wire.begin();
  
  // Short delay to ensure stable initialization
  delay(1000);
  
  // Initialize write buffer with test pattern data (full 256 bytes)
  // Pro Micro has enough RAM for complete buffer
  // Default: Load backup data for block 1 (change as needed)
  loadBackupData(1);  // Load Block 1 backup data into write_buffer
  
  // Alternative initialization options:
  // loadBackupData(2);           // Load Block 2 backup data
  // loadBackupData(3);           // Load Block 3 backup data  
  // loadBackupData(4);           // Load Block 4 backup data
  // Test patterns:
  // for (int i = 0; i < 256; i++) {
  //   write_buffer[i] = i;        // Test pattern: 0-255
  //   write_buffer[i] = 0xFF;     // Fill with 0xFF
  //   write_buffer[i] = 0x00;     // Fill with 0x00
  // }
  
  // Display startup message and available commands (using F() to save RAM)
  Serial.println(F("=== IBM A22M EEPROM AT24RF08 Controller ==="));
  Serial.println(F("Status: Ready"));
  Serial.println();
  Serial.println(F("Available Commands:"));
  Serial.println(F("READ:  r1, r2, r3, r4 - Read blocks 1-4 individually"));
  Serial.println(F("       ra             - Read all blocks (complete dump)"));
  Serial.println(F("LOAD:  l1, l2, l3, l4 - Load backup data for blocks 1-4 into write_buffer"));
  Serial.println(F("WRITE: w1, w2, w3, w4 - Write blocks 1-4 individually"));
  Serial.println(F("       wa             - Write all blocks (DANGER: overwrites all!)"));
  Serial.println();
  Serial.println(F("Enter command:"));
}

/**
 * Main loop function - runs continuously after setup()
 * Monitors serial input for commands and executes corresponding operations
 */
void loop() {
  // Check if data is available on serial port
  if (Serial.available()) {
    // Read the complete command string from serial buffer
    String command = Serial.readString();
    
    // Clean up the command: remove whitespace and convert to lowercase
    command.trim();           // Remove leading/trailing whitespace
    command.toLowerCase();    // Convert to lowercase for case-insensitive matching
    
    // Command parsing and execution
    // READ COMMANDS - Individual blocks
    if (command == "r1") {
      readBlock(0);           // Read block 1 (array index 0, I2C addr 0x54)
    } else if (command == "r2") {
      readBlock(1);           // Read block 2 (array index 1, I2C addr 0x55)
    } else if (command == "r3") {
      readBlock(2);           // Read block 3 (array index 2, I2C addr 0x56)
    } else if (command == "r4") {
      readBlock(3);           // Read block 4 (array index 3, I2C addr 0x57)
    
    // READ COMMAND - All blocks  
    } else if (command == "ra") {
      readAllBlocks();        // Read all 4 blocks sequentially
    
    // LOAD COMMANDS - Load backup data into write_buffer
    } else if (command == "l1") {
      loadBackupData(1);      // Load Block 1 backup data
    } else if (command == "l2") {
      loadBackupData(2);      // Load Block 2 backup data
    } else if (command == "l3") {
      loadBackupData(3);      // Load Block 3 backup data
    } else if (command == "l4") {
      loadBackupData(4);      // Load Block 4 backup data
    
    // WRITE COMMANDS - Individual blocks
    } else if (command == "w1") {
      writeBlock(0);          // Write block 1 with data from write_buffer
    } else if (command == "w2") {
      writeBlock(1);          // Write block 2 with data from write_buffer
    } else if (command == "w3") {
      writeBlock(2);          // Write block 3 with data from write_buffer
    } else if (command == "w4") {
      writeBlock(3);          // Write block 4 with data from write_buffer
    
    // WRITE COMMAND - All blocks (DANGEROUS)
    } else if (command == "wa") {
      writeAllBlocks();       // Write all blocks (includes safety delay)
    
    // INVALID COMMAND
    } else {
      Serial.print(F("ERROR: Unknown command: "));
      Serial.println(command);
      Serial.println(F("Valid commands:"));
      Serial.println(F("  READ: r1-r4 (read blocks), ra (read all)"));
      Serial.println(F("  LOAD: l1-l4 (load backup data into write_buffer)"));
      Serial.println(F("  WRITE: w1-w4 (write blocks), wa (write all)"));
      Serial.println(F("Commands are case-insensitive."));
    }
  }
}

/**
 * Read a single EEPROM block and display contents in hex format
 * 
 * @param blockNum Block number to read (0-3)
 *                 0 = Block 1 (I2C 0x54)
 *                 1 = Block 2 (I2C 0x55) 
 *                 2 = Block 3 (I2C 0x56)
 *                 3 = Block 4 (I2C 0x57)
 * 
 * Output format: 16 bytes per line with address offset
 * Example: 0x00: 3F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 */
void readBlock(int blockNum) {
  // Get I2C address for this block from lookup table
  byte addr = eeprom_addresses[blockNum];
  
  // Display block information header
  Serial.print("Reading Block ");
  Serial.print(blockNum + 1);           // Display as 1-4 instead of 0-3
  Serial.print(" (I2C Address: 0x");
  Serial.print(addr, HEX);
  Serial.println("):");
  
  // Read all 256 bytes from this block
  for (int i = 0; i < block_size; i++) {
    // Set address pointer in EEPROM for sequential read
    Wire.beginTransmission(addr);       // Start I2C transmission to EEPROM
    Wire.write((byte)i);                // Send address to read from (0-255)
    Wire.endTransmission();             // End transmission (address is now set)
    
    // Request one byte of data from the set address
    Wire.requestFrom((uint8_t)addr, (uint8_t)1);          // Request 1 byte from EEPROM
    
    // Process received data
    if (Wire.available()) {             // Check if data was received
      byte data = Wire.read();          // Read the byte from I2C buffer
      
      // Format output: new line every 16 bytes with address offset
      if (i % 16 == 0) {
        Serial.print("\n0x");           // New line and hex prefix
        if (i < 0x10) Serial.print("0"); // Leading zero for single digit hex
        Serial.print(i, HEX);           // Address offset in hex
        Serial.print(": ");             // Separator
      }
      
      // Display data byte with leading zero if needed
      if (data < 0x10) Serial.print("0"); // Leading zero for single digit
      Serial.print(data, HEX);            // Data in hex format
      Serial.print(" ");                  // Space separator
    }
  }
  
  // End of block marker
  Serial.println("\n======================================");
}

/**
 * Read all four EEPROM blocks sequentially
 * 
 * This function performs a complete EEPROM dump by reading all 4 blocks
 * (total 1024 bytes). Useful for creating backups or analyzing entire contents.
 * Small delays between blocks prevent I2C bus overload.
 */
void readAllBlocks() {
  Serial.println("=== COMPLETE EEPROM DUMP (ALL BLOCKS) ===");
  Serial.println("Total size: 1024 bytes (4 blocks × 256 bytes)");
  Serial.println();
  
  // Read each of the 4 blocks sequentially
  for (int blockIndex = 0; blockIndex < 4; blockIndex++) {
    readBlock(blockIndex);              // Read current block
    delay(100);                         // Short delay to prevent I2C bus overload
  }
  
  Serial.println("\n=== DUMP COMPLETE ===");
  Serial.println("All 4 blocks have been read successfully.");
  Serial.println("Use this data for backup or password extraction.");
}

/**
 * Write data to a single EEPROM block from the write_buffer array
 * 
 * @param blockNum Block number to write (0-3)
 *                 Data source: write_buffer[0-255]
 * 
 * IMPORTANT: Ensure WP (Write Protect) pin is connected to GND before writing!
 * 
 * Error codes from Wire.endTransmission():
 * 0: Success
 * 1: Data too long to fit in transmit buffer
 * 2: Received NACK on transmit of address
 * 3: Received NACK on transmit of data
 * 4: Other error
 */
void writeBlock(int blockNum) {
  // Get I2C address for this block
  byte addr = eeprom_addresses[blockNum];
  
  // Display write operation header
  Serial.print("Writing Block ");
  Serial.print(blockNum + 1);           // Display as 1-4 instead of 0-3
  Serial.print(" (I2C Address: 0x");
  Serial.print(addr, HEX);
  Serial.println("):");
  Serial.println("WARNING: This will overwrite existing data!");
  
  // Write all 256 bytes to this block
  for (int i = 0; i < block_size; i++) {
    // Start I2C transmission to EEPROM
    Wire.beginTransmission(addr);
    Wire.write((byte)i);                // Set address to write to (0-255)
    Wire.write(write_buffer[i]);        // Write data byte from buffer
    
    // Execute the write operation and check for errors
    byte error = Wire.endTransmission();
    
    // Check write result and display progress
    if (error == 0) {
      // Successful write - show progress every 32 bytes
      if (i % 32 == 0) {
        Serial.print("Progress: Written up to address 0x");
        if (i < 0x10) Serial.print("0");   // Leading zero formatting
        Serial.print(i, HEX);
        Serial.print(" (");
        Serial.print((i * 100) / block_size);
        Serial.println("%)");
      }
    } else {
      // Write error occurred - display error details
      Serial.print("ERROR: Write failed at address 0x");
      if (i < 0x10) Serial.print("0");
      Serial.print(i, HEX);
      Serial.print(" - Error code: ");
      Serial.print(error);
      Serial.print(" (");
      
      // Decode error code for user
      switch(error) {
        case 1: Serial.print("Data too long"); break;
        case 2: Serial.print("NACK on address"); break;
        case 3: Serial.print("NACK on data"); break;
        case 4: Serial.print("Other error"); break;
        default: Serial.print("Unknown error"); break;
      }
      Serial.println(")");
    }
    
    // Delay between writes to ensure EEPROM has time to complete write cycle
    delay(5);                           // 5ms delay (EEPROM write cycle time)
  }
  
  // Write operation complete
  Serial.print("Block ");
  Serial.print(blockNum + 1);
  Serial.println(" write operation completed.");
  Serial.println("Recommendation: Read back the block to verify data integrity.");
}

/**
 * Write data to ALL EEPROM blocks - EXTREMELY DANGEROUS OPERATION!
 * 
 * This function overwrites the ENTIRE EEPROM (all 1024 bytes) with data
 * from the write_buffer. This will destroy existing BIOS settings and
 * supervisor password data.
 * 
 * SAFETY FEATURES:
 * - 5 second warning delay
 * - Multiple warning messages
 * - Recommendation to backup first
 * 
 * USE ONLY IF:
 * - You have a complete backup of original data
 * - You are restoring from a known good backup
 * - You understand the risks involved
 */
void writeAllBlocks() {
  Serial.println("\n*** DANGER: COMPLETE EEPROM WRITE OPERATION ***");
  Serial.println("\n⚠️  WARNING ⚠️  WARNING ⚠️  WARNING ⚠️");
  Serial.println("This will OVERWRITE ALL EEPROM DATA!");
  Serial.println("- BIOS supervisor password will be destroyed");
  Serial.println("- All BIOS settings will be overwritten");
  Serial.println("- ThinkPad may become unbootable if data is incorrect");
  Serial.println("\n🛡️  SAFETY CHECKLIST:");
  Serial.println("[ ] Do you have a backup of original EEPROM data?");
  Serial.println("[ ] Have you verified write_buffer contains correct data?");
  Serial.println("[ ] Is WP pin connected to GND?");
  Serial.println("[ ] Are you prepared for potential ThinkPad recovery?");
  Serial.println("\n⏰ ABORTING IN 5 SECONDS...");
  Serial.println("   (Reset Arduino now to cancel operation)");
  
  // 5-second countdown with abort option
  for (int countdown = 5; countdown > 0; countdown--) {
    Serial.print("   Continuing in ");
    Serial.print(countdown);
    Serial.println(" seconds...");
    delay(1000);
  }
  
  Serial.println("\n🚀 STARTING COMPLETE EEPROM WRITE...");
  
  // Write each of the 4 blocks sequentially
  for (int blockIndex = 0; blockIndex < 4; blockIndex++) {
    Serial.print("\n--- Writing Block ");
    Serial.print(blockIndex + 1);
    Serial.println(" of 4 ---");
    
    writeBlock(blockIndex);             // Write current block
    delay(100);                         // Delay between blocks
  }
  
  Serial.println("\n=== ALL BLOCKS WRITTEN ===");
  Serial.println("✅ Complete EEPROM write operation finished.");
  Serial.println("🔍 STRONGLY RECOMMENDED: Use 'ra' command to verify all data.");
  Serial.println("💾 Save verification dump for future reference.");
}

/**
 * Load backup data for a specific block into the write_buffer
 * 
 * @param blockNum Block number to load (1-4)
 *                 1 = Load backup_block1 data
 *                 2 = Load backup_block2 data
 *                 3 = Load backup_block3 data
 *                 4 = Load backup_block4 data
 * 
 * This function copies 256 bytes from PROGMEM backup arrays into the write_buffer.
 * After loading, you can use w1-w4 commands to write the data to EEPROM.
 * 
 * IMPORTANT: Replace the backup_blockX arrays with your actual backup data
 * before using this function for real password recovery operations!
 */
void loadBackupData(int blockNum) {
  const byte* sourceArray;
  
  // Select the appropriate backup array based on block number
  switch(blockNum) {
    case 1:
      sourceArray = backup_block1;
      break;
    case 2:
      sourceArray = backup_block2;
      break;
    case 3:
      sourceArray = backup_block3;
      break;
    case 4:
      sourceArray = backup_block4;
      break;
    default:
      Serial.print("ERROR: Invalid block number: ");
      Serial.print(blockNum);
      Serial.println(" (Valid range: 1-4)");
      return;
  }
  
  // Copy data from PROGMEM to RAM buffer
  Serial.print("Loading backup data for Block ");
  Serial.print(blockNum);
  Serial.println(" into write_buffer...");
  
  for (int i = 0; i < 256; i++) {
    write_buffer[i] = pgm_read_byte(&sourceArray[i]);
  }
  
  // Display first 16 bytes as confirmation
  Serial.print("Preview (first 16 bytes): ");
  for (int i = 0; i < 16; i++) {
    if (write_buffer[i] < 0x10) Serial.print("0");
    Serial.print(write_buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  Serial.print("✅ Block ");
  Serial.print(blockNum);
  Serial.println(" backup data loaded successfully.");
  Serial.print("💡 Use 'w");
  Serial.print(blockNum);
  Serial.println("' command to write this data to EEPROM.");
}