#pragma once

#include <cstdint>
#include <cstddef>

typedef uint8_t error_type;

class Instruction {
public:
	// Number of instruction
	// Valid range: 0=>31
	uint8_t instruction;

	// Registers targeted by instruction
	// Register 1: Lower  4 bits (0b00001111)
	// Register 2: Higher 4 bits (0b11110000)
	uint8_t register_target;

	// Memory address targeted by instruction
	// Only used by Store(16)/Load(17) instructions
	size_t memory_target;

	// Times instruction was used in a row
	// Ignored by instructions where the result would be the same for any amount of times used above 1
	size_t instruction_count;
};

template <class memory_type, size_t memory_size>
class Interpreter {
private:
	size_t instruction_pointer;
	std::string instruction_string;
	size_t instruction_count;
	Instruction *instruction_array;

	// Remove unessesarry instructions from code stored in instruction_string
	error_type minimize_code();
	// Optimize code stored in instruction_string to be as fast as possible
	error_type optimize_code();
	// Convert code stored in instruction_string to instruction_code format
	error_type convert_code();

public:
	memory_type *memory;
	memory_type registers[16];
	error_type load_instructions(const std::string& instruction_string);
	error_type reset_state();
	Interpreter();
	Interpreter(std::string instruction_string);
};