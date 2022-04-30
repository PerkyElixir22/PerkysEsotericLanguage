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
			//std::cout << std::hex << std::setfill('0') << std::setw(4) << lCodePointer << " : " << std::dec << std::setw(0) << std::setfill(' ');
			switch(lCode[lCodePointer]) {
			case ' ':
				//std::cout << "ADD " << lCode[lCodePointer+1] << ", " << lCode[lCodePointer+2] << "\n";
				lRegister[charToReg(lCode[lCodePointer+1])] += lRegister[charToReg(lCode[lCodePointer+2])];
				lCodePointer += 3;
				break;
			case '!':
				//std::cout << "SUB " << lCode[lCodePointer+1] << ", " << lCode[lCodePointer+2] << "\n";
				lRegister[charToReg(lCode[lCodePointer+1])] -= lRegister[charToReg(lCode[lCodePointer+2])];
				lCodePointer += 3;
				break;
			case '"':
				//std::cout << "TEST " << lCode[lCodePointer+1] << "\n";
				lIsZero = lRegister[charToReg(lCode[lCodePointer+1])] == 0;
				lCodePointer += 2;
				break;
			case '#':
				//std::cout << "JNZ " << +static_cast<int8_t>(getImmediate8(lCode, lCodePointer+1)) << "\n";
				if(!lIsZero)
					lCodePointer += static_cast<int8_t>(getImmediate8(lCode, lCodePointer+1));
				else
					lCodePointer += 3;
				break;
			case '$':
				//std::cout << "LOAD " << lCode[lCodePointer+1] << ", " << (lCodePointer + static_cast<int8_t>(getImmediate8(lCode, lCodePointer+2))+lRegister[7]) << "\n";
				lRegister[charToReg(lCode[lCodePointer+1])] = getImmediate16(lCode, lCodePointer + static_cast<int8_t>(getImmediate8(lCode, lCodePointer+2)) + lRegister[7]);
				lCodePointer += 4;
				break;
			case '%':
				// This doesn't work yet!
				//std::cout << "STORE rl8 r\n";
				lCode[lCodePointer + static_cast<int8_t>(getImmediate8(lCode, lCodePointer+1))] = lRegister[charToReg(lCode[lCodePointer+3])];
				lCodePointer += 4;
				throw std::runtime_error("STORE is not implemented.");
				break;
			case '&':
				//std::cout << "INC " << lCode[lCodePointer+1] << "\n";
				++lRegister[charToReg(lCode[lCodePointer+1])];
				lCodePointer += 2;
				break;
			case '\'':
				//std::cout << "DEC " << lCode[lCodePointer+1] << "\n";
				--lRegister[charToReg(lCode[lCodePointer+1])];
				lCodePointer += 2;
				break;
			case '(':
				//std::cout << "PRINT " << lCode[lCodePointer+1] << "\n";
				std::cout.put(lRegister[charToReg(lCode[lCodePointer+1])] & 0x00FF);
				lCodePointer += 2;
				break;
			case ')':
				//std::cout << "READ " << lCode[lCodePointer+1] << "\n";
				std::cin.read(reinterpret_cast<char*>(&lRegister[charToReg(lCode[lCodePointer+1])]), 1);
				lCodePointer += 2;
				break;
			case '*':
				//std::cout << "JMP " << +static_cast<int8_t>(getImmediate8(lCode, lCodePointer+1)) << "\n";
				lCodePointer += static_cast<int8_t>(getImmediate8(lCode, lCodePointer+1));
				break;
			case '+':
				//std::cout << "COPY " << lCode[lCodePointer+1] << ", " << lCode[lCodePointer+2] << "\n";
				lRegister[charToReg(lCode[lCodePointer+1])] = lRegister[charToReg(lCode[lCodePointer+2])];
				lCodePointer += 3;
				break;
			case ',':
				//std::cout << "ZERO " << lCode[lCodePointer+1] << "\n";
				lRegister[charToReg(lCode[lCodePointer+1])] = 0;
				lCodePointer += 2;
				break;
			case '-':
				//std::cout << "ANDLB " << lCode[lCodePointer+1] << "\n";
				lRegister[charToReg(lCode[lCodePointer+1])] &= 0x00FF;
				lCodePointer += 2;
				break;
			case '.':	
				//std::cout << "LOAD " << lCode[lCodePointer+1] << ", " << +(getImmediate8(lCode, lCodePointer+1) & 0x0F) << "\n";
				lRegister[charToReg(lCode[lCodePointer+1])] = getImmediate8(lCode, lCodePointer+1) & 0x0F;
				lCodePointer += 3;
				break;
			case '/':
				//std::cout << "EXIT\n";
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