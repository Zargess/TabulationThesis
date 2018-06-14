#include "RealDataGenerator.h"
#include "../randomnumbergenerator/CRandNumberGenerator.h"
#include "../randomnumbergenerator/RandomNumberGenerator.h"
#include <assert.h>
#include <algorithm>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <bitset>
#include <stdint.h>

uint32_t MNISTDataGenerator::getInt(std::vector<char>& buffer, uint32_t pos) {
    uint32_t a(0);
    assert(pos+4 <= buffer.size());
    for(uint32_t i = 0;i < 4; ++i) {
        int32_t t = (int32_t)buffer[pos+i];
        if(t < 0) t += (1 << 8);
        a = (a << 8) + t;
    }
    return a;
}

void MNISTDataGenerator::readImages(std::string fileName) {
    std::ifstream input(fileName, std::ios::binary|std::ios::in);
    std::vector<char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
    assert(getInt(buffer,0) == 2051);
    uint32_t n = getInt(buffer,4);
    uint32_t rows = getInt(buffer, 8);
    uint32_t cols = getInt(buffer, 12);
    assert(rows == 28);
    assert(cols == 28);
    assert(n*rows*cols+16 == buffer.size());
    uint32_t size = rows*cols;
    images->resize(n,std::vector<uint8_t>(size));
    for(uint32_t i = 0;i < n; ++i) {
        for(uint32_t j = 0;j < size; ++j) {
            (* images)[i][j] = buffer[(i*size)+16+j];
        }
    }
    input.close();
}

MNISTDataGenerator::MNISTDataGenerator() {
    std::string filename = "src/datagenerator/data/mnist-train.idx3-ubyte";
    images = new std::vector<std::vector<uint8_t>>();
    readImages(filename.c_str());
}

MNISTDataGenerator::~MNISTDataGenerator() {
    delete images;
}

uint32_t* MNISTDataGenerator::createDataSet(int n) {
    uint32_t* res = new uint32_t[n];
    
    uint8_t threashold = 1;
    uint32_t imageSize = 28*28;

    int counter = 0;

    for (uint32_t i = 0; i < images->size(); i++) {
        for (uint32_t j = 0; j < imageSize; j++) {
            if (counter >= n) {
                return res;
            }

            if ((*images)[i][j] < threashold) {
                continue;
            }

            uint32_t number = (i + 1) * j;
            bool isInRes = false;

            for (uint32_t k = 0; k < counter; k++) {
                if (number == res[k]) {
                    isInRes = true;
                    break;
                }
            }

            if (isInRes) {
                continue;
            }

            res[counter] = number;
            counter++;
        }
    }
    
    return res;
}

uint32_t MNISTDataGenerator::getNumberNotInData() {
}

//-------------------------------------------------------------

uint32_t BagOfWordsDataGenerator::wordToNumber(std::string s, uint32_t p) {
    uint32_t sum = 0;
    uint32_t factor = 1;

    for (int i = 0; i < s.size(); i++) {
        uint32_t part = (((uint32_t) s[i]) * factor) % p;
        sum = (sum + part) % p;
        factor = (factor * 255) % p;
    }
    return sum;
}

BagOfWordsDataGenerator::BagOfWordsDataGenerator(RandomNumberGenerator* rnd) {
    this->rnd = rnd;
    RandomPrimeGenerator rpg(rnd);
    uint32_t p = rpg.get(1000000000, 4200000000);

    // Dataset found at https://archive.ics.uci.edu/ml/datasets/Bag+of+Words
    std::string fileName = "src/datagenerator/data/vocab.pubmed.txt";
    std::ifstream infile(fileName);

    std::string line;
    while (std::getline(infile, line)) {
        uint32_t number = wordToNumber(line, p);
        wordsAsNumbers.push_back(number);
    }
    infile.close();

    this->notInData = NumberNotInData();
}

uint32_t* BagOfWordsDataGenerator::createDataSet(int n) {
    uint32_t* data = new uint32_t[n];
    
    assert(n <= wordsAsNumbers.size());
    
    int i = this->rnd->getNextNumber()  % wordsAsNumbers.size();
    
    int counter = 0;
    while(counter < n) {
        if(i == wordsAsNumbers.size()) {
            i = 0;
        }

        data[counter] = wordsAsNumbers[i];
        counter++;
        i++;
    }

    return data;
}

uint32_t BagOfWordsDataGenerator::NumberNotInData() {
    uint32_t res = UINT32_MAX;
    bool isInData = false;
    while(true) {
        for(std::vector<uint32_t>::iterator it = wordsAsNumbers.begin(); it!=wordsAsNumbers.end(); ++it) {
            if(*it == res) {
                isInData = true;
            }
        }
        if(!isInData) {
            return res;
        }
        res--;
    }
}

uint32_t BagOfWordsDataGenerator::getNumberNotInData() {
    return this->notInData;
}