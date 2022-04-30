#include <string>

#include "getval.hpp"

uint8_t getImmediate8(const std::string& pCode, size_t pCodePointer) {
    uint8_t byte1 = pCode[pCodePointer] & 0x0F;
    uint8_t byte2 = pCode[pCodePointer + 1] & 0x0F;

    return (byte1 << 4) | byte2;
}
uint16_t getImmediate16(const std::string& pCode, size_t pCodePointer) {
    return (getImmediate8(pCode, pCodePointer+2)<<8) | getImmediate8(pCode, pCodePointer);
}