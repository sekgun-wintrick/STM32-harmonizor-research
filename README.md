# STM32 Neural Harmonizer Research Platform

## Goal
An open-source development log for a guitar harmonizer research platform, aiming to integrate neural control (EMG/EEG) in the future.

## Current Status
- [x] Basic UART communication tested
- [x] Custom power supply module built
- [x] ADC reading from potentiometer & rotary switch
- [x] DAC output basic waveforms tested
- [ ] Install CMSIS-DSP library
- [ ] Implement circular buffer for real-time audio
- [ ] Create simple pitch-shift algorithm

## Hardware
- MCU: STM32F407vet6
- Power supply: 5V 5600mAh & AMS1117 3.3V module
- Pre amp: planned
- Sensors: EMG

## Folder Structure
- `/firmware` - STM32 source code
- `/hardware` - Schematics, breadboard photos
- `/docs` - Learning notes, references

## Why This Project?
I'm a Year 2 student at HKUST who wants to become a scientist/inventor. This project documents my learning journey from basic STM32 to real-time DSP, and eventually to neural interfaces. All feedback welcome.

## License
MIT
