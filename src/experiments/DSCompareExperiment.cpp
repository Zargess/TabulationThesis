#include "DSCompareExperiment.h"
#include "../randomnumbergenerator/CRandNumberGenerator.h"
#include "../datagenerator/SyntheticDataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include "../linearprobing/LinearProbing.h"
#include "../cuckoo/Cuckoo.h"
#include "../hashtable/HashTable.h"
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include <iterator>
#include <algorithm>

DSCompareExperiment::DSCompareExperiment() {
    int n = 1000;
    int numRepeats = 3;
    std::string filename = "DSCompareExperiment";
	RandomNumberGenerator* unirnd = new UniformDistMT();
    RandomNumberGenerator * rnd = new PolyHashRNG(unirnd, 20);
    DataGenerator* gen = new BadSimpleTabulationDataGenerator(1,1,2,2); 
	std::string hashName = "simple";
	int loadFactorPercentage = 50;
	int numQueries = 100000;
	std::vector<double>* querySizes = new std::vector<double>();
	DSCompareExperiment(n, numRepeats, rnd, gen, filename, hashName, loadFactorPercentage, querySizes);
}

DSCompareExperiment::DSCompareExperiment(int n, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename, std::string hashName, int loadFactorPercentage, std::vector<double>* querySizes) {
    this->rnd = rnd;
    this->gen = g;
	this->size = (n / (loadFactorPercentage / 100.0));
	this->hashName = hashName;
	this->querySizes = querySizes;
    init(n, numRepeats, filename);
}

void DSCompareExperiment::clean() {
	delete[] cuckooInsertTimes;
	delete[] lpInsertTimes;
	delete[] cuckooQueryTimes;
	delete[] lpQueryTimes;
}

DSCompareExperiment::~DSCompareExperiment() {
	clean();
}

void DSCompareExperiment::makeDS() {
	cuckoo = new Cuckoo(size, hashName, hashName, rnd, 32, gen->getNumberNotInData());
	linearProbing = new LinearProbing(size, hashName, rnd, gen->getNumberNotInData(), 32);
}


void DSCompareExperiment::setup() {
	cuckooInsertTimes = new uint64_t[querySizes->size()]{0};
    lpInsertTimes = new uint64_t[querySizes->size()]{0};

	cuckooQueryTimes = new uint64_t[querySizes->size()]{0};
    lpQueryTimes = new uint64_t[querySizes->size()]{0};
}

uint64_t DSCompareExperiment::timeHashTableInsert(uint32_t* data, HashTable* table) {
	uint64_t result = 0;
	auto begin = std::chrono::high_resolution_clock::now();        
			
	for(int i = 0; i < n; i++) {
		table->insert(data[i]);
	} 
	auto endTime = std::chrono::high_resolution_clock::now(); 
	auto elapsed_millisecs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - begin).count();
	result += elapsed_millisecs;
	return result;
}


uint64_t DSCompareExperiment::timeHashTableQuery(std::vector<uint32_t>* queryData, HashTable* table) {
	uint64_t result = 0;
	auto begin = std::chrono::high_resolution_clock::now();        
	for(int i = 0; i < numQueries; i++) {       
		table->search((*queryData)[i]);
	} 
	auto endTime = std::chrono::high_resolution_clock::now(); 
	auto elapsed_millisecs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - begin).count();
	result += elapsed_millisecs;
	return result;
}

std::vector<uint32_t>* DSCompareExperiment::generateQueryData(uint32_t* data) {
	std::vector<uint32_t>* result = new std::vector<uint32_t>();
	for(int i = 0; i < numQueries; i++) {
		int index = rnd->getNextNumber() % n;
		result->push_back(data[index]);
	}
	return result;
}


void DSCompareExperiment::run() {
	for(int j = 0; j<querySizes->size(); j++) {
		numQueries = (*querySizes)[j] * n;
		std::cout << "numqueriers: " << numQueries  << std::endl;
		for(int i = 0; i < numRepeats; i++) {
			makeDS();
			if(i % 1 == 0) {
				std::cout << "Iteration: " << i+1 << "/" << numRepeats  << std::endl;
			}
			
			uint32_t* data = gen->createDataSet(n);
			std::vector<uint32_t>* queryData = generateQueryData(data);

			cuckooInsertTimes[j] += timeHashTableInsert(data, cuckoo);
			lpInsertTimes[j] += timeHashTableInsert(data, linearProbing);

			cuckooQueryTimes[j] +=  timeHashTableQuery(queryData, cuckoo);
			lpQueryTimes[j] +=  timeHashTableQuery(queryData, linearProbing);

			delete cuckoo;
			delete linearProbing;

			delete[] data;
			delete queryData;
		}
		cuckooInsertTimes[j] = cuckooInsertTimes[j] / numRepeats;
		lpInsertTimes[j] = lpInsertTimes[j] / numRepeats;

		cuckooQueryTimes[j] =  cuckooQueryTimes[j] / numRepeats;
		lpQueryTimes[j] =  lpQueryTimes[j] / numRepeats;
	}
}



void DSCompareExperiment::store() {
	std::ofstream myfile;
    std::string path = "data/DS/compare/" + hashName + "/" + filename + ".txt";
    myfile.open(path);
    
	myfile << "Compare time \n";
	myfile << "--- \n" << "Cuckoo" << "\n";
	for(int i = 0; i < querySizes->size(); i++) {
		myfile << (*querySizes)[i] << ", " << cuckooInsertTimes[i]  / 1000000.0 << ", " << + cuckooQueryTimes[i] / 1000000.0 << std::endl;
	}

	myfile << "--- \n" << "Linear Probing" << "\n";
	for(int i = 0; i < querySizes->size(); i++) {
		myfile << (*querySizes)[i] << ", " << lpInsertTimes[i] / 1000000.0 << ", " << lpQueryTimes[i] / 1000000.0 << std::endl;
	}

	myfile.close();

}