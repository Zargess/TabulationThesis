#include <cstddef>
#include <string>
#include <iostream>
#include "DoubleHashing.h"
#include "../hashtable/HashTable.h"
#include "../hashfunction/Tabulation.h"
#include "../hashfunction/StandardHashing.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

HashFunction * DoubleHashing::initHash(std::string hash) {
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

    return res;
}

DoubleHashing::DoubleHashing(int size, std::string hash_name1, std::string hash_name2, RandomNumberGenerator* rnd, uint32_t notInData, int outputBits) {
    table = new uint32_t[size];
    this->empty = notInData;

    for(int i = 0; i<size; i++) {
        table[i] = notInData;
    }
    
    
    this->size = size;
	this->hash_name1 = hash_name1;
    this->hash_name2 = hash_name2;
	this->outputBits = outputBits;
	this->rnd = rnd;
    this->hashFunction1 = initHash(hash_name1);
    this->hashFunction2 = initHash(hash_name2);
}

DoubleHashing::DoubleHashing(uint32_t* table, int size, std::string hash_name1, std::string hash_name2, RandomNumberGenerator* rnd, HashFunction* hashFunction1, HashFunction* hashFunction2, uint32_t notInData, int outputBits) {
	
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
}

DoubleHashing::~DoubleHashing() {
	delete[] table;
	delete hashFunction1;
    delete hashFunction2;
}

HashTable* DoubleHashing::clone() {
	return new DoubleHashing(table, size, hash_name1, hash_name2, rnd, hashFunction1->clone(), hashFunction2->clone(), empty, outputBits);
}

void DoubleHashing::insert(uint32_t value) {
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
        index = (index + h2) % size;
    }

    if (table[index] == value) {
        return;
    }
    
    table[index] = value;
}

int DoubleHashing::search(uint32_t value) {
    uint32_t h1 = hashFunction1->hash(value) % size;
    
    if (table[h1] == value) {
        return h1;
    }
    
    uint32_t h2 = (hashFunction2->hash(value) % (size - 1)) + 1;
    uint32_t index = (h1 + h2) % size;

    while (table[index] != value) {
        index = (index + h2) % size;
    }

    return index;
}

void DoubleHashing::clean() {
    for(int i = 0; i < size; i++) {
        table[i] = empty;
    }
}

void DoubleHashing::reinitHashFunction() {
	delete hashFunction1;
    delete hashFunction2;
	hashFunction1 = initHash(hash_name1);
	hashFunction2 = initHash(hash_name2);
}