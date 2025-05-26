#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <optional>
#include <string>
#include "Polynomials.h"
#include "HashTableOpen.h"
#include "Table.h"
#include "RBtree.h"
#include "AVLtree.h"
#include "HashTableChain.h"
#include "SortedTable.h"

class Storage {
private:
	Table table;
	HashTableOpenAdress HashTable;
	RBtree tree;
	AVLtree avlTree;
	SArray SortTable;
	HashTableChanes HashChanes;
public:
	int stringToAscii(const std::string& str) {
		int sum = 0;
		for (char c : str) {
			sum += static_cast<int>(c);  // преобразуем символ в его ASCII-код
		}
		return sum;
	}

	void insert(std::string key, Polynomials p) {
		table.insert(key, p);
		HashTable.insert(key, p);
		tree.RB_insert(stringToAscii(key), p);
		avlTree.add(key, p);
		SortTable.add(key, p);
		HashChanes.add(key, p);

	}
	void erase(std::string key) {
		table.erase(key);
		HashTable.erase(key);
		tree.RB_erase(stringToAscii(key));
		avlTree.remove(key);
		SortTable.remove(key);
		HashChanes.remove(key);

	}
	bool find(std::string key) {
		return table.find(key);

	}
	Polynomials get(std::string key) {
		return table.GetPoly(key);
	}
	std::string printAVL()  {
		return avlTree.GetPoly();
	}

	std::string printRB()  {
		return tree.GetPoly();
	}
	std::string HashTableOpenPrint() {
		return HashTable.GetPoly();
	}

	std::string printTable()  {
			return table.GetString();;
	}

	std::string printSortedTable()  {
		return SortTable.GetPoly();
	}

	std:: string  print() {
		return table.GetString();
		//HashTable.print();
		//tree.RB_print();
	}
};

