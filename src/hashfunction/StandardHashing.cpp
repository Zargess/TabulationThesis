#include "StandardHashing.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include "../randomnumbergenerator/CRandNumberGenerator.h"
#include "HashFunction.h"
#include <iostream>
#include <string>
#include <bitset>


uint32_t IdentityFunction::hash(uint32_t n) {
    return n;
}

HashFunction* IdentityFunction::clone() {
    return new IdentityFunction();
}

MultiplyModPrime::MultiplyModPrime(RandomNumberGenerator* rnd) {
    a = 0;
    //REAL MERSENNE 2^61 - 1
    p = 2305843009213693951;
    while(a == 0 || a >= p) {
        a = rnd->getNextNumber();
    } 
    b = p;
    while(b >= p) {
        b = rnd->getNextNumber();
    }
}

MultiplyModPrime::MultiplyModPrime(uint64_t a, uint64_t b, uint64_t p) {
    this->a = a;
    this->b = b;
    this->p = p;
}

MultiplyModPrime::~MultiplyModPrime() {
}

HashFunction* MultiplyModPrime::clone() {
    return new MultiplyModPrime(a, b, p);
}

uint32_t MultiplyModPrime::hash(uint32_t n) {
    uint64_t res = (a * n)  + b;
    return res % p;
}

MultiplyShift::MultiplyShift(RandomNumberGenerator* rnd, int numOfOutputbits) {
    a = (uint32_t) rnd->getNextNumber();
    if(a % 2 == 0) a += 1;
    l = numOfOutputbits;
}

MultiplyShift::MultiplyShift(uint32_t a, uint32_t numOfOutputbits) {
    this->a = a;
    this->l = numOfOutputbits;
}

MultiplyShift::~MultiplyShift() {
}

HashFunction* MultiplyShift::clone(){
    return new MultiplyShift(a, l);
}

uint32_t MultiplyShift::hash(uint32_t n) {
    return (a * (uint64_t) n) >> (32-l);

}

PolyHash::PolyHash(RandomNumberGenerator* rnd, uint32_t degree) {
    this->degree = degree;
    this->coefs = new uint64_t[degree];

    for(int i = 0; i < degree; i++) {
        this->coefs[i] = rnd->getNextNumber();
    }

    RandomPrimeGenerator primegen(rnd);
    uint64_t lower = 2305843009213693952;
    uint64_t upper = 18446744073709551614;
    p = primegen.get(lower, upper);
}

PolyHash::PolyHash(uint32_t degree, uint64_t* coefs, uint64_t p) {
    this->degree = degree;
    this->coefs = coefs;
    this->p = p;
}

PolyHash::~PolyHash() {
    delete[] coefs;
}

PolyHash* PolyHash::clone() {
    uint64_t* newcoefs = new uint64_t[degree];
    for(int i = 0; i < degree; i++) {
        newcoefs[i] = coefs[i];
    }
    return new PolyHash(degree, newcoefs, p);
}

uint64_t PolyHash::hash(uint32_t n) {
    __int128 h = 0;
    for(int i = degree-1; i >=0 ; i--) {
        h = h * n + coefs[i];
        //61 because p = 2^61 (mersenne prime)
        h = h % p;
    }

    return (uint64_t) h;
    
}

PolyHash2_Mersenne::PolyHash2_Mersenne(RandomNumberGenerator* rnd) {
    a = 0;
    while(a == 0 || a >= p) {
        a = rnd->getNextNumber();
    } 
    b = p;
    while(b >= p) {
        b = rnd->getNextNumber();
    }
}

PolyHash2_Mersenne::PolyHash2_Mersenne(uint64_t a, uint64_t b) {
    this->a = a;
    this->b = b;
}

PolyHash2_Mersenne::~PolyHash2_Mersenne() {
}

HashFunction* PolyHash2_Mersenne::clone() {
    return new PolyHash2_Mersenne(a, b);
}

uint32_t PolyHash2_Mersenne::hash(uint32_t n) {
    __int128 h = a * n + b;
    h = (h & p) + (h >> 61);
    h = (h & p) + (h >> 61);
    return (uint32_t)h;
}


PolyHash3_Mersenne::PolyHash3_Mersenne(RandomNumberGenerator* rnd) {
    a = 0;
    while(a == 0 || a >= p) {
        a = rnd->getNextNumber();
    } 
    b = p;
    while(b >= p) {
        b = rnd->getNextNumber();
    }
    c = p;
    while(c >= p) {
        c = rnd->getNextNumber();
    } 
}

PolyHash3_Mersenne::PolyHash3_Mersenne(uint64_t a, uint64_t b, uint64_t c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

PolyHash3_Mersenne::~PolyHash3_Mersenne() {
}

HashFunction* PolyHash3_Mersenne::clone() {
    return new PolyHash3_Mersenne(a, b, c);
}

uint32_t PolyHash3_Mersenne::hash(uint32_t n) {
    __int128 h = a * n + b;
    h = (h & p) + (h >> 61);
    h = h * n + c;
    h = (h & p) + (h >> 61);
    h = (h & p) + (h >> 61);
    return (uint32_t) h;
}


PolyHash2::PolyHash2(RandomNumberGenerator* rnd) {
    a = rnd->getNextNumber();
    b = rnd->getNextNumber();

    RandomPrimeGenerator primegen(rnd);
    uint64_t lower = 4294967296;
    uint64_t upper = 4400000000;
    p = primegen.get(lower, upper);
}

PolyHash2::PolyHash2(uint64_t a, uint64_t b, uint64_t p) {
    this->a = a;
    this->b = b;
    this->p = p;
}

PolyHash2::~PolyHash2() {
}

HashFunction* PolyHash2::clone() {
    return new PolyHash2(a, b, p);
}

uint32_t PolyHash2::hash(uint32_t n) {
    return (uint32_t) ((__int128) ((__int128) a * (__int128) n) + (__int128) b) % p;
}

PolyHash3::PolyHash3(RandomNumberGenerator* rnd) {
    a = rnd->getNextNumber();
    b = rnd->getNextNumber();
    c = rnd->getNextNumber();

    RandomPrimeGenerator primegen(rnd);
    uint64_t lower = 4294967296;
    uint64_t upper = 4400000000;
    p = primegen.get(lower, upper);
}

PolyHash3::PolyHash3(uint64_t a, uint64_t b, uint64_t c, uint64_t p) {
    this->a = a;
    this->b = b;
    this->c = c;
    this->p = p;
}

PolyHash3::~PolyHash3() {
}

HashFunction* PolyHash3::clone() {
    return new PolyHash3(a, b, c, p);
}

uint32_t PolyHash3::hash(uint32_t n) {
    return (uint32_t) ((__int128) (((__int128) ((__int128)a * (__int128) n) +(__int128) b) % p) * (__int128)c) % p;
}
