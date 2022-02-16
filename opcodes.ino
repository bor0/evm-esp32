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
#include "include/opcodes.h"

void parse_opcode(struct vm *VM, uint16_t opcode)
{
    uint8_t i, vx, vy;

    switch (opcode) {
    case 0x00EE:
        /* Returns from a subroutine. */
        VM->registers.sp--;
        if (VM->registers.sp < 0) {
            printf("Fatal error: Stack underflow\n");
            VM->halt = 1;
            return;
        }
        VM->registers.pc = VM->stack[VM->registers.sp];
        return;
        break;
    case 0xE0A2:
        /* Read from input (first register) into output (second register) */
        VM->registers.v[1] = analogRead(VM->registers.v[0]);
        return;
        break;
    case 0xE0A3:
        /* Write to pin (first register) a value (second register) */
        digitalWrite(VM->registers.v[0], VM->registers.v[1]);
        return;
        break;
    case 0xE0A4:
        delay(VM->registers.v[2]);
        return;
        break;
    default:
        break;
    }

    switch (opcode & 0xF000) {
    case 0x1000:
        /* Jumps to address NNN. */
        VM->registers.pc = opcode & 0x0FFF;
        return;
        break;
    case 0x2000:
        /* Calls subroutine at NNN. */
        if (VM->registers.sp > sizeof(VM->stack) / sizeof(uint16_t)) {
            printf("Fatal error: Stack overflow\n");
            VM->halt = 1;
        }
        VM->stack[VM->registers.sp] = VM->registers.pc;
        VM->registers.sp++;
        VM->registers.pc = opcode & 0x0FFF;
        return;
        break;
    case 0x3000:
        /* Skips the next instruction if VX equals NN. */
        if (VM->registers.v[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
            VM->registers.pc += 2;
        }
        return;
        break;
    case 0x4000:
        /* Skips the next instruction if VX doesn't equal NN. */
        if (VM->registers.v[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
            VM->registers.pc += 2;
        }
        return;
        break;
    case 0x6000:
        /* Sets VX to NN. */
        VM->registers.v[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        return;
        break;
    case 0x7000:
        /* Adds NN to VX. */
        VM->registers.v[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
        return;
        break;
    case 0xA000:
        /* Sets I to the address NNN. */
        VM->registers.I = opcode & 0x0FFF;
        return;
        break;
    case 0xB000:
        /* Jumps to the address NNN plus V0. */
        VM->registers.pc = (opcode & 0x0FFF) + VM->registers.v[0];
        return;
        break;
    case 0xC000:
        /* Sets VX to a random number and NN. */
        VM->registers.v[(opcode & 0x0F00) >> 8] = (clock() % 255) & (opcode & 0x00FF);
        return;
        break;
    default:
        break;
    }

    switch (opcode & 0xF00F) {
    case 0x5000:
        /* Skips the next instruction if VX equals VY. */
        if (VM->registers.v[(opcode & 0x0F00) >> 8] == VM->registers.v[(opcode & 0x00F0) >> 4]) {
            VM->registers.pc += 2;
        }
        return;
        break;
    case 0x8000:
        /* Sets VX to the value of VY. */
        VM->registers.v[(opcode & 0x0F00) >> 8] = VM->registers.v[(opcode & 0x00F0) >> 4];
        return;
        break;
    case 0x8001:
        /* Sets VX to VX or VY. */
        VM->registers.v[(opcode & 0x0F00) >> 8] |= VM->registers.v[(opcode & 0x00F0) >> 4];
        return;
        break;
    case 0x8002:
        /* Sets VX to VX and VY. */
        VM->registers.v[(opcode & 0x0F00) >> 8] &= VM->registers.v[(opcode & 0x00F0) >> 4];
        return;
        break;
    case 0x8003:
        /* Sets VX to VX xor VY. */
        VM->registers.v[(opcode & 0x0F00) >> 8] ^= VM->registers.v[(opcode & 0x00F0) >> 4];
        return;
        break;
    case 0x8004:
        /* Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't. */
        vx = VM->registers.v[(opcode & 0x0F00) >> 8];
        vy = VM->registers.v[(opcode & 0x00F0) >> 4];

        if (vx + vy > 0xFF) {
            VM->registers.v[15] = 1;
        } else {
            VM->registers.v[15] = 0;
        }

        VM->registers.v[(opcode & 0x0F00) >> 8] += vy;
        return;
        break;
    case 0x8005:
        /* VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't. */
        vx = VM->registers.v[(opcode & 0x0F00) >> 8];
        vy = VM->registers.v[(opcode & 0x00F0) >> 4];

        if (vx - vy >= 0) {
            VM->registers.v[15] = 1;
        } else {
            VM->registers.v[15] = 0;
        }

        VM->registers.v[(opcode & 0x0F00) >> 8] -= vy;
        return;
        break;
    case 0x8006:
        /* Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift. */
        VM->registers.v[15] = VM->registers.v[(opcode & 0x0F00) >> 8] & 1;
        VM->registers.v[(opcode & 0x0F00) >> 8] >>= 1;
        return;
        break;
    case 0x8007:
        /* Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't. */
        vx = VM->registers.v[(opcode & 0x0F00) >> 8];
        vy = VM->registers.v[(opcode & 0x00F0) >> 4];

        if (vy - vx >= 0) {
            VM->registers.v[15] = 1;
        } else {
            VM->registers.v[15] = 0;
        }

        VM->registers.v[(opcode & 0x0F00) >> 8] = vy - VM->registers.v[(opcode & 0x0F00) >> 8];
        return;
        break;
    case 0x800E:
        /* Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift. */
        VM->registers.v[15] = (VM->registers.v[(opcode & 0x0F00) >> 8] >> 7) & 0x80;
        VM->registers.v[(opcode & 0x0F00) >> 8] <<= 1;
        return;
        break;
    case 0x9000:
        /* Skips the next instruction if VX doesn't equal VY. */
        if (VM->registers.v[(opcode & 0x0F00) >> 8] != VM->registers.v[(opcode & 0x00F0) >> 4]) {
            VM->registers.pc += 2;
        }
        return;
        break;
    default:
        break;
    }

    switch (opcode & 0xF0FF) {
    case 0xE0A1:
        /* Sets pinmode */
        pinMode(VM->registers.v[0], (opcode & 0x0F00) >> 8);
        return;
        break;
    case 0xF007:
        /* Sets VX to the value of the delay timer. */
        VM->registers.v[(opcode & 0x0F00) >> 8] = VM->delay_timer;
        return;
        break;
    case 0xF015:
        /* Sets the delay timer to VX. */
        VM->delay_timer = VM->registers.v[(opcode & 0x0F00) >> 8];
        return;
        break;
    case 0xF01E:
        /* Adds VX to I. */
        if ((VM->registers.I += VM->registers.v[(opcode & 0x0F00) >> 8]) > 0x0FFF) {
            VM->registers.v[15] = 1;
        } else {
            VM->registers.v[15] = 0;
        }
        return;
        break;
    case 0xF033:
        /* Stores the Binary-coded decimal representation of VX, with the most significant
           of three digits at the address in I, the middle digit at I plus 1, and the least
           significant digit at I plus 2. (In other words, take the decimal representation of VX,
           place the hundreds digit in VM at location in I, the tens digit at location I+1,
           and the ones digit at location I+2.) */
        vx = VM->registers.v[(opcode & 0x0F00) >> 8];
        VM->memory[VM->registers.I] = (vx / 100) % 10;
        VM->memory[VM->registers.I + 1] = (vx / 10) % 10;
        VM->memory[VM->registers.I + 2] = vx % 10;
        return;
        break;
    case 0xF055:
        /* Stores V0 to VX in VM starting at address I. */
        for (i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
             VM->memory[VM->registers.I++] = VM->registers.v[i];
        }
        return;
        break;
    case 0xF065:
        /* Fills V0 to VX with values from VM starting at address I. */
        for (i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
            VM->registers.v[i] = VM->memory[VM->registers.I++];
        }
        return;
        break;
    default:
        break;
    }

    printf("Unknown opcode %X\n", opcode);
    VM->halt = 1;
}
