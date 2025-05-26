#include <iostream>
#include <queue>
#include <random>
#include "Polynomials.h"
#include "tlist.h"
#include "Basedata.h"

#pragma once

class AVLtree :public BasePolynomialsStorage {
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
    struct Node {
        Node* left;
        Node* right;
        Entry data;
        int height;
        Node() {
            left = nullptr;
            right = nullptr;
            data = Entry();
            height = 0;
        }
        ~Node() {
            left = nullptr;
            right = nullptr;
        }
        int countHeigth() {
            int l, r;
            if (this->left == nullptr) {
                l = 0;
            }
            else {
                l = this->left->height;
            }
            if (this->right == nullptr) {
                r = 0;
            }
            else {
                r = this->right->height;
            }
            int maximum = (r > l) ? r : l;
            return 1 + maximum;
        }
        int countBalance() {
            int l, r;
            if (this->left == nullptr) {
                l = 0;
            }
            else {
                l = this->left->height;
            }
            if (this->right == nullptr) {
                r = 0;
            }
            else {
                r = this->right->height;
            }
            return l - r;
        }
        Node* leftTurn() {
            //std::cout << "Left turt in "<<this->data.name<<"\n";
            Node* newRoot = this->right;
            Node* leftSubtree = newRoot->left;

            newRoot->left = this;
            this->right = leftSubtree;

            this->height = this->countHeigth();
            newRoot->height = newRoot->countHeigth();

            return newRoot;
        }

        Node* rightTurn() {
            //std::cout << "Right turt in " << this->data.name << "\n";
            Node* newRoot = this->left;
            Node* rightSubtree = newRoot->right;

            newRoot->right = this;
            this->left = rightSubtree;

            this->height = this->countHeigth();
            newRoot->height = newRoot->countHeigth();

            return newRoot;
        }
        Node* findMin() {
            if (this->left == nullptr) {
                return this;
            }
            Node* res = this->left->findMin();
            return res;
        }
    };
    Node* root;

    Node* recInsert(Node* node, const Entry& d) {
        //std::cout << "Insert is in " << node->data.name << "\n";
        if (d.name < node->data.name) {
            if (node->left == nullptr) {
                node->left = new Node();
                node->left->data = d;
                node->left->height = 1;
                node->height = node->countHeigth();
                return node;
            }
            Node* next = recInsert(node->left, d);
            node->left = next;
            node->height = node->countHeigth();
            int balance = node->countBalance();
            if (balance == 2) {
                if (node->left->countBalance() == 1) {
                    Node* ret = node->rightTurn();
                    return ret;
                }
                if (node->left->countBalance() == -1) {
                    node->left = node->left->leftTurn();
                    Node* ret = node->rightTurn();
                    return ret;
                }
            }
            return node;
        }
        if (d.name > node->data.name) {
            if (node->right == nullptr) {
                node->right = new Node();
                node->right->data = d;
                node->right->height = 1;
                node->height = node->countHeigth();
                return node;
            }
            Node* next = recInsert(node->right, d);
            node->right = next;
            node->height = node->countHeigth();
            int balance = node->countBalance();
            if (balance == -2) {
                if (node->right->countBalance() == -1) {
                    Node* ret = node->leftTurn();
                    return ret;
                }
                if (node->right->countBalance() == 1) {
                    node->right = node->right->rightTurn();
                    Node* ret = node->leftTurn();
                    return ret;
                }
            }
            return node;
        }
        if (d.name == node->data.name) {
            throw std::invalid_argument("Same name");
        }
    }
    int recDestructor(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        else {
            recDestructor(node->left);
            recDestructor(node->right);
            delete node;
        }
    }
    Node* recDel(Node* node, std::string name) {
        if (node == nullptr) {
            throw std::invalid_argument("No such name");
        }
        if (name < node->data.name) {
            node->left = recDel(node->left, name);
            node->height = node->countHeigth();
            int balance = node->countBalance();
            if (balance == 2) {
                if (node->left->countBalance() == 1) {
                    Node* ret = node->rightTurn();
                    return ret;
                }
                if (node->left->countBalance() == -1) {
                    node->left = node->left->leftTurn();
                    Node* ret = node->rightTurn();
                    return ret;
                }
            }
            if (balance == -2) {
                if (node->right->countBalance() == -1) {
                    Node* ret = node->leftTurn();
                    return ret;
                }
                if (node->right->countBalance() == 1) {
                    node->right = node->right->rightTurn();
                    Node* ret = node->leftTurn();
                    return ret;
                }
            }
            return node;
        }
        if (name > node->data.name) {
            node->right = recDel(node->right, name);
            node->height = node->countHeigth();
            int balance = node->countBalance();
            if (balance == 2) {
                if (node->left->countBalance() == 1) {
                    Node* ret = node->rightTurn();
                    return ret;
                }
                if (node->left->countBalance() == -1) {
                    node->left = node->left->leftTurn();
                    Node* ret = node->rightTurn();
                    return ret;
                }
            }
            if (balance == -2) {
                if (node->right->countBalance() == -1) {
                    Node* ret = node->leftTurn();
                    return ret;
                }
                if (node->right->countBalance() == 1) {
                    node->right = node->right->rightTurn();
                    Node* ret = node->leftTurn();
                    return ret;
                }
            }
            return node;
        }
        if (name == node->data.name) {
            int k = 0;
            if (node->left != nullptr) {
                k += 1;
            }
            if (node->right != nullptr) {
                k += 1;
            }
            if (k == 0) {
                delete node;
                return nullptr;
            }
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            if (k == 2) {
                Node* minInRight = node->right->findMin();
                node->data.name = minInRight->data.name;
                node->data.data = minInRight->data.data;
                node->right = recDel(node->right, minInRight->data.name);

                node->height = node->countHeigth();
                int balance = node->countBalance();
                if (balance == 2) {
                    if (node->left->countBalance() == 1) {
                        Node* ret = node->rightTurn();
                        return ret;
                    }
                    if (node->left->countBalance() == -1) {
                        node->left = node->left->leftTurn();
                        Node* ret = node->rightTurn();
                        return ret;
                    }
                }
                if (balance == -2) {
                    if (node->right->countBalance() == -1) {
                        Node* ret = node->leftTurn();
                        return ret;
                    }
                    if (node->right->countBalance() == 1) {
                        node->right = node->right->rightTurn();
                        Node* ret = node->leftTurn();
                        return ret;
                    }
                }
                return node;
            }
        }
    }

public:
    AVLtree() {
        root = nullptr;
    }
    ~AVLtree() {
        recDestructor(root);
    }
    int add(const std::string& name, const Polynomials& pol) {
        Entry d = Entry(name, pol);
        if (root == nullptr) {
            Node* temp = new Node();
            temp->data = d;
            root = temp;
            temp = nullptr;
            return 0;
        }
        root = recInsert(root, Entry(name, pol));
        return 0;
    }
    Polynomials find(std::string name) {
        Node* cur = root;
        while (cur != nullptr) {
            if (name > cur->data.name) {
                cur = cur->right;
            }
            if (name < cur->data.name) {
                cur = cur->left;
            }
            if (name == cur->data.name) {
                return cur->data.data;
            }
        }
        throw std::invalid_argument("No such name");
    }

    int remove(std::string name) {
        root = recDel(root, name);
        return 0;
    }
    std::string GetPoly() {
        if (!root) return ""; // Проверка на пустое дерево

        std::queue<Node*> q;
        q.push(root);
        std::string result;

        while (!q.empty()) {
            int levelSize = q.size();

            for (int i = 0; i < levelSize; ++i) {
                Node* current = q.front();
                q.pop();

                result += current->data.name + " ";

                if (current->left) q.push(current->left);
                if (current->right) q.push(current->right);
            }
            result += "\n"; // Переход на новую строку после каждого уровня
        }
        return result;
    }

    void printTreeByLevels() {
        if (!root) return; // Ïðîâåðêà íà ïóñòîå äåðåâî

        std::queue<Node*> q;       // Î÷åðåäü äëÿ îáõîäà äåðåâà
        q.push(root);         // Äîáàâëÿåì êîðåíü â î÷åðåäü

        while (!q.empty()) {
            int levelSize = q.size(); // Êîëè÷åñòâî óçëîâ íà òåêóùåì óðîâíå

            for (int i = 0; i < levelSize; ++i) {
                Node* current = q.front(); // Èçâëåêàåì óçåë èç î÷åðåäè
                q.pop();

                std::cout << current->data.name << " "; // Âûâîäèì çíà÷åíèå óçëà

                // Äîáàâëÿåì äî÷åðíèå óçëû â î÷åðåäü
                if (current->left) q.push(current->left);
                if (current->right) q.push(current->right);
            }
            std::cout << std::endl; // Ïåðåõîä íà íîâóþ ñòðîêó ïîñëå êàæäîãî óðîâíÿ
        }
    }
};
#pragma once
