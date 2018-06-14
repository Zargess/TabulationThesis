#include "HashFunction.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

#pragma once
class SimpleTabulation : public HashFunction {
    public:
        SimpleTabulation(RandomNumberGenerator * rnd);
        SimpleTabulation(uint32_t (&table)[256][4]);
        ~SimpleTabulation();
        uint32_t hash(uint32_t n);
        HashFunction* clone();
    private:
        RandomNumberGenerator * rnd;
        uint32_t tables[256][4];
};

class SimpleTabulation16Bit : public HashFunction {
    public:
        SimpleTabulation16Bit(RandomNumberGenerator * rnd);
        SimpleTabulation16Bit(uint32_t (&table)[65536][2]);
        ~SimpleTabulation16Bit();
        uint32_t hash(uint32_t n);
        HashFunction* clone();

    private:
        RandomNumberGenerator * rnd;
        uint32_t tables[65536][2];
};

class MixedTabulation : public HashFunction {
    public:
        MixedTabulation(RandomNumberGenerator * rnd);
        MixedTabulation(uint64_t (&table_1)[256][4], uint32_t (&table_2)[256][4]);
        ~MixedTabulation();
        uint32_t hash(uint32_t n);
        HashFunction* clone();
    private:
        RandomNumberGenerator * rnd;
        uint64_t T1[256][4];
        uint32_t T2[256][4];
};