# Documentation Directory

This directory contains all project documentation, images, diagrams, and reference materials.

## Structure

```
docs/
├── images/           # Hardware diagrams, pinouts, photos
│   └── 24rf08-pinout.gif
├── hardware/         # Hardware-specific documentation
├── software/         # Software guides and references  
└── README.md         # This file
```

## Contents

### Images (`images/`)

- **`24rf08-pinout.gif`** - AT24RF08 EEPROM chip pinout diagram
  - Shows 8-pin and 14-pin package pinouts
  - Essential reference for hardware connections
  - Used in main project README

### Hardware Documentation (`hardware/`)
*Future location for:*
- Detailed connection diagrams
- PCB layouts (if created)
- Component specifications
- Troubleshooting guides

### Software Documentation (`software/`)
*Future location for:*
- Detailed code explanations
- API documentation
- Algorithm descriptions  
- Development notes

## Contributing Documentation

When adding new documentation:

1. **Images:** Use standard formats (PNG, JPG, GIF, SVG)
2. **Naming:** Use descriptive, lowercase names with hyphens
3. **Size:** Optimize images for web viewing
4. **Formats:** Prefer markdown (.md) for text documentation
5. **Structure:** Maintain the directory organization

## Documentation Standards

- **Language:** English (with German comments in code if needed)
- **Format:** Markdown with clear headings and structure
- **Images:** Include alt-text for accessibility
- **Links:** Use relative paths for internal references
- **Updates:** Keep documentation in sync with code changes