#include <cstddef>
#include <string>
#include <iostream>
#include "LinearProbingWithCounter.h"
#include "../hashtable/HashTableCounter.h"
#include "../hashfunction/Tabulation.h"
#include "../hashfunction/StandardHashing.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

HashFunction * LinearProbingWithCounter::initHash(std::string hash) {
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

LinearProbingWithCounter::LinearProbingWithCounter(int size, std::string hash_name, RandomNumberGenerator* rnd, uint32_t notInData, int outputBits) {
    
    table = new uint32_t[size];
    this->empty = notInData;
    
    for(int i = 0; i<size; i++) {
        table[i] = notInData;
    }
    this->rnd = rnd;
    this->size = size;
	this->hash_name = hash_name;
	this->outputBits = outputBits;
    this->sequenceOfHashFunctions = new std::vector<HashFunction*>();
	this->hashFunction = initHash(hash_name);
    probeCount = 0;
    searchProbeCount = 0;
    
}

LinearProbingWithCounter::LinearProbingWithCounter(uint32_t* table, int size, std::string hash_name, RandomNumberGenerator* rnd, HashFunction* hashFunction, uint32_t notInData, int outputBits, int probeCount, int searchProbeCount, std::vector<HashFunction*>* sequenceOfHashFunctions) {
	this->table = new uint32_t[size];
    this->empty = notInData;

    for(int i = 0; i < size; i++) {
        this->table[i] = table[i];
    }

    this->hashFunction = hashFunction;
	this->hash_name = hash_name;
    this->size = size;
	this->outputBits = outputBits;
	this->rnd = rnd;
	this->probeCount = probeCount;
    this->searchProbeCount = searchProbeCount;
	this->sequenceOfHashFunctions = sequenceOfHashFunctions;
}

LinearProbingWithCounter::~LinearProbingWithCounter() {
	delete[] table;
	delete hashFunction;
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		delete (*sequenceOfHashFunctions)[i];
	}
	delete sequenceOfHashFunctions;
}

HashTableCounter* LinearProbingWithCounter::clone() {
	return new LinearProbingWithCounter(table, size, hash_name, rnd, hashFunction->clone(), empty, outputBits, probeCount, searchProbeCount, getHashFunctions());
}

std::vector<HashFunction*>* LinearProbingWithCounter::getHashFunctions() {
	std::vector<HashFunction*>* hash_funcs = new std::vector<HashFunction*>();
	
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		hash_funcs->push_back((*sequenceOfHashFunctions)[i]->clone());
	}
	return hash_funcs;
}

void LinearProbingWithCounter::insertWithCounter(uint32_t value) {
    int index = hashFunction->hash(value) % size;
    int j = 0;
    for(int i=0; i<size; i++) {
        if(index+i < size) {
            if(table[index+i] == value) {
                return;
            } else if(table[index+i] == empty) {
                table[index+i] = value;
                return;
            }
        } else  {
            if (table[j] == value) {
                return;
            } else if (table[j] == empty) {
                table[j] = value;
                return;
            }
            j++;
        }
        probeCount++;
    }
    return;
}

int LinearProbingWithCounter::searchWithCounter(uint32_t value) {
    int index = hashFunction->hash(value) % size;
    int j = 0;
    for(int i=0; i<size; i++) {
        if(index+i < size) {
            if(table[index+i] == value) {
                return index+i;
            }
        } else {
            if(table[j] == value) {
                return j;
            }
            j++;
        }
        searchProbeCount++;
    }
    return -1;
}

void LinearProbingWithCounter::clean() {
    for(int i = 0; i < size; i++) {
        table[i] = empty;
    }
}

std::vector<int>* LinearProbingWithCounter::getCounters() {
    std::vector<int>* res = new std::vector<int>();
    res->push_back(probeCount);
    res->push_back(searchProbeCount);
    return res;
}

void LinearProbingWithCounter::resetCounters() {
    probeCount = 0;
    searchProbeCount = 0;
}

void LinearProbingWithCounter::reinitHashFunction() {
	delete hashFunction;
	hashFunction = initHash(hash_name);
}

void LinearProbingWithCounter::cleanHashFunctions() {
    for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		delete (*sequenceOfHashFunctions)[i];
	}
    sequenceOfHashFunctions->clear();
}