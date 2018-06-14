#include <cstddef>
#include <string>
#include <iostream>
#include "../hashfunction/HashFunction.h"
#include "../hashfunction/Tabulation.h"
#include "../hashfunction/StandardHashing.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include "../randomnumbergenerator/CRandNumberGenerator.h"
#include "../hashtable/HashTableCounter.h"
#include "CuckooWithCounter.h"

HashFunction * CuckooWithCounter::initHash(std::string hash) {
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

CuckooWithCounter::CuckooWithCounter(int size, std::string h1_name, std::string h2_name, RandomNumberGenerator* rnd, int outputBits, uint32_t notInData) {
    this->table = new uint32_t[size];
    
    this->outputBits = outputBits;
    this->rnd = rnd;

    this->size = size;
    this->h1_name = h1_name;
    this->h2_name = h2_name;

    this->empty = notInData;
    probeCount = 0;
    searchProbeCount = 0;

    for(int i = 0; i<size; i++) {
        table[i] = notInData;
    }
    sequenceOfHashFunctions = new std::vector<HashFunction*>();
    this->h1 = initHash(h1_name);
    this->h2 = initHash(h2_name);
	
}

CuckooWithCounter::CuckooWithCounter(uint32_t* table, int size, std::string h1, std::string h2, RandomNumberGenerator* rnd, int outputBits, uint32_t notInData, HashFunction* h1_func, HashFunction* h2_func, int probeCount, int searchProbeCount, std::vector<HashFunction*>* sequenceOfHashFunctions) {
    this->table = new uint32_t[size];
    this->outputBits = outputBits;
    this->rnd;
    this->h1 = h1_func;
    this->h2 = h2_func;
    this->size = size;
    this->h1_name = h1;
    this->h2_name = h2;
    this->empty = notInData;
    this->probeCount = probeCount;
    this->searchProbeCount = searchProbeCount;

    for(int i = 0; i < size; i++) {
        this->table[i] = table[i];
    }
	this->sequenceOfHashFunctions = new std::vector<HashFunction*>();
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		this->sequenceOfHashFunctions->push_back((* sequenceOfHashFunctions)[i]->clone());
	}
}

CuckooWithCounter::~CuckooWithCounter() {
    delete[] table;
    delete h1;
    delete h2;

	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		delete (*sequenceOfHashFunctions)[i];
	}
	delete sequenceOfHashFunctions;
}

HashTableCounter* CuckooWithCounter::clone() {
	return new CuckooWithCounter(table, size, h1_name, h2_name, rnd, outputBits, empty, h1->clone(), h2->clone(), probeCount, searchProbeCount, getHashFunctions());
}


void CuckooWithCounter::rehashWithCounter() {
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
            if(!rehashInsertWithCounter(elements[i])) {
                insertedSuccesfully = false;
            }
        }
        if(insertedSuccesfully) {
            break;
        }
    }
}


void CuckooWithCounter::insertWithCounter(uint32_t value) {
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
        this->probeCount++;
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

    rehashWithCounter();
    insertWithCounter(value);
    return;
}

int CuckooWithCounter::searchWithCounter(uint32_t value) {
    int index1 = this->h1->hash(value) % size;
    if(this->table[index1] == value ) {
        return index1;
    }
    searchProbeCount++;
    int index2 = this->h2->hash(value) % size;
    if(this->table[index2] == value) {
        return index2;
    }
    return -1;
}


bool CuckooWithCounter::rehashInsertWithCounter(uint32_t value) {
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
        this->probeCount++;
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

void CuckooWithCounter::clean() {
    for(int i = 0; i < size; i++) {
        table[i] = empty;
    }
}

std::vector<int>* CuckooWithCounter::getCounters() {
    std::vector<int>* res = new std::vector<int>();

    res->push_back(probeCount);
    res->push_back(searchProbeCount);

    return res;
}

void CuckooWithCounter::resetCounters() {
    probeCount = 0;
    searchProbeCount = 0;
}

void CuckooWithCounter::reinitHashFunction() {
	delete h1;
	delete h2;
    h1 = initHash(h1_name);
    h2 = initHash(h2_name);
}

std::vector<HashFunction*>* CuckooWithCounter::getHashFunctions() {
	std::vector<HashFunction*>* res = new std::vector<HashFunction*>();
	for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		res->push_back((* sequenceOfHashFunctions)[i]->clone());
	}
	return res;
}

void CuckooWithCounter::cleanHashFunctions() {
    for(int i = 0; i < sequenceOfHashFunctions->size(); i++) {
		delete (*sequenceOfHashFunctions)[i];
	}
    sequenceOfHashFunctions->clear();
}