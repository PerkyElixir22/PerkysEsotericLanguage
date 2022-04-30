#pragma once

#include <cstdint>

uint8_t getImmediate8(const std::string& pCode, size_t pCodePointer);
uint16_t getImmediate16(const std::string& pCode, size_t pCodePointer);