# CHIP-8-Emulator
Simple Chip8-Emulator written in modern C++ with SDL3

ToDo:
-Sound System
-Keyboard Mapping for all Layouts(Currently it is only German Layout)

### Keyboard Layout

The emulator maps your PC keyboard to the original CHIP-8 hexadecimal keypad (0-F) as follows:

| PC Key | CHIP-8 Key | | PC Key | CHIP-8 Key |
| :---: | :---: | :---: | :---: | :---: |
| **1** | 0x1 | | **A** | 0x7 |
| **2** | 0x2 | | **S** | 0x8 |
| **3** | 0x3 | | **D** | 0x9 |
| **4** | 0xC | | **F** | 0xE |
| **Q** | 0x4 | | **Y** | 0xA |
| **W** | 0x5 | | **X** | 0x0 |
| **E** | 0x6 | | **C** | 0xB |
| **R** | 0xD | | **V** | 0xF |

**Layout Visualization:**
[1] [2] [3] [C]
[Q] [W] [E] [D]
[A] [S] [D] [E]
[Y] [X] [C] [F]

## Third-Party Libraries
- [SDL3](https://github.com/libsdl-org/SDL) — zlib

## License
This project is licensed under the GNU General Public License v3.0 (GPLv3)