#include <cstddef>
#include <string>
#include <iostream>
#include "DoubleHashingListInput.h"
#include "../hashtable/HashTableListInput.h"
#include "../hashfunction/Tabulation.h"
#include "../hashfunction/StandardHashing.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"

DoubleHashingListInput::DoubleHashingListInput(int size, uint32_t notInData, std::vector<HashFunction*>* sequenceOfHashFunctions) {
    table = new uint32_t[size];
    this->empty = notInData;

    for(int i = 0; i<size; i++) {
        table[i] = notInData;
    }
    
    this->size = size;
	this->sequenceOfHashFunctions = sequenceOfHashFunctions;
    hashFunctionIndex = 0;
	reinitHashFunction();
}

DoubleHashingListInput::DoubleHashingListInput(uint32_t* table, int size, HashFunction* hashFunction1, HashFunction* hashFunction2, uint32_t notInData, std::vector<HashFunction*>* sequenceOfHashFunctions, int hashFunctionIndex) {
	this->table = new uint32_t[size];
    this->empty = notInData;

    for(int i = 0; i < size; i++) {
        this->table[i] = table[i];
    }

    this->size = size;
	this->sequenceOfHashFunctions = sequenceOfHashFunctions;
	this->hashFunction1 = hashFunction1;
	this->hashFunction2 = hashFunction2;
	this->hashFunctionIndex = hashFunctionIndex;
}

DoubleHashingListInput::~DoubleHashingListInput() {
	delete[] table;
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		delete (*sequenceOfHashFunctions)[i];
	}
	delete sequenceOfHashFunctions;
}

HashTableListInput* DoubleHashingListInput::clone() {
	std::vector<HashFunction*>* hash_funcs = new std::vector<HashFunction*>();
	
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		hash_funcs->push_back((*sequenceOfHashFunctions)[i]->clone());
	}

	return new DoubleHashingListInput(table, size, hashFunction1->clone(), hashFunction2->clone(), empty, hash_funcs, hashFunctionIndex);
}

void DoubleHashingListInput::insert(uint32_t value) {
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

int DoubleHashingListInput::search(uint32_t value) {
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

void DoubleHashingListInput::clean() {
    for(int i = 0; i < size; i++) {
        table[i] = empty;
    }
}

void DoubleHashingListInput::reinitHashFunction() {
	hashFunction1 = (*sequenceOfHashFunctions)[hashFunctionIndex];
	hashFunctionIndex++;
	hashFunction2 = (*sequenceOfHashFunctions)[hashFunctionIndex];
	hashFunctionIndex++;
}