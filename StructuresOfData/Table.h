#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <optional>
#include "Polynomials.h"

class Table {
private:
	std::vector <std::pair <std::string, Polynomials> >vect;

public:
	Table() {}
	bool find(std::string key) {
		for (int i = 0; i < vect.size(); i++) {
			if (vect[i].first == key) return true;

		}
		return false;
	}
	int size() {
		return vect.size();
	}
	void erase(std::string key) {
		for (int i = 0; i < vect.size(); i++) {
			if (vect[i].first == key) {
				std::pair<std::string, Polynomials> tmp{ vect[i].first, vect[i].second };
				vect[i] = vect[vect.size() - 1];
				vect[vect.size() - 1] = tmp;
				vect.pop_back();
			}
		}


	}
	void insert(std::string key, Polynomials val) {
		std::pair<std::string, Polynomials> p(key, val);
		if (!find(key)) {
			vect.push_back(p);
		}
		else throw "the key is already exists!";

	}
	Polynomials GetPoly(std::string key) {
		for (int i = 0; i < vect.size(); i++) {
			if (vect[i].first == key) return vect[i].second;
		}
		return false;
	}

	std::string GetString() {
		std::string result;
		for (int i = 0; i < vect.size(); i++) {
			result = result + vect[i].first + " = " + vect[i].second.to_string() + "\r\n";
		}
		return result;
	}

	void print() {
		for (int i = 0; i < vect.size(); i++) {
			std::cout << vect[i].first << " = ";
			vect[i].second.print();

		}

	}


};