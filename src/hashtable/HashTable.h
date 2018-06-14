#include <cstdint>
#include <vector>
#pragma once
class HashTable {
    public:
        virtual void insert(uint32_t value) = 0;
        virtual int search(uint32_t value) = 0;
        virtual void clean() = 0;
        virtual void reinitHashFunction() = 0;
        virtual HashTable* clone() = 0;
        virtual ~HashTable();
};