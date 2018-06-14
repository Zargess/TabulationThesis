#include "RandomNumberGenerator.h"
#include <random> 
#include <vector>
#include "../hashfunction/StandardHashing.h"

#pragma once
class CRandNumberGenerator : public RandomNumberGenerator {
public:
    uint64_t getNextNumber();
    CRandNumberGenerator();
};

#pragma once
class UniformDistMT : public RandomNumberGenerator {
public:
    uint64_t getNextNumber();
    UniformDistMT();
private:
    std::mt19937 rng;
    std::uniform_int_distribution<uint64_t> dist;
};

#pragma once
class PolyHashRNG : public RandomNumberGenerator {
public:
    uint64_t getNextNumber();
    PolyHashRNG(RandomNumberGenerator* rng, uint32_t degree);
    ~PolyHashRNG();
private:
    PolyHash* h;
    uint32_t x;
};

// Based on code found at http://www.cplusplus.com/forum/beginner/21769/
class RandomPrimeGenerator
{
    public:
      RandomPrimeGenerator(RandomNumberGenerator* rand);
      uint64_t get(uint64_t lower, uint64_t upper);
    private:
      bool isPrime(uint64_t n);
      RandomNumberGenerator* rand;
};