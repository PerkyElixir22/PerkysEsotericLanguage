#include <iostream>
#include <cstdint>
#include <iomanip>

#include <thread>

#include "file.hpp"
#include "getval.hpp"

#define printUsage() std::cout << "Usage: " << pArgVector[0] << " [Input file]\n";

uint8_t charToReg(char pRegChar) {
	switch(pRegChar) {
	case '<':
		return 0;
	case '>':
		return 1;
	case '~':
		return 2;
	case '^':
		return 3;
	case '?':
		return 4;
	case '@':
		return 5;
	case '=':
		return 6;
	case '|':
		return 7;
	default:
		throw std::out_of_range(std::string("Invalid register '") + pRegChar + std::string("'"));
	}
}

int main(const int pArgCount, const char* pArgVector[]) {
	if(pArgCount == 1) {
		std::cout << "No input file.\n";
		printUsage();
		return -1;
	}
	if(pArgCount > 2) {
		std::cout << "Too many arguments.\n";
		printUsage();
		return -2;
	}

	std::string lCode = readFile(pArgVector[1]);

	std::size_t lCodePointer = 0;
	std::size_t lCodeSize = lCode.size();

	uint16_t lRegister[8] {0};

	bool lIsZero = false;

	try {
		while(lCodePointer < lCodeSize) {
			uint16_t lRegVal;
			size_t lAddress;
			switch(lCode[lCodePointer]) {
			case ' ':
				lRegister[charToReg(lCode[lCodePointer+1])] += lRegister[charToReg(lCode[lCodePointer+2])];
				lCodePointer += 3;
				break;
			case '!':
				lRegister[charToReg(lCode[lCodePointer+1])] -= lRegister[charToReg(lCode[lCodePointer+2])];
				lCodePointer += 3;
				break;
			case '"':
				lIsZero = lRegister[charToReg(lCode[lCodePointer+1])] == 0;
				lCodePointer += 2;
				break;
			case '#':
				if(!lIsZero)
					lCodePointer += static_cast<int8_t>(getImmediate8(lCode, lCodePointer+1));
				else
					lCodePointer += 3;
				break;
			case '$':
				lRegister[charToReg(lCode[lCodePointer+1])] = getImmediate16(lCode, lCodePointer + static_cast<int8_t>(getImmediate8(lCode, lCodePointer+2)) + lRegister[7]);
				lCodePointer += 4;
				break;
			case '%':
				lRegVal = lRegister[charToReg(lCode[lCodePointer+3])];
				lAddress = lCodePointer + static_cast<int8_t>(getImmediate8(lCode, lCodePointer+1) + lRegister[7]);

				lCode[lAddress + 0] = ((lRegVal & 0x00F0) >>  4) + ' ';
				lCode[lAddress + 1] = ((lRegVal & 0x000F) >>  0) + ' ';

				lCode[lAddress + 2] = ((lRegVal & 0xF000) >> 12) + ' ';
				lCode[lAddress + 3] = ((lRegVal & 0x0F00) >>  8) + ' ';

				lCodePointer += 4;

				break;
			case '&':
				++lRegister[charToReg(lCode[lCodePointer+1])];
				lCodePointer += 2;
				break;
			case '\'':
				--lRegister[charToReg(lCode[lCodePointer+1])];
				lCodePointer += 2;
				break;
			case '(':
				std::cout.put(lRegister[charToReg(lCode[lCodePointer+1])] & 0x00FF);
				lCodePointer += 2;
				break;
			case ')':
				std::cin.read(reinterpret_cast<char*>(&lRegister[charToReg(lCode[lCodePointer+1])]), 1);
				lCodePointer += 2;
				break;
			case '*':
				lCodePointer += static_cast<int8_t>(getImmediate8(lCode, lCodePointer+1));
				break;
			case '+':
				lRegister[charToReg(lCode[lCodePointer+1])] = lRegister[charToReg(lCode[lCodePointer+2])];
				lCodePointer += 3;
				break;
			case ',':
				lRegister[charToReg(lCode[lCodePointer+1])] = 0;
				lCodePointer += 2;
				break;
			case '-':
				lRegister[charToReg(lCode[lCodePointer+1])] &= 0x00FF;
				lCodePointer += 2;
				break;
			case '.':	
				lRegister[charToReg(lCode[lCodePointer+1])] = getImmediate8(lCode, lCodePointer+1) & 0x0F;
				lCodePointer += 3;
				break;
			case '/':
				lCodePointer = lCodeSize;
				break;
			default:
				throw std::runtime_error(std::string("Invalid instruction '") + lCode[lCodePointer] + std::string("'"));
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	catch(const std::exception& e) {
		std::cout << e.what() << " @ " << lCodePointer << "\n";
		return -3;
	}
	std::cout << "\n";
}