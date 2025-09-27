# Tools Directory

This directory contains external tools and utilities required for the IBM ThinkPad A22M EEPROM project.

## Contents

### IBMPASS Tool (`setup_ibmpass22.7z`)

**Purpose:** Decode BIOS supervisor passwords from extracted EEPROM data.

**Description:** 
The IBMPASS tool is essential for converting the raw EEPROM password data into readable supervisor passwords for IBM ThinkPad laptops.

**Usage:**
1. Extract the EEPROM data using the Arduino tool
2. Run IBMPASS with the extracted data
3. Obtain the decoded supervisor password

**Installation:**
1. Extract `setup_ibmpass22.7z` 
2. Follow the included installation instructions
3. Run the tool with your EEPROM dump data

**Compatibility:**
- Windows systems
- IBM ThinkPad A22M and related models
- AT24RF08 EEPROM password data

**Legal Notice:**
This tool is for educational and legitimate recovery purposes only. Use only on systems you own or have explicit permission to access.

## Adding New Tools

When adding new tools to this directory:

1. **Archive Format:** Use `.zip` or `.7z` for compression
2. **Documentation:** Update this README with tool descriptions  
3. **Naming Convention:** Use descriptive names (e.g., `toolname_v1.2.zip`)
4. **Subdirectories:** Create subdirectories for complex tool sets
5. **Licenses:** Include license information for each tool

## Security Note

⚠️ **Important:** All tools in this directory should be scanned for malware before use. Only download tools from trusted sources and verify checksums when available.