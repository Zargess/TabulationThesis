#include "SimpleMixedTimeExperiment.h"
#include "../randomnumbergenerator/CRandNumberGenerator.h"
#include "../datagenerator/SyntheticDataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include <iostream>
#include <math.h>
SimpleMixedTimeExperiment::SimpleMixedTimeExperiment()  {
    int n = pow(10, 7); 
    int numRepeats = 50;
    RandomNumberGenerator* rnd = new CRandNumberGenerator();
    DataGenerator* gen = new  OneToNDataGenerator();
    std::string filename = "TimeDefault";
    SimpleMixedTimeExperiment(n, numRepeats, rnd, gen, filename);
}

SimpleMixedTimeExperiment::SimpleMixedTimeExperiment(int n, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename) {
    this->rnd = rnd;
    this->gen = g;
    init(n, numRepeats, filename);
}

SimpleMixedTimeExperiment::~SimpleMixedTimeExperiment() {
    clean();
}
void SimpleMixedTimeExperiment::clean() {
    delete simple;
    delete mixed;
    delete ms;
    delete p2;
    delete p3;   
}
void SimpleMixedTimeExperiment::setup() {
    simple = new SimpleTabulation(rnd);
    mixed = new MixedTabulation(rnd);
    ms = new MultiplyShift(rnd, 32);
    p2 = new PolyHash2_Mersenne(rnd); 
    p3 = new PolyHash3_Mersenne(rnd); 
}

void SimpleMixedTimeExperiment::run() {
    simpleTimeSum = 0;
    mixedTimeSum = 0;
    msTimeSum = 0;
    p2TimeSum = 0;
    p3TimeSum = 0;
    uint32_t res = 0;
    uint64_t g = 0;
    
    for(int j = 0; j < numRepeats; j++) {
        if(j % 1000 == 0) {
            std::cout << "Iteration: " << j+1 << "/" << numRepeats  << std::endl;
        }
        this->data = gen->createDataSet(n);
        
        auto begin = std::chrono::high_resolution_clock::now();        
        for(int i = 0; i < n; i++) {       
            res = simple->hash(data[i]);
        } 
        auto endTime = std::chrono::high_resolution_clock::now(); 
        auto elapsed_millisecs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - begin).count();
        simpleTimeSum += elapsed_millisecs;
        g += res;

        begin = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < n; i++) {       
            res = mixed->hash(data[i]);
        }
        endTime = std::chrono::high_resolution_clock::now(); 
        elapsed_millisecs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - begin).count();
        mixedTimeSum += elapsed_millisecs;
        g += res;

        begin = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < n; i++) {       
            res = ms->hash(data[i]);
        }
        endTime = std::chrono::high_resolution_clock::now(); 
        elapsed_millisecs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - begin).count();
        msTimeSum += elapsed_millisecs;
        g += res;

        begin = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < n; i++) {       
            res = p2->hash(data[i]);   
        }
        endTime = std::chrono::high_resolution_clock::now(); 
        elapsed_millisecs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - begin).count();
        p2TimeSum += elapsed_millisecs;
        g += res;

        begin = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < n; i++) {       
            res = p3->hash(data[i]);
        }
        endTime = std::chrono::high_resolution_clock::now(); 
        elapsed_millisecs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - begin).count();
        p3TimeSum += elapsed_millisecs;
        g += res;

        delete[] data;
    }
}

void SimpleMixedTimeExperiment::store() {
    std::ofstream myfile;
    std::string path = "data/HashFunctionTiming/" + filename + ".txt";
    myfile.open(path);
    myfile << "average runtime \n" << "--- \n";
    myfile << "Simple Tabulation, " << (simpleTimeSum / 1000000.0 / (double) numRepeats ) << "\n";
    myfile << "Mixed Tabulation, " << (mixedTimeSum / 1000000.0 / (double) numRepeats) << "\n";
    myfile << "Multiply Shift, " << (msTimeSum / 1000000.0 / (double) numRepeats) << "\n";
    myfile << "PolyHash 2, " << (p2TimeSum / 1000000.0 / (double) numRepeats) << "\n";
    myfile << "PolyHash 3, " << (p3TimeSum / 1000000.0 / (double) numRepeats) << "\n";
    myfile.close();
}