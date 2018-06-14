#include <cstdint>
#include <vector>
#pragma once
class HashTableListInput {
    public:
        virtual void clean() = 0;
        virtual void insert(uint32_t value) = 0;
        virtual int search(uint32_t value) = 0;
        virtual HashTableListInput* clone() = 0;
        virtual void reinitHashFunction() = 0;
        virtual ~HashTableListInput();
};