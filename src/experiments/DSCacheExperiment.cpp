#include "DSCacheExperiment.h"
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

DSCacheExperiment::DSCacheExperiment(int n, int size, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, std::string hashName) {
	this->rnd = rnd;
    this->gen = g;
	this->size = size;
	this->hashName = hashName;
    init(n, numRepeats, filename);
}

DSCacheExperiment::~DSCacheExperiment() {
}

void DSCacheExperiment::setup() {

}

void DSCacheExperiment::run() {
	for(int i = 0; i < numRepeats; i++) {
		if (i % (numRepeats / 100) == 0) {
			std::cout << "Iteration " << i << std::endl;
		}
		uint32_t* data = gen->createDataSet(n);

		if (size != 131221) {
			Cuckoo cuckoo(size, hashName, hashName, rnd, 32, gen->getNumberNotInData());
			for(int i = 0; i < n; i++) {
				cuckoo.insert(data[i]);
			}
		}

		LinearProbing linear(size, hashName, rnd, gen->getNumberNotInData(), 32);
		for(int i = 0; i < n; i++) {
			linear.insert(data[i]);
		}

		DoubleHashing doublehashing(size, hashName, hashName, rnd, gen->getNumberNotInData(), 32);
		for(int i = 0; i < n; i++) {
			doublehashing.insert(data[i]);
		}
		
		delete[] data;
	}
}

void DSCacheExperiment::store() {

}

void DSCacheExperiment::clean() {

}