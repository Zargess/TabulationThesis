#include "DataGenerator.h"
#pragma once
class OneToNDataGenerator : public DataGenerator {
    public:
        uint32_t* createDataSet(int n);
        uint32_t getNumberNotInData();

};

class BadSimpleTabulationDataGenerator : public DataGenerator {
    public:
        BadSimpleTabulationDataGenerator(int k1, int k2, int k3, int k4);
        uint32_t* createDataSet(int n);
        uint32_t getNumberNotInData();
    private:
        uint32_t k1;
        uint32_t k2;
        uint32_t k3;
        uint32_t k4;
};