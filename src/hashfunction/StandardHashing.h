#include "HashFunction.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include <math.h>

#pragma once
class IdentityFunction : public HashFunction {

public:
    uint32_t hash(uint32_t n);
    HashFunction* clone();
};

class MultiplyModPrime : public HashFunction {
    public:
        MultiplyModPrime(RandomNumberGenerator * rnd);
        MultiplyModPrime(uint64_t a, uint64_t b, uint64_t p);
        ~MultiplyModPrime();
        uint32_t hash(uint32_t n);
        HashFunction* clone();
    private:
        uint64_t a;
        uint64_t b;
        // Large mersenne prime (2^61 - 1)
        uint64_t p;
};

class MultiplyShift : public HashFunction {
    public:
        MultiplyShift(RandomNumberGenerator * rnd, int numOfOutputBits);
        MultiplyShift(uint32_t a, uint32_t numOfOutputBits);
        ~MultiplyShift();
        uint32_t hash(uint32_t n);
        HashFunction* clone();
    private:
        uint32_t a;
        uint32_t l;
};

class PolyHash {
    public:
        PolyHash(RandomNumberGenerator * rnd, uint32_t degree);
        PolyHash(uint32_t degree, uint64_t* coefs, uint64_t p);
        ~PolyHash();
        uint64_t hash(uint32_t n);
        PolyHash* clone();
    private:
        uint32_t degree;
        uint64_t* coefs;
        // Large mersenne prime (2^61 - 1)
        uint64_t p;
};

class PolyHash2_Mersenne : public HashFunction {
    public:
        PolyHash2_Mersenne(RandomNumberGenerator * rnd);
        PolyHash2_Mersenne(uint64_t a, uint64_t b);
        ~PolyHash2_Mersenne();
        uint32_t hash(uint32_t n);
        HashFunction* clone();
    private:
        uint64_t a;
        uint64_t b;
        // Large mersenne prime (2^61 - 1)
        const uint64_t p = 2305843009213693951;
};

class PolyHash3_Mersenne : public HashFunction {
    public:
        PolyHash3_Mersenne(RandomNumberGenerator * rnd);
        PolyHash3_Mersenne(uint64_t a, uint64_t b, uint64_t c);
        ~PolyHash3_Mersenne();
        uint32_t hash(uint32_t n);
        HashFunction* clone();
    private:
        uint64_t a;
        uint64_t b;
        uint64_t c;
        // Large mersenne prime (2^61 - 1)
        const uint64_t p = 2305843009213693951;
};

class PolyHash2 : public HashFunction {
    public:
        PolyHash2(RandomNumberGenerator * rnd);
        PolyHash2(uint64_t a, uint64_t b, uint64_t p);
        ~PolyHash2();
        uint32_t hash(uint32_t n);
        HashFunction* clone();
    private:
        uint64_t a;
        uint64_t b;
        // Large mersenne prime (2^61 - 1)
        uint64_t p;
};

class PolyHash3 : public HashFunction {
    public:
        PolyHash3(RandomNumberGenerator * rnd);
        PolyHash3(uint64_t a, uint64_t b, uint64_t c, uint64_t p);
        ~PolyHash3();
        uint32_t hash(uint32_t n);
        HashFunction* clone();
    private:
        uint64_t a;
        uint64_t b;
        uint64_t c;
        // Large mersenne prime (2^61 - 1)
        uint64_t p;
};