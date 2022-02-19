evm-esp32
=========

A virtual machine for ESP32, allowing dynamic program updates.

Based on my [implementation of CHIP-8](https://github.com/bor0/chip-8), where the gaming related instructions are removed, and instead instructions for IO pin controlling are added.

The following instructions that are related to games (drawing, etc.) are removed:

- `00E0`
- `DXYN`
- `EX9E`
- `EXA1`
- `FX0A`
- `FX18`
- `FX29`

The following instructions that are related to IO control are added:

- `E0A0` - sleep for second register's value milliseconds
- `EXA1` - sets the pin mode X to the pin (value in first register)
- `E0A2` - does `analogRead` to the first register and stores the result in the second register
- `E0A3` - does `analogWrite` to the first register using the value from the second register

Registers are changed from 8 bit to 16 bit.

In order to reload a program, use the `0000` instruction to halt the VM and restart.

Tested on ESP32 Arduino (WROOM32).

References:

- http://en.wikipedia.org/wiki/CHIP-8
- https://github.com/bor0/chip-8

Boro Sitnikovski
