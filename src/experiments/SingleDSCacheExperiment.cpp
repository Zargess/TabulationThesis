#include "SingleDSCacheExperiment.h"
#include "../randomnumbergenerator/CRandNumberGenerator.h"
#include "../datagenerator/SyntheticDataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include "../linearprobing/LinearProbing.h"
#include "../cuckoo/Cuckoo.h"
#include "../doublehashing/DoubleHashing.h"
#include "../hashtable/HashTable.h"
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include <iterator>
#include <algorithm>

SingleDSCacheExperiment::SingleDSCacheExperiment(int n, int size, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, std::string dsName) {
	this->rnd = rnd;
    this->gen = g;
	this->size = size;
	this->dsName = dsName;
    init(n, numRepeats, filename);
}

SingleDSCacheExperiment::~SingleDSCacheExperiment() {
}

void SingleDSCacheExperiment::setup() {

}

HashTable* SingleDSCacheExperiment::getNextDS(int counter) {
	std::string hashName;

	switch(counter) {
		case 0: 
			hashName = "simple";
			break;
		case 1:
			hashName = "mixed";
			break;
		case 2:
			hashName = "multshift";
			break;
		case 3:
			hashName = "poly2";
			break;
		case 4:
			hashName = "poly3";
			break;
		default:
			hashName = "simple";
			break;
		
	}
	
	if(dsName == "linear") {
		return new LinearProbing(size, hashName, rnd, gen->getNumberNotInData(), 32);
	} else if(dsName == "cuckoo") {
		return new Cuckoo(size, hashName, hashName, rnd, 32, gen->getNumberNotInData());
	} else if(dsName == "double") {
		return new DoubleHashing(size, hashName, hashName, rnd, gen->getNumberNotInData(), 32);
	}
}

void SingleDSCacheExperiment::run() {
	for(int j = 0; j < numRepeats; j++) {
		if (j % (numRepeats / 10) == 0) {
			std::cout << "Iteration " << j << std::endl;
		}
		uint32_t* data = gen->createDataSet(n);

		
		// Simple
		HashTable* table = getNextDS(0);
		for(int i = 0; i < n; i++) {
			table->insert(data[i]);
		}
		delete table;

		// Mixed
		table = getNextDS(1);
		for(int i = 0; i < n; i++) {
			table->insert(data[i]);
		}
		delete table;
		
		// MultShift
		table = getNextDS(2);
		for(int i = 0; i < n; i++) {
			table->insert(data[i]);
		}
		delete table;

		// P2
		table = getNextDS(3);
		for(int i = 0; i < n; i++) {
			table->insert(data[i]);
		}
		delete table;

		// P3
		table = getNextDS(4);
		for(int i = 0; i < n; i++) {
			table->insert(data[i]);
		}
		delete table;
		delete[] data;
	}
}

void SingleDSCacheExperiment::store() {

}

void SingleDSCacheExperiment::clean() {

}