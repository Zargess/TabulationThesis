#include <cstddef>
#include <string>
#include <iostream>
#include "DoubleHashingWithCounter.h"
#include "../hashtable/HashTableCounter.h"
#include "../hashfunction/Tabulation.h"
#include "../hashfunction/StandardHashing.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

HashFunction * DoubleHashingWithCounter::initHash(std::string hash) {
     HashFunction * res;

	if(hash == "simple") {
        res = new SimpleTabulation(this->rnd);
    } else if(hash == "mixed") {
        res = new MixedTabulation(this->rnd);
    } else if(hash == "multshift") {
        res = new MultiplyShift(this->rnd, this->outputBits);
    } else if(hash == "poly2") {
        res = new PolyHash2_Mersenne(this->rnd);
    } else if(hash == "poly3") {
        res = new PolyHash3_Mersenne(this->rnd);
    }

	sequenceOfHashFunctions->push_back(res->clone());
    
    return res;
}

DoubleHashingWithCounter::DoubleHashingWithCounter(int size, std::string hash_name1, std::string hash_name2, RandomNumberGenerator* rnd, uint32_t notInData, int outputBits) {
    table = new uint32_t[size];
    this->empty = notInData;

    for(int i = 0; i<size; i++) {
        table[i] = notInData;
    }
    this->rnd = rnd;
    this->size = size;
	this->hash_name1 = hash_name1;
    this->hash_name2 = hash_name2;
	this->outputBits = outputBits;
    this->sequenceOfHashFunctions = new std::vector<HashFunction*>();
    this->hashFunction1 = initHash(hash_name1);
    this->hashFunction2 = initHash(hash_name2);
    probeCount = 0;
    searchProbeCount = 0;
}

DoubleHashingWithCounter::DoubleHashingWithCounter(uint32_t* table, int size, std::string hash_name1, std::string hash_name2, RandomNumberGenerator* rnd, HashFunction* hashFunction1, HashFunction* hashFunction2, uint32_t notInData, int outputBits, int probeCount, int searchProbeCount, std::vector<HashFunction*>* sequenceOfHashFunctions) {
    this->table = new uint32_t[size];
    this->empty = notInData;

    for(int i = 0; i < size; i++) {
        this->table[i] = table[i];
    }

    this->hashFunction1 = hashFunction1;
	this->hash_name1 = hash_name1;
    this->hashFunction2 = hashFunction2;
	this->hash_name2 = hash_name2;
    this->size = size;
	this->outputBits = outputBits;
	this->rnd = rnd;
	this->probeCount = probeCount;
    this->searchProbeCount = searchProbeCount;
	this->sequenceOfHashFunctions = sequenceOfHashFunctions;
}

DoubleHashingWithCounter::~DoubleHashingWithCounter() {
	delete[] table;
	delete hashFunction1;
    delete hashFunction2;
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		delete (*sequenceOfHashFunctions)[i];
	}
	delete sequenceOfHashFunctions;
}

HashTableCounter* DoubleHashingWithCounter::clone() {
	return new DoubleHashingWithCounter(table, size, hash_name1, hash_name2, rnd, hashFunction1->clone(), hashFunction2->clone(), empty, outputBits, probeCount, searchProbeCount, getHashFunctions());
}

std::vector<HashFunction*>* DoubleHashingWithCounter::getHashFunctions() {
	std::vector<HashFunction*>* hash_funcs = new std::vector<HashFunction*>();
	
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		hash_funcs->push_back((*sequenceOfHashFunctions)[i]->clone());
	}
	return hash_funcs;
}

void DoubleHashingWithCounter::insertWithCounter(uint32_t value) {
    uint32_t h1 = hashFunction1->hash(value) % size;

    if (table[h1] == value) {
        return;
    }
    
    if (table[h1] == empty) {
        table[h1] = value;
        return;
    }
    
    uint32_t h2 = (hashFunction2->hash(value) % (size - 1)) + 1;
    uint32_t index = (h1 + h2) % size;

    while (table[index] != empty && table[index] != value) {
		probeCount++;
        index = (index + h2) % size;
    }

    if (table[index] == value) {
        return;
    }
    
    table[index] = value;
}

int DoubleHashingWithCounter::searchWithCounter(uint32_t value) {
    uint32_t h1 = hashFunction1->hash(value) % size;
    
    if (table[h1] == value) {
        return h1;
    }
    
    uint32_t h2 = (hashFunction2->hash(value) % (size - 1)) + 1;
    uint32_t index = (h1 + h2) % size;

    while (table[index] != value) {
		searchProbeCount++;
        index = (index + h2) % size;
    }

    return index;
}

void DoubleHashingWithCounter::clean() {
    for(int i = 0; i < size; i++) {
        table[i] = empty;
    }
}

std::vector<int>* DoubleHashingWithCounter::getCounters() {
    std::vector<int>* res = new std::vector<int>();
    res->push_back(probeCount);
    res->push_back(searchProbeCount);
    return res;
}

void DoubleHashingWithCounter::resetCounters() {
    probeCount = 0;
    searchProbeCount = 0;
}

void DoubleHashingWithCounter::reinitHashFunction() {
	delete hashFunction1;
    delete hashFunction2;
	hashFunction1 = initHash(hash_name1);
	hashFunction2 = initHash(hash_name2);
}

void DoubleHashingWithCounter::cleanHashFunctions() {
    for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		delete (*sequenceOfHashFunctions)[i];
	}
    sequenceOfHashFunctions->clear();
}