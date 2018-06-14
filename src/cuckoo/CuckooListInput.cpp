#include <cstddef>
#include <string>
#include <iostream>
#include "../hashfunction/HashFunction.h"
#include "../hashfunction/Tabulation.h"
#include "../hashfunction/StandardHashing.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include "../randomnumbergenerator/CRandNumberGenerator.h"
#include "../hashtable/HashTableListInput.h"
#include "CuckooListInput.h"

CuckooListInput::CuckooListInput(int size, uint32_t notInData, std::vector<HashFunction*>* sequenceOfHashFunctions) {
    this->table = new uint32_t[size];
    this->hashFunctionIndex = 0;
	this->sequenceOfHashFunctions = sequenceOfHashFunctions;
    this->size = size;

    this->empty = notInData;

    for(int i = 0; i<size; i++) {
        table[i] = notInData;
    }

	reinitHashFunction();
}

CuckooListInput::CuckooListInput(uint32_t* table, int size, uint32_t notInData, HashFunction* h1, HashFunction* h2, std::vector<HashFunction*>* sequenceOfHashFunctions, int hashFunctionIndex) {
    this->table = new uint32_t[size];
    
    this->size = size;
    this->empty = notInData;
	this->h1 = h1;
	this->h2 = h2;
	this->sequenceOfHashFunctions = sequenceOfHashFunctions;
	this->hashFunctionIndex = hashFunctionIndex;

    for(int i = 0; i < size; i++) {
        this->table[i] = table[i];
    }

	reinitHashFunction();
}

CuckooListInput::~CuckooListInput() {
    delete[] table;
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		delete (*sequenceOfHashFunctions)[i];
	}
	delete sequenceOfHashFunctions;
}

HashTableListInput* CuckooListInput::clone() {
	std::vector<HashFunction*>* hash_funcs = new std::vector<HashFunction*>();
	
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		hash_funcs->push_back((*sequenceOfHashFunctions)[i]->clone());
	}

    return new CuckooListInput(table, size, empty, h1->clone(), h2->clone(), hash_funcs, hashFunctionIndex);
}

void CuckooListInput::rehash() {
    //make list of all elements currently in table
    //clean table
    std::vector<uint32_t> elements;
    for (int i = 0; i < this->size; i++) {
        if (table[i] == empty) {
            continue;
        }
        elements.push_back(table[i]);
    }    
    while(true) {
        clean();
		reinitHashFunction();
        
        //insert all elements.
        bool insertedSuccesfully = true;
        for (int i = 0; i < elements.size(); i++) {
            if(!rehashInsert(elements[i])) {
                insertedSuccesfully = false;
            }
        }
        if(insertedSuccesfully) {
            break;
        }
    }
}

void CuckooListInput::insert(uint32_t value) {
    int index1 = this->h1->hash(value) % this->size;
    
    if(this->table[index1] == value) {
        return ;
    }
    int index2 = this->h2->hash(value) % this->size;
    if (this->table[index2] == value) {
        return;
    }

    int pos = index1;
    int tempValue = 0;
    for(int i=0; i<this->size; i++) {
        if(this->table[pos] == empty) {
            this->table[pos] = value;
            return;
        }
        tempValue = table[pos];
        this->table[pos] = value;
        value = tempValue; 
        index1 = this->h1->hash(value) % this->size;
        index2 = this->h2->hash(value) % this->size;
        if(pos == index1) {
            pos = index2;
        } else {
            pos = index1;
        }
    }

    rehash();
    insert(value);
    return;
}

bool CuckooListInput::rehashInsert(uint32_t value) {
    int index1 = this->h1->hash(value) % this->size;

    if(this->table[index1] == value) {
        return true;
    }

    int index2 = this->h2->hash(value) % this->size;
    if (this->table[index2] == value) {
        return true;
    }

    int pos = index1;
    int tempValue = 0;
    for(int i=0; i<this->size; i++) {
        if(this->table[pos] == empty) {
            this->table[pos] = value;
            return true;
        }
        tempValue = table[pos];
        this->table[pos] = value;
        value = tempValue; 
        index1 = this->h1->hash(value) % this->size;
        index2 = this->h2->hash(value) % this->size;
        if(pos == index1) {
            pos = index2;
        } else {
            pos = index1;
        }
    }
    return false;
}

int CuckooListInput::search(uint32_t value) {
    int index1 = this->h1->hash(value) % size;
    if(this->table[index1] == value ) {
        return index1;
    }
    int index2 = this->h2->hash(value) % size;
    if(this->table[index2] == value) {
        return index2;
    }
    return -1;
}

void CuckooListInput::clean() {
    for(int i = 0; i < size; i++) {
        table[i] = empty;
    }
}

void CuckooListInput::reinitHashFunction() {
    h1 = (*sequenceOfHashFunctions)[hashFunctionIndex];
	hashFunctionIndex++;
    h2 = (*sequenceOfHashFunctions)[hashFunctionIndex];
	hashFunctionIndex++;
}