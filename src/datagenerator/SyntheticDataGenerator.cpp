#include "SyntheticDataGenerator.h"

uint32_t* OneToNDataGenerator::createDataSet(int n) {
    uint32_t* data = new uint32_t[n];
    for(int i = 0; i < n; i++) {
        data[i] = i;
    }   

    return data;
}

uint32_t OneToNDataGenerator::getNumberNotInData() {
    return UINT32_MAX;
}

BadSimpleTabulationDataGenerator::BadSimpleTabulationDataGenerator(int k1, int k2, int k3, int k4) {
    this->k1 = k1;
    this->k2 = k2;
    this->k3 = k3;
    this->k4 = k4;
}

uint32_t BadSimpleTabulationDataGenerator::getNumberNotInData() {
    return UINT32_MAX;
}



uint32_t* BadSimpleTabulationDataGenerator::createDataSet(int n) {
    uint32_t* data = new uint32_t[n];
    int counter = 0;

    for (int i = 0; i < k1; i++) {
        for (int j = 0; j < k2; j++) {
            for (int k = 0; k < k3; k++) {
                for (int l = 0; l < k4; l++) {
                    if (counter >= n) {
                        return data;
                    }

                    uint32_t number = i;
                    number <<= 8;
                    number ^= j;
                    number <<= 8;
                    number ^= k;
                    number <<= 8;
                    number ^= l;

                    data[counter] = number;
                    counter++;
                }
            }
        }
    }

    return data;
}