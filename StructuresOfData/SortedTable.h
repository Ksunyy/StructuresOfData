#include <iostream>
#include <queue>
#include <random>
#include "Polynomials.h"
#include "tlist.h"
#include "Basedata.h"

class SArray : public BasePolynomialsStorage {
private:
    struct Entry {
        std::string name;
        Polynomials data;
        Entry() {};
        Entry(const std::string& n, const Polynomials& d) {
            name = n;
            data = d;
        }
    };
    std::vector<Entry> data;
    bool hasName(std::string n) {
        for (Entry c : data) {
            if (c.name == n) {
                return 1;
            }
        }
        return 0;
    }
    bool isSorted() {
        for (int i = 0; i < data.size() - 1; i++) {
            if (data[i].name > data[i + 1].name) {
                return 0;
            }
        }
        return 1;
    }

    int find_int(std::string n) {
        int left = 0, right = data.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (data[mid].name == n) {
                return mid;
            }
            else if (data[mid].name < n) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        return -1;
    }
public:

    int add(const std::string& n, const Polynomials& d) {
        //O(n)
        if (hasName(n)) {
            throw "this name already in array";
        }
        Entry dat = Entry(n, d);
        //O(c)
        data.push_back(dat);
        //O(n)
        if (isSorted()) {
            std::cout << n << "\n";
            return 0;
        }
        int k = data.size() - 1;
        //O(n) (íàõîäèì i îäèí ðàç è îäèí ðàç âûïîëíÿåì while)
        for (int i = 0; i < k; i++) {
            if (data[i].name < n && n < data[i + 1].name) {
                Entry temp = data[i + 1];
                data[i + 1] = data[k];
                data[k] = temp;
                i++;
                while (i < k) {
                    temp = data[i + 1];
                    data[i + 1] = data[k];
                    data[k] = temp;
                    i++;
                }
                break;
            }
            if (data[i].name > n) {
                Entry temp = data[i];
                data[i] = data[k];
                data[k] = temp;
                while (i < k) {
                    temp = data[i + 1];
                    data[i + 1] = data[k];
                    data[k] = temp;
                    i++;
                }
                break;
            }
        }
        return 0;
    }
    Polynomials find(std::string n) {
        int left = 0, right = data.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (data[mid].name == n) {
                return data[mid].data;
            }
            else if (data[mid].name < n) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        return NULL;
    }
    int remove(std::string n) {
        int ind = find_int(n);
        if (ind != -1) {
            int k = ind;
            while (k < data.size() - 1) {
                data[k] = data[k + 1];
            }
            data.pop_back();
            return 0;
        }
        return -1;
    }

    std::string GetPoly() {
        std::string result;
        for (int i = 0; i < data.size(); i++) {
            result = result + data[i].name + " = " + data[i].data.to_string() + "\r\n";
        }
        return result;
    }

    void print() {
        for (Entry c : data) {
            std::cout << c.name << ": ";
            c.data.print();
        }
    }
};