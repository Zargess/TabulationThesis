#include <cstddef>
#include <string>
#include <iostream>
#include "LinearProbingListInput.h"
#include "../hashtable/HashTableListInput.h"
#include "../hashfunction/Tabulation.h"
#include "../hashfunction/StandardHashing.h"

LinearProbingListInput::LinearProbingListInput(int size, uint32_t notInData, std::vector<HashFunction*>* sequenceOfHashFunctions) {
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

LinearProbingListInput::LinearProbingListInput(uint32_t* table, int tableSize, HashFunction* hashFunction, uint32_t notInData, std::vector<HashFunction*>* sequenceOfHashFunctions, int hashFunctionIndex) {
	this->table = new uint32_t[size];
    this->empty = notInData;

    for(int i = 0; i < size; i++) {
        this->table[i] = table[i];
    }

    this->size = size;
	this->sequenceOfHashFunctions = sequenceOfHashFunctions;
	this->hashFunction = hashFunction;
	this->hashFunctionIndex = hashFunctionIndex;
}

LinearProbingListInput::~LinearProbingListInput() {
	delete[] table;
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		delete (*sequenceOfHashFunctions)[i];
	}
	delete sequenceOfHashFunctions;
}

HashTableListInput* LinearProbingListInput::clone() {
	std::vector<HashFunction*>* hash_funcs = new std::vector<HashFunction*>();
	
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		hash_funcs->push_back((*sequenceOfHashFunctions)[i]->clone());
	}

	return new LinearProbingListInput(table, size, hashFunction, empty, hash_funcs, hashFunctionIndex);
}

void LinearProbingListInput::insert(uint32_t value) {
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

int LinearProbingListInput::search(uint32_t value) {
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

void LinearProbingListInput::clean() {
    for(int i = 0; i < size; i++) {
        table[i] = empty;
    }
}

void LinearProbingListInput::reinitHashFunction() {
	hashFunction = (*sequenceOfHashFunctions)[hashFunctionIndex];
	hashFunctionIndex++;
}