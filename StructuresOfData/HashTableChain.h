#include <iostream>
#include <queue>
#include <random>
#include "Polynomials.h"
#include "tlist.h"
#include "Basedata.h"

#pragma once


class HashTableChanes {
private:
    struct Entry {
        std::string name;
        Polynomials data;
        Entry() {};
        Entry(const std::string& n, const Polynomials& d) {
            name = n;
            data = d;
        }
        bool operator== (Entry& right) {
            if (name == right.name) {
                return 1;
            }
            else {
                return 0;
            }
        }
    };

    std::vector<Entry>** data;
    int randA, randB;
    int num;
    int count;


    bool hasName(std::string n) {

    }
    const uint64_t PRIME1 = 11400714785074694791ULL;
    const uint64_t PRIME2 = 14029467366897019727ULL;
    const uint64_t PRIME3 = 1609587929392839161ULL;
    const uint64_t PRIME4 = 9650029242287828579ULL;
    const uint64_t PRIME5 = 2870177450012600261ULL;
    inline uint64_t rotl64(uint64_t x, int r) {
        return (x << r) | (x >> (64 - r));
    }
    inline uint64_t mergeRound(uint64_t h, uint64_t v) {
        h ^= rotl64(v * PRIME2, 31) * PRIME1;
        h = h * PRIME1 + PRIME4;
        return h;
    }

    uint64_t hash(const std::string& s) {


        uint64_t v1 = PRIME1 + PRIME2;
        uint64_t v2 = PRIME2;
        uint64_t v3 = 0;
        uint64_t v4 = PRIME1;

        uint64_t hash = 0;
        uint64_t d;
        int k = 0;
        if (s.length() >= 32) {
            while (k < s.length() - 32) {
                uint64_t r1 = 0, r2 = 0, r3 = 0, r4 = 0;
                for (int i = 0; i < 8; i++) {
                    d = static_cast<uint8_t>(s[k + i]);
                    r1 |= d << (8 * i);
                    d = static_cast<uint8_t>(s[k + i + 8]);
                    r2 |= d << (8 * i);
                    d = static_cast<uint8_t>(s[k + i + 16]);
                    r3 |= d << (8 * i);
                    d = static_cast<uint8_t>(s[k + i + 24]);
                    r4 |= d << (8 * i);
                }

                k += 32;

                v1 += r1 * PRIME2;
                v1 = rotl64(v1, 31);
                v1 *= PRIME1;

                v2 += r2 * PRIME2;
                v2 = rotl64(v2, 31);
                v2 *= PRIME1;

                v3 += r3 * PRIME2;
                v3 = rotl64(v3, 31);
                v3 *= PRIME1;

                v4 += r4 * PRIME2;
                v4 = rotl64(v4, 31);
                v4 *= PRIME1;
            }
            hash = rotl64(v1, 1) + rotl64(v2, 7) + rotl64(v3, 12) + rotl64(v4, 18);
            hash = mergeRound(hash, v1);
            hash = mergeRound(hash, v2);
            hash = mergeRound(hash, v3);
            hash = mergeRound(hash, v4);
        }
        if (s.length() - k >= 4) {
            while (k < s.length() - 4) {
                uint64_t r1 = 0;
                for (int i = 0; i < 4; i++) {
                    d = static_cast<uint8_t>(s[k + i]);
                    r1 |= d << (4 * i);
                }
                k += 4;
                hash ^= r1 * PRIME1;
                hash = rotl64(hash, 23) * PRIME2 + PRIME3;
            }
        }
        uint64_t r1 = 0;
        for (int i = 0; i < s.length() - k; i++) {
            r1 |= static_cast<uint8_t>(s[i + k]) << (8 * i);
        }
        hash ^= r1 * PRIME1;
        hash = rotl64(hash, 23) * PRIME2 + PRIME3;

        hash ^= hash >> 33;
        hash *= PRIME2;
        hash ^= hash >> 29;
        hash *= PRIME3;
        hash ^= hash >> 32;

        return hash;
    }

    void resize() {

        int tnum = num * 2;
        count = 0;
        std::vector<Entry>** temp = new std::vector<Entry>*[tnum];

        for (int i = 0; i < tnum; ++i) {
            temp[i] = new std::vector<Entry>();
        }

        for (int i = 0; i < num; i++) {
            for (int j = 0; j < data[i]->size(); j++) {
                Entry dat = (*data[i])[j];

                int pos = hash(dat.name) & (tnum - 1);
                if (temp[pos]->size() == 0) {
                    count++;
                }
                temp[pos]->push_back(dat);

            }
        }
        for (int i = 0; i < num; ++i) {
            delete data[i];
        }
        delete[] data;
        data = temp;
        num = tnum;
    }

public:
    HashTableChanes() {

        num = 2;
        count = 0;

        data = new std::vector<Entry>*[num];

        for (int i = 0; i < num; ++i) {
            data[i] = new std::vector<Entry>();
        }
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> dist(100, 6000);

        int randA = dist(gen);
        int randB = dist(gen);
    }
    ~HashTableChanes() {
        for (int i = 0; i < num; ++i) {
            delete data[i];
        }
        delete[] data;
    }
    void add(const std::string& name, const Polynomials& pol) {

        Entry dat = Entry(name, pol);
        int pos = hash(name);
        pos = pos & (num - 1);
        if (data[pos]->size() == 0) {
            count++;
        }
        for (int i = 0; i < data[pos]->size(); i++) {
            if ((*data[pos])[i].name == name) {
                throw std::runtime_error("This name already exists in the array");
            }
        }
        data[pos]->push_back(dat);

        if (static_cast<float>(count) / static_cast<float>(num) >= 0.5) {
            std::cout << "Resizing from " << num << " to " << num * 2 << std::endl;
            resize();
        }
    }
    Polynomials find(std::string name) {
        int pos = hash(name);
        pos = pos & (num - 1);
        Entry temp;
        temp.name = name;
        bool has_name = false;
        Polynomials res;
        for (int i = 0; i < data[pos]->size(); i++) {
            if ((*data[pos])[i].name == name) {
                has_name = true;
                res = (*data[pos])[i].data;
            }
        }
        if (has_name) {
            return res;
        }
        else {
            return false;
        }
    }
    int remove(std::string name) {
        int pos = hash(name);
        pos = pos & (num - 1);
        Entry temp;
        temp.name = name;
        bool has_name = false;
        for (int i = 0; i < data[pos]->size(); i++) {
            if ((*data[pos])[i].name == name) {
                has_name = true;
                (*data[pos])[i] = (*data[pos])[data[pos]->size() - 1];
                data[pos]->pop_back();
            }
        }
        if (data[pos]->size() == 0) {
            count--;
        }
        if (has_name) {
            return 0;
        }
        else {
            throw "wsgspin";
        }
    }

};
