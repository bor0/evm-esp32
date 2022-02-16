/*
This file is part of evm-arduino.

evm-arduino is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

evm-arduino is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with evm-arduino. If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef VM_H
#define VM_H
#include <HTTPClient.h>
#include <stdint.h>
#include <stdio.h>

struct vm {
    struct {
        /* 16 16-bit data registers named from V0 to VF. The VF register doubles as a carry flag. */
        uint16_t v[16];

        /* The address register is 16 bits wide and is used with several opcodes that involve memory operations. */
        uint16_t I;

        /* Program counter. */
        uint16_t pc;

        /* Stack pointer. */
        uint16_t sp;
    } registers;

    /* CHIP-8's memory addresses range from 200h to FFFh, making for 3,584 bytes. */
    uint8_t memory[0x0FFF];

    /* The original 1802 version allocated 48 bytes for up to 12 levels of nesting; modern implementations normally have at least 16 levels. */
    uint16_t stack[16];

    /* Halt variable. */
    uint8_t halt;

    /* Timers. */
    uint8_t delay_timer;
};

void vm_init(struct vm *, String);
uint16_t calc_opcode(struct vm *);
void vm_cycle(struct vm *);
#endif
