/*
This file is part of evm-esp32.

evm-esp32 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

evm-esp32 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with evm-esp32. If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef OPCODES_H
#define OPCODES_H
#include "vm.h"

void parse_opcode(struct vm *, uint16_t);
#endif
