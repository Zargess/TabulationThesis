#include "DataGenerator.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include <vector>
#include <set>
#include <string>
#pragma once

class MNISTDataGenerator : public DataGenerator {
    public:
        MNISTDataGenerator();
        ~MNISTDataGenerator();
        uint32_t* createDataSet(int n);

        uint32_t getNumberNotInData();
    private:
        std::vector<std::vector<uint8_t>>* images;
        uint32_t getInt(std::vector<char>& buffer, uint32_t pos);
        void readImages(std::string fileName);
};

class BagOfWordsDataGenerator : public DataGenerator {
    public:
        BagOfWordsDataGenerator(RandomNumberGenerator* rnd);
        uint32_t* createDataSet(int n);
        uint32_t getNumberNotInData();
    private:
        uint32_t NumberNotInData();
        RandomNumberGenerator* rnd;
        std::vector<uint32_t> wordsAsNumbers;
        uint32_t wordToNumber(std::string s, uint32_t p);
        uint32_t notInData;
};

class TwentyNewsgroupDataGenerator : public DataGenerator {
    public:
        TwentyNewsgroupDataGenerator();
        uint32_t* createDataSet(int n);

        uint32_t getNumberNotInData();
    private:
        std::set<uint32_t> numbers;
};