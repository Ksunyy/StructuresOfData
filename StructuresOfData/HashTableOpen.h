#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <optional>
#include "Polynomials.h"


class HashTableOpenAdress {
private:
	struct Node {
		std::string key;
		Polynomials p;
		int flag;	// 0 - не было значения и пусто, 1 - занято, -1 - было удалено
		Node() {
			key = "";
			Polynomials poly;
			p = poly;
			flag = 0;
		}
		Node(std::string key, Polynomials p, int flag = 0) : key(key), p(p), flag(flag) {}
	};
	std::vector< Node > data;
	int size = data.size();	// это то что m	(размер таблицы)
	int items = 0;	// это то что n	(количество уже вставленных элементов)

	// Hash Functions

	unsigned int MurMurHash(const void* key, uint32_t len) {
		const unsigned int m = 0x5bd1e995;
		const unsigned int seed = 0;
		const int r = 24;

		unsigned int h = seed ^ len;   // len - длина строки в байтах

		const unsigned char* data = (const unsigned char*)key;
		unsigned int k = 0;
		while (len >= 4)
		{
			k = data[0];
			k |= data[1] << 8;
			k |= data[2] << 16;
			k |= data[3] << 24;

			k *= m;
			k ^= k >> r;
			k *= m;

			h *= m;
			h ^= k;

			data += 4;
			len -= 4;
		}

		switch (len)
		{
		case 3:
			h ^= data[2] << 16;
		case 2:
			h ^= data[1] << 8;
		case 1:
			h ^= data[0];
			h *= m;
		};

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;


	}
	uint32_t Hash1(const std::string& key) {
		return MurMurHash(key.data(), key.size()) % size;
	}
	uint32_t Hash2(const std::string& key) {
		uint32_t hash = MurMurHash(key.data(), key.size());
		if (size <= 1) return 1;  // защита от деления на 0
		return (hash % (size - 1)) + 1;
	}

	int FinalHash(std::string key, int i) {
		return (Hash1(key) + i * Hash2(key)) % size;
	}
	void Resize() {
		HashTableOpenAdress NewTab(items * 2);
		for (int i = 0; i < size; i++) {
			int ind;
			if (this->data[i].flag == 1) {
				NewTab.insert(this->data[i].key, this->data[i].p);

			}

		}

		data = NewTab.data;
		size = NewTab.size;
		items = NewTab.items;


	}


public:
	HashTableOpenAdress() {
		size = 1;
		data.resize(size);

	};
	HashTableOpenAdress(int m) {
		if (m <= 0) throw "size incorrect";
		else {
			size = m;
			data.resize(size);
		}


	};

	HashTableOpenAdress(const HashTableOpenAdress& other) : data(other.data), size(other.size), items(other.items) {}
	~HashTableOpenAdress() = default;
	HashTableOpenAdress& operator=(const HashTableOpenAdress& other) {
		if (&other != this) {
			data = other.data;
			size = other.size;
			items = other.items;
		}
		return *this;
	}

	double GetFilling() {
		if (size != 0) return static_cast<double>(items) / size;
		else throw "division by zero";
	}

	int GetCountItems() {
		return items;
	}

	double GetSize() { return size; }

	void insert(std::string key, Polynomials p) {
		if (find(key)) throw "key already insert";
		else {
			if (GetFilling() > 0.7)Resize();
			for (int i = 0; i < size; i++) {
				int ind = FinalHash(key, i);
				if (ind > size) throw "calculate with hash is wrong";
				if (data[ind].flag != 1) {
					Node n(key, p, 1);
					data[ind] = n;
					items++;
					return;
				}
			}


		}

	}
	bool find(std::string key) {
		int i = 0;
		while (i < size) {
			int ind = FinalHash(key, i);
			if (data[ind].flag == 1 && data[ind].key == key) return true;
			if (data[ind].flag == 0) return false;
			i++;

		}

		return false;
	}


	void erase(std::string key) {
		if (!find(key)) return;
		else {
			for (int i = 0; i < size; i++) {
				int ind = FinalHash(key, i);
				if (data[ind].flag == 1 && data[ind].key == key) {
					Polynomials p;
					data[ind] = Node("", p, -1);
					items--;
					return;
				}
			}

		}
	}

	std::string GetPoly() {
		std::string result;
		for (int i = 0; i < size; i++) {
			result = result + data[i].key + " = " + data[i].p.to_string() + "\r\n";
		}
		return result;
	}

	void print() {
		for (int i = 0; i < size; i++) {
			std::cout << data[i].key << '\n';
			data[i].p.print();
			std::cout << "---------------- - " << '\n';
		}
	}

};