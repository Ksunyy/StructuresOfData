#pragma once
#include <iostream>	  
#include "string"
#include "tlist.h"


struct monom {
	double coef;
	int degree;
	monom(double coeficent = 0.0, int degr = 0) : coef(coeficent), degree(degr) {}
	bool operator==(monom m) {
		return (m.coef == this->coef && m.degree == this->degree);
	}
	bool operator!=(monom m) {
		return !(m.coef == this->coef && m.degree == this->degree);
	}
	monom operator-() {
		monom res(-(this->coef), this->degree);
		return res;
	}
	monom operator*(monom& m) {
		if (this->degree + m.degree > 999) throw "degree out of range";
		else return monom(this->coef * m.coef, this->degree + m.degree);
	}
};


inline int pos(tlist<monom>& m, int low, int high) {
	int piv = m[high].degree;
	int i = low - 1;
	for (int j = low; j < high; j++) {
		if (m[j].degree < piv) {
			i++;
			monom tmp = m[i];
			m[i] = m[j];
			m[j] = tmp;
		}
	}
	monom tmp2 = m[i + 1];
	m[i + 1] = m[high];
	m[high] = tmp2;
	return i + 1;

}
inline void QuikSort(tlist<monom>& m, int low, int high) {
	if (low < high) {
		int pivInd = pos(m, low, high);
		QuikSort(m, low, pivInd - 1);
		QuikSort(m, pivInd + 1, high);

	}


}

class Polynomials {
private:
	tlist<monom> monoms;
public:
	Polynomials(double val) {
		if (val == 0) {
			// пустой или нулевой полином (по желанию)
		}
		else {
			monom m(val, 0);  // моном с коэффициентом val и степенью 0
			monoms.insert_front(m);
		}
	}
	Polynomials(std::string str) {
		if (!str.empty() && str[0] != '+' && str[0] != '-') {
			str = "+" + str;
		}

		machine(str);
	}
	Polynomials() {

	}

	Polynomials(const Polynomials& other) : monoms(other.monoms) {}

	Polynomials& operator=(const Polynomials& other) {
		if (this != &other) {
			monoms = other.monoms; // Вызывает operator= у tlist
		}
		return *this;
	}
	~Polynomials() = default;

	int lenPoly() {
		return monoms.sz();
	}


	void machine(std::string str) {
		int status = 0;
		monom m(0, 0);
		int sign = 1;
		std::string tmp = "";
		for (int i = 0; i < str.size(); i++) {
			try {

				switch (status) {
				case 0:
					if (str[i] == '+' || str[i] == '-') {
						sign = str[i] == '+' ? 1 : -1;
						status = 1;
					}
					else if ('0' <= str[i] && str[i] <= '9') {
						tmp += str[i];
						status = 1;
					}
					else if (str[i] == 'x') {
						m.coef = sign;
						status = 2;
					}
					else throw "unexpected symbol";
					break;
				case 1:
					if ('0' <= str[i] && str[i] <= '9') {
						tmp += str[i];
					}
					else if (str[i] == 'x') {
						status = 2;
						if (tmp.empty()) m.coef = sign;
						else m.coef = std::stod(tmp) * sign;

					}
					else throw "unexpected symbol";
					break;
				case 2:
					if (str[i] != '^') throw "unexpected symbol";
					else status = 3;
					break;
				case 3:
					if ('0' <= str[i] && str[i] <= '9') {
						m.degree = (str[i] - '0') * 100;
						status = 4;
					}
					else throw "unexpected symbol";
					break;
				case 4:
					if (str[i] == 'y') status = 5;
					else throw "unexpected symbol";
					break;
				case 5:
					if (str[i] != '^') throw "unexpected symbol";
					else status = 6;
					break;
				case 6:
					if ('0' <= str[i] && str[i] <= '9') {
						m.degree += (str[i] - '0') * 10;
						status = 7;
					}
					else throw "unexpected symbol";
					break;
				case 7:
					if (str[i] == 'z') status = 8;
					else throw "unexpected symbol";
					break;
				case 8:
					if (str[i] != '^') throw "unexpected symbol";
					else status = 9;
					break;
				case 9:
					if ('0' <= str[i] && str[i] <= '9') {
						m.degree += (str[i] - '0');
						if (m.coef != 0) monoms.insert_back(m);
						m.coef = 0;
						m.degree = 0;
						tmp = "";
						if (i != str.size() - 1) status = 0;
						if (i == str.size() - 1) break;


					}
					else throw "unexpected symbol";
					break;

				}
			}
			catch (const char* e) {
				std::cerr << "Error " << e << std::endl;
				break;
			}




		}

		QuikSort(monoms, 0, monoms.sz() - 1);
	}


	Polynomials operator+(Polynomials pol) {
		Polynomials res;
		auto p1 = monoms.begin();
		auto p2 = pol.monoms.begin();
		while (p1 != this->monoms.end() && p2 != pol.monoms.end()) {
			if (p1->degree < p2->degree) {
				res.monoms.insert_front(*p1);
				++p1;

			}
			else if (p1->degree > p2->degree) {
				res.monoms.insert_front(*p2);
				++p2;

			}
			else {
				if (p1->coef != -(p2->coef)) {
					res.monoms.insert_front(*p1);
					res.monoms.insert_front(*p2);
				}
				++p1;
				++p2;

			}
		}
		while (p1 != this->monoms.end()) {
			res.monoms.insert_front(*p1);
			++p1;
		}
		while (p2 != pol.monoms.end()) {
			res.monoms.insert_front(*p2);
			++p2;
		}
		auto ptr = res.monoms.begin();
		while (ptr != res.monoms.end() && ptr.getCurrent()->next) {
			auto tmpPtr = ptr;
			++tmpPtr;
			if (ptr->degree == tmpPtr->degree) {
				ptr->coef += tmpPtr->coef;
				monoms.erase(ptr.getCurrent());
			}
			else ++ptr;



		}
		res.monoms.reverse();

		return res;

	}
	Polynomials operator-(Polynomials pol) {
		Polynomials res;
		auto p1 = monoms.begin();
		auto p2 = pol.monoms.begin();
		while (p1 != this->monoms.end() && p2 != pol.monoms.end()) {
			if (p1->degree < p2->degree) {
				res.monoms.insert_front(*p1);
				++p1;
			}
			else if (p1->degree > p2->degree) {
				res.monoms.insert_front(*p2);
				++p2;
			}
			else {
				if (p1->coef != p2->coef) {
					res.monoms.insert_front(*p1);
					res.monoms.insert_front(*p2);
				}

				++p1;
				++p2;

			}
		}
		while (p1 != this->monoms.end()) {
			res.monoms.insert_front(*p1);
			++p1;
		}
		while (p2 != pol.monoms.end()) {
			res.monoms.insert_front(*p2);
			++p2;
		}
		auto ptr = res.monoms.begin();
		while (ptr != res.monoms.end() && ptr.getCurrent()->next) {
			auto tmpPtr = ptr;
			++tmpPtr;
			if (ptr->degree == tmpPtr->degree) {
				tmpPtr->coef -= ptr->coef;
				monoms.erase(ptr.getCurrent());
			}
			else ++ptr;



		}
		res.monoms.reverse();
		return res;

	}
	Polynomials operator-() {
		auto ptr = this->monoms.begin();
		Polynomials res;
		while (ptr != this->monoms.end()) {
			res.monoms.insert_front(-(*ptr));
			++ptr;
		}
		res.monoms.reverse();
		return res;
	}
	Polynomials operator*(double val) {
		auto ptr = this->monoms.begin();
		Polynomials res;
		if (val != 0) {
			while (ptr != this->monoms.end()) {
				monom m(ptr->coef * val, ptr->degree);
				res.monoms.insert_front(m);
				++ptr;
			}
		}
		res.monoms.reverse();
		return res;
	}

	bool operator==(Polynomials& p) {
		if (this->monoms.sz() != p.monoms.sz()) return 0;
		auto p1 = this->monoms.begin();
		auto p2 = p.monoms.begin();
		bool chek = 1;
		while (p1 != this->monoms.end() && p2 != p.monoms.end()) {
			if (*p1 != *p2) return 0;
			else chek = 1;
			++p1;
			++p2;

		}
		return chek;
	}

	Polynomials operator*(monom& m) {
		Polynomials res;
		auto ptr = this->monoms.begin();
		while (ptr != this->monoms.end()) {
			if ((m.degree + ptr->degree) > 999) throw "degree out of range";
			res.monoms.insert_front(*ptr * m);
			++ptr;
		}
		res.monoms.reverse();
		return res;

	}

	Polynomials operator*(Polynomials& p) {
		Polynomials finRes;
		auto p1 = this->monoms.begin();
		while (p1 != this->monoms.end()) {
			Polynomials res;
			auto p2 = p.monoms.begin();
			while (p2 != p.monoms.end()) {
				res.monoms.insert_front(*p1 * (*p2));

				++p2;
			}
			res.monoms.reverse();

			finRes = res + finRes;


			++p1;
		}


		return finRes;

	}

	static Polynomials from_constant(double val) {
		Polynomials result;
		if (val != 0.0) {
			monom m(val, 0); // Создаём моном с заданным коэффициентом и нулевой степенью
			result.monoms.insert_back(m);
		}
		return result;
	}

	std::string to_string() {
		std::string result;
		auto ptr = this->monoms.begin();
		while (ptr != this->monoms.end()) {
			if (ptr->coef > 0 && ptr != this->monoms.begin()) {
				result += "+";
			}
			result += std::to_string(ptr->coef);
			result += "x^" + std::to_string(ptr->degree / 100);
			result += "y^" + std::to_string((ptr->degree % 100) / 10);
			result += "z^" + std::to_string(ptr->degree % 10);
			ptr++;
		}
		return result;
	}


	void print() {
		char sign = ' ';
		auto ptr = this->monoms.begin();
		while (ptr != this->monoms.end()) {
			if (ptr->coef > 0 && ptr != this->monoms.begin()) {
				std::cout << "+" << ptr->coef << "x^" << ptr->degree / 100 << "y^" << (ptr->degree % 100) / 10 << "z^" << ptr->degree % 10;
			}
			else std::cout << ptr->coef << "x^" << (ptr->degree) / 100 << "y^" << ((ptr->degree) % 100) / 10 << "z^" << ptr->degree % 10;
			ptr++;

		}
		std::cout << '\n';
	}


};





