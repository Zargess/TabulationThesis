#include "CRandNumberGenerator.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include "../hashfunction/StandardHashing.h"

uint64_t CRandNumberGenerator::getNextNumber() {
    return rand();
}

CRandNumberGenerator::CRandNumberGenerator() {
    srand(time(NULL));
}

UniformDistMT::UniformDistMT() {
    rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

uint64_t UniformDistMT::getNextNumber() {
    return  dist(rng);
}

PolyHashRNG::PolyHashRNG(RandomNumberGenerator* rng, uint32_t degree) {
    h = new PolyHash(rng, degree);
    x = 0;
    
}

PolyHashRNG::~PolyHashRNG() {
    delete h;
}

uint64_t PolyHashRNG::getNextNumber() {
    return h->hash(x++);
}

// ----------------------------- Random Prime Generator ---------------------------------

RandomPrimeGenerator::RandomPrimeGenerator(RandomNumberGenerator* rand) {
    this->rand = rand;
}

bool RandomPrimeGenerator::isPrime(uint64_t n) {
    if((n&1)==0) return n==2;
    if(n%3==0) return n==3;
    if(n<25) return n>1;
    uint64_t p = 5;
    while (p*p <= n) {
        if (n%p==0) return false;
        p += 2;
        if (n%p==0) return false;
        p += 4;
    }
    return true;
}

uint64_t RandomPrimeGenerator::get(uint64_t lower, uint64_t upper) {
    uint64_t spread = upper - lower + 1;
    while(true) {
        uint64_t p = 1 | (this->rand->getNextNumber() % spread + lower);
        if (isPrime(p)) return p;
    }
}