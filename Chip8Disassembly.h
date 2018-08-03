#pragma once

#include <string>

class Chip8Disassembly
{
public:
	Chip8Disassembly();
	~Chip8Disassembly();

	int disassembly(std::string romPath);

private:
	int loadRom(std::string path, char* &buffer);

};

