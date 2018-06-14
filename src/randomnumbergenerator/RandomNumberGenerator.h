#include <cstdint>

#pragma once
class RandomNumberGenerator {
public:
    virtual uint64_t getNextNumber() = 0;
};