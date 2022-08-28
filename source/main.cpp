#include <iostream>
#include <fstream>
#include <array>
#include <bit>

class PerkysEsoLangInterpreter {
public:
	PerkysEsoLangInterpreter();
	PerkysEsoLangInterpreter(const std::string &pInputFile);
	~PerkysEsoLangInterpreter();

	virtual void run();
	virtual void setInputFile(const std::string &pInputFile);
protected:
	// Read a 4-bit value at current instruction pointer
	inline std::uint8_t mRead4bit() {
		std::uint8_t pValue = mRead4bit(mCodeFile.tellg());
		mCodeFile.seekg(1, std::ios::seekdir::_S_cur);
		return pValue;
	}
	// Read a 8-bit value at current instruction pointer
	inline std::uint8_t mRead8bit() {
		std::uint8_t pValue = mRead8bit(mCodeFile.tellg());
		mCodeFile.seekg(2, std::ios::seekdir::_S_cur);
		return pValue;
	}
	// Read a 16-bit value at current instruction pointer
	inline std::uint16_t mRead16bit() {
		std::uint16_t pValue = mRead16bit(mCodeFile.tellg());
		mCodeFile.seekg(4, std::ios::seekdir::_S_cur);
		return pValue;
	}

	// Gets a register at current instruction pointer and increments instruction pointer
	virtual std::uint8_t getRegister() {
		uint8_t lRegister = mCodeFile.get();
		switch(lRegister) {
		case '<': return 0;
		case '>': return 1;
		case '~': return 2;
		case '^': return 3;
		case '?': return 4;
		case '@': return 5;
		case '=': return 6;
		case '|': return 7;
		default: throw std::runtime_error("Invalid register.");
		}
	}

	// Read a 4-bit value at specified offset to instruction pointer
	virtual std::uint8_t mRead4bit(std::streampos pPosition) {
		auto lOldPosition = mCodeFile.tellg();
		mCodeFile.seekg(pPosition);
		std::uint8_t lChar = mCodeFile.get();
		mCodeFile.seekg(lOldPosition);

		return lChar - ' ';
	}
	// Read a 8-bit value at specified offset to instruction pointer
	virtual std::uint8_t mRead8bit(std::streampos pPosition) {
		return (mRead4bit(pPosition) << 4) | mRead4bit(pPosition.operator+(1));
	}
	// Read a 16-bit value at specified offset to instruction pointer
	virtual std::uint16_t mRead16bit(std::streampos pPosition) {
		return mRead8bit(pPosition) | (mRead8bit(pPosition.operator+(2)) << 8);
	}

	// Write a 4-bit value to specified offset to instruction pointer
	virtual void mWrite4bit(std::streampos pPosition, std::uint8_t pValue) {
		std::uint8_t lChar = ' ' + pValue;

		auto lOldPosition = mCodeFile.tellg();
		mCodeFile.seekg(pPosition);
		mCodeFile.put(lChar);
		mCodeFile.seekg(lOldPosition);
	}
	// Write a 8-bit value to specified offset to instruction pointer
	virtual void mWrite8bit(std::streampos pPosition, std::uint8_t pValue) {
		mWrite4bit(pPosition, pValue >> 4);
		mWrite4bit(pPosition.operator+(1), pValue & 0x0F);
	}
	// Write a 16-bit value to specified offset to instruction pointer
	virtual void mWrite16bit(std::streampos pPosition, std::uint16_t pValue) {
		mWrite8bit(pPosition, pValue);
		mWrite8bit(pPosition.operator+(2), pValue >> 8);
	}

	std::array<std::uint16_t, 8> mRegisters;
	bool mZeroFlag;

	std::fstream mCodeFile;

	bool mIsCodeFileLoaded;
};

int main(const int pArgCount, const char* pArgVector[]) {
	if(pArgCount != 2) {
		std::cout << "Usage: " << pArgVector[0] << " <Input File>\n";
		return -1;
	}

	PerkysEsoLangInterpreter lInterpreter(pArgVector[1]);
	lInterpreter.run();

	return 0;
}

PerkysEsoLangInterpreter::PerkysEsoLangInterpreter() {
	for(auto lRegister : mRegisters)
		lRegister = 0;
	
	mIsCodeFileLoaded = false;
}
PerkysEsoLangInterpreter::PerkysEsoLangInterpreter(const std::string &pInputFile) {
	for(auto lRegister : mRegisters)
		lRegister = 0;
	
	mIsCodeFileLoaded = true;
	setInputFile(pInputFile);
}
PerkysEsoLangInterpreter::~PerkysEsoLangInterpreter() {
	if(mIsCodeFileLoaded)
		mCodeFile.close();
}

//#define _debug

#ifdef _debug
void write(std::string msg, int a = -10000, int b = -10000, bool newline = true) {
	std::cout << msg;
	if(a != -10000)
		std::cout << " " << a;
	if(b != -10000)
		std::cout << ", " << b;
	if(newline)
		std::cout << "\n";
}
#else
#define write(a,...)
#endif

void PerkysEsoLangInterpreter::run() {
	bool lRunning = true;
	if(mIsCodeFileLoaded) {
		while(lRunning && !mCodeFile.eof()) {

#ifdef _debug
			std::cout << mCodeFile.tellg() << " | ";
#endif
			
			auto lPosition = mCodeFile.tellg();
			uint8_t lInstructionCode = mCodeFile.get();
			uint8_t lFirstRegister, lSecondRegister, lRegister, lImmediate;
			int8_t lOffset;

			if(mCodeFile.tellg() < 0) {
				lRunning = false;
				continue;
			}

			switch(lInstructionCode) {
			case ' ': // ADD r, r
				lFirstRegister = getRegister();
				lSecondRegister = getRegister();
				mRegisters[lFirstRegister] += mRegisters[lSecondRegister];
				write("ADDrr", lFirstRegister, lSecondRegister);
				break;
			case '!': // SUB r, r
				lFirstRegister = getRegister();
				lSecondRegister = getRegister();
				mRegisters[lFirstRegister] -= mRegisters[lSecondRegister];
				write("SUBrr", lFirstRegister, lSecondRegister);
				break;
			case '"': // CMP r
				lRegister = getRegister();
				mZeroFlag = mRegisters[lRegister] == 0;
				write("CMPr", lRegister, -10000, false);
				write(mZeroFlag?" true":" false");
				break;
			case '#': // JNZ rel8
				lOffset = std::bit_cast<std::int8_t>(mRead8bit());
				if(!mZeroFlag)
					mCodeFile.seekg(lPosition.operator+(lOffset).operator+(mRegisters[6]));
				write("JNZr8", lOffset);
				break;
			case '$': // MOV r, rel8
				lRegister = getRegister();
				lOffset = std::bit_cast<std::int8_t>(mRead8bit());
				mRegisters[lRegister] = mRead16bit(lPosition.operator+(lOffset).operator+(mRegisters[7]));
				write("MOVrr8", lRegister, lOffset);
				break;
			case '%': // MOV rel8, r
				lOffset = std::bit_cast<std::int8_t>(mRead8bit());
				lRegister = getRegister();
				mWrite16bit(lPosition.operator+(lOffset).operator+(mRegisters[7]), mRegisters[lRegister]);
				write("MOVr8r", lOffset, lRegister);
				break;
			case '&': // INC r
				lRegister = getRegister();
				mRegisters[lRegister]++;
				write("INCr", lRegister);
				break;
			case '\'': // DEC r
				lRegister = getRegister();
				mRegisters[lRegister]--;
				write("DECr", lRegister);
				break;
			case '(': // OUT r
				lRegister = getRegister();
				std::cout.put(mRegisters[lRegister]);
				write("OUTr", lRegister);
				break;
			case ')': // IN r
				lRegister = getRegister();
				mRegisters[lRegister] = std::cin.get();
				write("INr", lRegister);
				break;
			case '*': // JMP rel8
				lOffset = std::bit_cast<std::int8_t>(mRead8bit());
				mCodeFile.seekg(lPosition.operator+(lOffset).operator+(mRegisters[6]));
				write("JMPr8", lOffset);
				break;
			case '+': // MOV r, r
				lFirstRegister = getRegister();
				lSecondRegister = getRegister();
				mRegisters[lFirstRegister] = mRegisters[lSecondRegister];
				write("MOVrr", lFirstRegister, lSecondRegister);
				break;
			case ',': // STZ r
				lRegister = getRegister();
				mRegisters[lRegister] = 0;
				write("STZr", lRegister);
				break;
			case '-': // CTB r
				lRegister = getRegister();
				mRegisters[lRegister] &= 0xFF;
				write("CTBr", lRegister, -10000, false);
				write(" ", mRegisters[lRegister]);
				break;
			case '.': // MOV r, imm4
				lRegister = getRegister();
				lImmediate = mRead4bit();
				mRegisters[lRegister] = lImmediate;
				write("MOVri", lRegister, lImmediate);
				break;
			case '/': // HLT
				lRunning = false;
				write("HLT");
				break;
			default: // Ignore other characters
				break;
			}
		}
	}
	std::cout.put('\n');

	mCodeFile.seekg(0);
	for(auto lRegister : mRegisters)
		lRegister = 0;
}
void PerkysEsoLangInterpreter::setInputFile(const std::string &pInputFile) {
	if(mIsCodeFileLoaded)
		mCodeFile.close();
	mCodeFile.open(pInputFile);
	if(!mCodeFile.good())
		throw std::runtime_error("Could not open input file.");
}
