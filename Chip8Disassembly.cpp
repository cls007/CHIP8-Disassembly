#include "Chip8Disassembly.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdio>

using namespace std;


Chip8Disassembly::Chip8Disassembly()
{
}


Chip8Disassembly::~Chip8Disassembly()
{
}

int Chip8Disassembly::disassembly(std::string romPath)
{
	char *buffer = nullptr;
	int len = loadRom(romPath, buffer);

	cout << "[*] Start disassembly " << endl;

	int p = 0;

	

	while (p < len) {
		short opcode = (buffer[p] << 8) | (buffer[p + 1] & 0x00FF);
		 
		//printf("0x%04X | ", (short)(opcode));
		
		cout << showbase << hex << setw(4) << setfill('0') << opcode << "  |  " ;
		switch (opcode & 0xF000)
		{
		case 0x0000:
		{
			if (0x00E0 == opcode) {
				cout << "CLS" << "  #Clear the display" << endl;
			}
			else if (0x00EE == opcode) {
				cout << "RET" << "  #Return from a subroutine" << endl;
			}
			else {
				printf("SYS 0x%X # Jump to a machine code routine at 0x%X \n", (opcode & 0x0FFF), (opcode & 0x0FFF));
			}
			break;
		}
		case 0x1000:
			printf("JP 0x%X # Jump to location 0x%X \n", (opcode & 0x0FFF), (opcode & 0x0FFF));
			break;

		case 0x2000:
			printf("CALL 0x%X # Call subroutine at 0x%X \n", (opcode & 0x0FFF), (opcode & 0x0FFF));
			break;

		case 0x3000:
			printf("SE V%d, 0x%X   #Skip next instruction if V%d==0x%X \n", ((opcode & 0x0F00) >> 8), (opcode & 0x00FF), ((opcode & 0x0F00) >> 8), (opcode & 0x00FF));
			break;

		case 0x4000:
			printf("SNE V%d, 0x%X   #Skip next instruction if V%d!=0x%X \n", ((opcode & 0x0F00) >> 8), (opcode & 0x00FF), ((opcode & 0x0F00) >> 8), (opcode & 0x00FF));
			break;

		case 0x5000:
			printf("SE V%d, V%d #Skip next instruction if V%d==V%d \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;

		case 0x6000:
			printf("LD V%d, 0x%X  #SET V%d=0x%X \n", ((opcode & 0x0F00) >> 8), (opcode & 0x00FF), ((opcode & 0x0F00) >> 8), (opcode & 0x00FF));
			break;

		case 0x7000:
			printf("ADD V%d, 0x%X  #Set V%d += 0x%X \n", ((opcode & 0x0F00) >> 8), (opcode & 0x00FF), ((opcode & 0x0F00) >> 8), (opcode & 0x00FF));
			break;

		case 0x8000:
		{
			switch (opcode & 0x000F)
			{
			case 0:
				printf("LD V%d, V%d  #Set V%d = V%d \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
				break;

			case 1:
				printf("OR V%d, V%d  #Set V%d |= V%d \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
				break;
			
			case 2:
				printf("AND V%d, V%d  #Set V%d &= V%d \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
				break;

			case 3:
				printf("XOR V%d, V%d  #Set V%d = V%d XOR V%d \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
				break;

			case 4:
				printf("ADD V%d, V%d  #Set V%d += V%d, set VF = carry \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
				break;

			case 5:
				printf("SUB V%d, V%d  #Set V%d -= V%d, set VF = NOT borrow \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
				break;

			case 6:
				printf("SHR V%d{, V%d}  #Set V%d = V%d SHR 1 \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
				break;

			case 7:
				printf("SUBN  V%d, V%d  #Set V%d = V%d - V%d, set VF = NOT borrow  \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8));
				break;

			case 8:
				printf("SHL V%d {, V%d}  #Set V%d = V%d SHL 1 \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
				break;
			
			default:
				printf("Unknow opcode \n");
				break;
			}

			break;
		}

		case 0x9000:
			printf("SNE V%d, V%d  #Skip next instruction if V%d != V%d \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;

		case 0xA000:
			printf("LD I 0x%X  #Set I = 0x%X \n", (opcode & 0x0FFF), (opcode & 0x0FFF));
			break;

		case 0xB000:
			printf("JP V0, 0x%X  #Jump to location 0x%X + V0 \n", (opcode & 0x0FFF), (opcode & 0x0FFF));
			break;

		case 0xC000:
			printf("RND V%d, 0x%X  #Set Set V%d = random byte AND 0x%X \n", ((opcode & 0x0F00) >> 8), opcode & 0x00FF, ((opcode & 0x0F00) >> 8), opcode & 0x00FF);
			break;

		case 0xD000:
			printf("DRW V%d, V%d, 0x%X  #Display 0x%X bytes sprite starting at memory location I at (V%d, V%d), set VF = collision \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4), (opcode & 0x000F), (opcode & 0x000F), ((opcode & 0x0F00) >> 8), ((opcode & 0x00F0) >> 4));
			break;

		case 0xE000:
			if (0x009E == (opcode & 0x00FF)) {
				printf("SKP V%d  #Skip next instruction if key with the value of V%d is pressed  \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
			}
			else if (0x00A1 == (opcode & 0x00FF)) {
				printf("SKNP V%d  #Skip next instruction if key with the value of V%d is not pressed  \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
			}
			else {
				printf("Unknow opcode \n");
			}
			break;

		case 0xF000:
			switch (opcode & 0x00FF)
			{
			case 0x0007:
				printf("LD V%d, DT  #Set V%d = delay timer value \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
				break;

			case 0x000A:
				printf("LD V%d, K  #Wait for a key press, store the value of the key in V%d  \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
				break;

			case 0x0015:
				printf("LD DT, V%d   #Set delay timer = V%d \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
				break;

			case 0x0018:
				printf("LD ST, V%d   #Set sound timer = V%d \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
				break;
				
			case 0x001E:
				printf("ADD I, V%d   #Set I = I + V%d \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
				break;

			case 0x0029:
				printf("LD F, V%d   #Set I = location of sprite for digit V%d \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
				break;

			case 0x0033:
				printf("LD B, V%d   #Store BCD representation of V%d in memory locations I, I+1, and I+2 \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
				break;

			case 0x0055:
				printf("LD [I], V%d   #Store registers V0 through V%d in memory starting at location I. \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
				break;

			case 0x0065:
				printf("LD V%d, [I]   #Read registers V0 through V%d from memory starting at location I. \n", ((opcode & 0x0F00) >> 8), ((opcode & 0x0F00) >> 8));
				break;

			default:
				printf("Unknow opcode \n");
				break;
			}
			break;

		default:
			printf("Unknow opcode \n");
			break;

		}
	
		p += 2;
	}


	delete[] buffer;
	return 0;
}

int Chip8Disassembly::loadRom(std::string path, char* &buffer)
{
	cout << "[*] Loading rom: " << path << endl;

	ifstream is(path, ifstream::binary);
	is.seekg(0, is.end);
	int len = is.tellg();
	is.seekg(0, is.beg);

	buffer = new char[len];

	is.read(buffer, len);

	cout << "[*] Total: " << len << " Bytes" << endl;

	is.close();

	return len;
}
