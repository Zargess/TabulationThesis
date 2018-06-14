#include "DSWorstCaseExperiment.h"
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

DSWorstCaseExperiment::DSWorstCaseExperiment() {

}

DSWorstCaseExperiment::DSWorstCaseExperiment(int n, RandomNumberGenerator* rnd, DataGenerator* gen, std::string filename, std::string ds, int loadFactorPercentage, int numDS) {
	this->rnd = rnd;
	this->gen = gen;
	this->size = (n / (loadFactorPercentage / 100.0));
	this->dsString = ds;
	this->numDS = numDS;
	init(n, 0, filename);
}

DSWorstCaseExperiment::~DSWorstCaseExperiment() {
	clean();
}

void DSWorstCaseExperiment::clean() {
	delete simple;
	delete mixed;
	delete multShift;
	delete polyHash2;
	delete polyHash3;
}

void DSWorstCaseExperiment::makeDS() {
	if(dsString == "cuckoo") {
	    simple = new Cuckoo(size, "simple", "simple", rnd, 32, gen->getNumberNotInData());
    	mixed = new Cuckoo(size, "mixed", "mixed", rnd, 32, gen->getNumberNotInData());
    	multShift = new Cuckoo(size, "multshift", "multshift", rnd, 32, gen->getNumberNotInData());
    	polyHash2 = new Cuckoo(size, "poly2", "poly2", rnd, 32, gen->getNumberNotInData());
     	polyHash3 = new Cuckoo(size, "poly3", "poly3", rnd, 32, gen->getNumberNotInData());
	} else if(dsString == "probing") {
		simple = new LinearProbing(size, "simple", rnd, gen->getNumberNotInData(), 32);
		mixed = new LinearProbing(size, "mixed", rnd, gen->getNumberNotInData(), 32);
		multShift = new LinearProbing(size, "multshift", rnd, gen->getNumberNotInData(), 32);
		polyHash2 = new LinearProbing(size, "poly2", rnd, gen->getNumberNotInData(), 32);
		polyHash3 = new LinearProbing(size, "poly3", rnd, gen->getNumberNotInData(), 32);
	} 
}

std::vector<HashTable*>* DSWorstCaseExperiment::createCopies(HashTable* table) {
	std::vector<HashTable*>* res = new std::vector<HashTable*>();

	for (int i = 0; i < numDS; i++) {

		res->push_back(table->clone());
	}

	return res;
}

uint64_t DSWorstCaseExperiment::timeWorstCase(uint32_t* data, HashTable* table) {


	std::vector<HashTable*>* clones = createCopies(table);


	uint64_t result = 0;

	for (int i = 0; i < n; i++) {
		if (i % 10000 == 0) {
			std::cout << "Looking at element " << i << std::endl;
		}
		auto begin = std::chrono::high_resolution_clock::now();		
		for (int j = 0; j < numDS; j++) {
			(*clones)[j]->search(data[i]);
		} 

		auto endTime = std::chrono::high_resolution_clock::now(); 
		auto elapsed_millisecs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - begin).count();

		if (elapsed_millisecs > result) {
			result = elapsed_millisecs;
		}
	}

	for(int i = 0; i < numDS; i++) {
		delete (*clones)[i];
	}

	delete clones;

	return result / numDS;
}

void DSWorstCaseExperiment::setup() {
	makeDS();
}

void DSWorstCaseExperiment::insertAllElements(uint32_t* data, HashTable* table) {
	for(int i = 0; i < n; i++) {
		table->insert(data[i]);
	}
}

void DSWorstCaseExperiment::run() {
	simpleWorstCase = 0;
	mixedWorstCase = 0;
	msWorstCase = 0;
	p2WorstCase = 0;
	p3WorstCase = 0;
	uint32_t* data = gen->createDataSet(n);

	std::cout << "Starting simple" << std::endl;

	insertAllElements(data, simple);
	simpleWorstCase = timeWorstCase(data, simple);
	simple->clean();

	std::cout << "Starting mixed" << std::endl;

	insertAllElements(data, mixed);
	mixedWorstCase = timeWorstCase(data, mixed);
	mixed->clean();

	std::cout << "Starting multshift" << std::endl;

	insertAllElements(data, multShift);
	msWorstCase = timeWorstCase(data, multShift);
	multShift->clean();

	std::cout << "Starting poly 2" << std::endl;

	insertAllElements(data, polyHash2);
	std::cout << "Inserted all elements" << std::endl;
	p2WorstCase = timeWorstCase(data, polyHash2);
	polyHash2->clean();

	std::cout << "Starting poly 3" << std::endl;

	insertAllElements(data, polyHash3);
	p3WorstCase = timeWorstCase(data, polyHash3);
	polyHash3->clean();
}

void DSWorstCaseExperiment::store() {
	std::ofstream myfile;
    std::string path = "data/DS/worstcase/" + filename + ".txt";
    myfile.open(path);

	myfile << "Worst Case \n";
	myfile << "--- \n" << "Simple Tab \n";
	myfile << size << "," << simpleWorstCase / 1000.0 << "\n";
	myfile << "--- \n" << "Mixed Tab \n";
	myfile << size << "," << mixedWorstCase / 1000.0 << "\n";
	myfile << "--- \n" << "MultShift \n";
	myfile << size << "," << msWorstCase / 1000.0 << "\n";
	myfile << "--- \n" << "PolyHash2 \n";
	myfile << size << "," << p2WorstCase / 1000.0 << "\n";
	myfile << "--- \n" << "PolyHash3 \n";
	myfile << size << "," << p3WorstCase / 1000.0 << "\n";

	myfile.close();
}