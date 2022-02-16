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
#include <stdio.h>
#include <string.h>
#include "include/vm.h"
#include "include/opcodes.h"

void vm_init(struct vm *VM, String program)
{
    uint16_t i = 0;
    int size = program.length();

    memset(VM, '\0', sizeof(struct vm));
    VM->registers.pc = 0x200;

    while (i < size && i < 0xFFF - 0x200) {
        int fH, fL;

        fH = program[i];
        if (fH == -1) break;
        VM->memory[VM->registers.pc + i] = fH;

        fL = program[i+1];
        if (fL == -1) break;
        VM->memory[VM->registers.pc + i + 1] = fL;

        i += 2;
    }
}

uint16_t calc_opcode(struct vm *VM)
{
    uint8_t fH = VM->memory[VM->registers.pc];
    uint8_t fL = VM->memory[VM->registers.pc + 1];
    return (fH << 8) | fL;
}

void vm_cycle(struct vm *VM)
{
    uint16_t opcode = calc_opcode(VM);
    VM->registers.pc += 2;
    parse_opcode(VM, opcode);
}
