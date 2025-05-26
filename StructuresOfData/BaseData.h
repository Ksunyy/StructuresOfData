#include <iostream>
#include <queue>
#include <random>
#include "Polynomials.h"
#include "tlist.h"

#pragma once

class BasePolynomialsStorage {
public:
    virtual ~BasePolynomialsStorage() = default;
    virtual int add() { return 0; }; 
    virtual Polynomials find() { return Polynomials(); }; 
    virtual int remove() { return 0; };
};
