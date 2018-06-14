#include "Tabulation.h"
#include <iostream>
#include <bitset>

// Based on: https://github.com/zera/Nips_MT/blob/master/src/framework/hashing.h
SimpleTabulation::SimpleTabulation(RandomNumberGenerator * rnd) {
    this->rnd = rnd;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 256; j++) {
            tables[j][i] = rnd->getNextNumber();
        }
    }
}

SimpleTabulation::SimpleTabulation(uint32_t (&table)[256][4]) {
    for(int i = 0; i < 256; i++) {
        for(int j = 0; j < 4; j++) {
            this->tables[i][j] = table[i][j];
        }
    }
}

SimpleTabulation::~SimpleTabulation() {}

HashFunction* SimpleTabulation::clone() {
    return new SimpleTabulation(tables);
}

uint32_t SimpleTabulation::hash(uint32_t n) {
    uint32_t result = 0;
    int shift = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t key = n >> shift;
        result ^= tables[key][i];
        shift += 8;
    }

    return result;
}

// Based on: https://github.com/zera/Nips_MT/blob/master/src/framework/hashing.h
SimpleTabulation16Bit::SimpleTabulation16Bit(RandomNumberGenerator * rnd) {
    rnd = rnd;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 65536; j++) {
            tables[j][i] = rnd->getNextNumber();
        }
    }
}

SimpleTabulation16Bit::SimpleTabulation16Bit(uint32_t (&table)[65536][2]) {
    for(int i = 0; i < 65536; i++) {
        for(int j = 0; j < 2; j++) {
            this->tables[i][j] = table[i][j];
        }
    }
}

SimpleTabulation16Bit::~SimpleTabulation16Bit() {}

HashFunction* SimpleTabulation16Bit::clone() {
    return new SimpleTabulation16Bit(tables);
}

uint32_t SimpleTabulation16Bit::hash(uint32_t n) {
    uint32_t result = 0;

    for (int i = 0; i < 2; ++i, n >>= 16) {
        result ^= tables[(uint16_t) n][i];
    }

    return result;
}

// Based on: https://github.com/zera/Nips_MT/blob/master/src/framework/hashing.h
MixedTabulation::MixedTabulation(RandomNumberGenerator * rnd) {
    rnd = rnd;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 256; j++) {
            T1[j][i] = rnd->getNextNumber();
            T1[j][i] <<= 32;
            T1[j][i] += rnd->getNextNumber();
            T2[j][i] = rnd->getNextNumber();
        }
    }
}

MixedTabulation::MixedTabulation(uint64_t (&table_1)[256][4], uint32_t (&table_2)[256][4]) {
    for(int i = 0; i < 256; i++) {
        for(int j = 0; j < 4; j++) {
            this->T1[i][j] = table_1[i][j];
            this->T2[i][j] = table_2[i][j];
        }
    }
}

MixedTabulation::~MixedTabulation() {}

HashFunction* MixedTabulation::clone() {
    return new MixedTabulation(T1, T2);
}

uint32_t MixedTabulation::hash(uint32_t n) {
    uint64_t result = 0;

    for (int i = 0; i < 4; i++, n >>= 8) {
        result ^= T1[(uint8_t)n][i];
    }

    uint32_t drv = result >> 32;

    for (int i = 0; i < 4; i++, drv >>= 8) {
        result ^= T2[(uint8_t) drv][i];
    }

    return (uint32_t)result;
}