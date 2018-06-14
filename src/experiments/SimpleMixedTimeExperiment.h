#include "Experiment.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include "../datagenerator/DataGenerator.h"
#include "../hashfunction/Tabulation.h"
#include "../hashfunction/StandardHashing.h"

#pragma once
class SimpleMixedTimeExperiment : public Experiment {
    public:
        SimpleMixedTimeExperiment();
        SimpleMixedTimeExperiment(int n, int numRepeats, RandomNumberGenerator* rnd, DataGenerator* g, std::string filename);
        ~SimpleMixedTimeExperiment();
        void setup();
        void run();
        void store();
        void clean();
    private:
        RandomNumberGenerator * rnd;
        DataGenerator * gen;

        HashFunction * simple;
        HashFunction * mixed;
        HashFunction * ms;
        HashFunction * p2;
        HashFunction * p3;
        
        uint32_t* data;
        int64_t simpleTimeSum;
        int64_t mixedTimeSum;
        int64_t msTimeSum;
        int64_t p2TimeSum;
        int64_t p3TimeSum;
};