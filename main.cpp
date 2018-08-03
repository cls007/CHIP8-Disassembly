#include <iostream>

#include "Chip8Disassembly.h"

using namespace std;

int main()
{

	Chip8Disassembly dis;
	dis.disassembly("PONG");

	system("pause");

	return 0;
}