#include <cstddef>
#include <string>
#include <iostream>
#include "LinearProbing.h"
#include "../hashtable/HashTable.h"
#include "../hashfunction/Tabulation.h"
#include "../hashfunction/StandardHashing.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

HashFunction * LinearProbing::initHash(std::string hash) {
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

LinearProbing::LinearProbing(int size, std::string hash_name, RandomNumberGenerator* rnd, uint32_t notInData, int outputBits) {
    table = new uint32_t[size];
    this->empty = notInData;

    for(int i = 0; i<size; i++) {
        table[i] = notInData;
    }
    
    
    this->size = size;
	this->hash_name = hash_name;
	this->outputBits = outputBits;
	this->rnd = rnd;
    this->hashFunction = initHash(hash_name);
}

LinearProbing::LinearProbing(uint32_t* table, int size, std::string hash_name, RandomNumberGenerator* rnd, HashFunction* hashFunction, uint32_t notInData, int outputBits) {
	
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
}

LinearProbing::~LinearProbing() {
	delete[] table;
	delete hashFunction;
}

HashTable* LinearProbing::clone() {
	return new LinearProbing(table, size, hash_name, rnd, hashFunction->clone(), empty, outputBits);
}

void LinearProbing::insert(uint32_t value) {
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
    }
    return;
}

int LinearProbing::search(uint32_t value) {
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
    }
    return -1;
}

void LinearProbing::clean() {
    for(int i = 0; i < size; i++) {
        table[i] = empty;
    }
}

void LinearProbing::reinitHashFunction() {
	delete hashFunction;
	hashFunction = initHash(hash_name);
}