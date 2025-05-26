#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include "Polynomials.h"
#include <sstream> 

class RBtree {
	std::vector<int> CounterInsertRotates;
	std::vector<int> CounterEraseRotates;

	enum Color { red = 0, black = 1 };
	struct Node {
		int key;
		Polynomials poly;
		Node* Rchild, * Lchild, * parent;
		Color col{ black };
		Node(Polynomials data, Node* R = nullptr, Node* L = nullptr, Node* P = nullptr, Color color = black, int k = -1) :poly(data), Rchild(R), Lchild(L), parent(P), col(color), key(k) {}
		Node& operator=(const Node& other) {
			if (this != &other) {
				this->poly = other.poly;
				this->col = other.col;
				this->key = other.key;

			}
			return *this;

		}
		bool operator==(const Node other) {
			return this == &other;
		}

	};
	Node* root;
	int size = 0;
	int  CountInsertRotate = 0;
	int  CountEraseRotate = 0;


	Node* copyTree(Node* node, Node* par) {
		Node* cur = new Node(node->poly, nullptr, nullptr, par, node->col, node->key);
		this->size++;
		if (node->Lchild) cur->Lchild = copyTree(node->Lchild, cur);
		if (node->Rchild) cur->Rchild = copyTree(node->Rchild, cur);
		return cur;

	}

	Node* Tree_min(Node* cur) {
		while (cur->Lchild) {
			cur = cur->Lchild;
		}

		return cur;
	}
	Node* Tree_max(Node* cur) {
		while (cur->Rchild) {
			cur = cur->Rchild;
		}
		return cur;
	}
	Node* Successor(Node* node) {
		if (node->Rchild != nullptr) {
			return Tree_min(node->Rchild);
		}
		Node* y = node->parent;
		while (y != nullptr && node == y->Rchild) {
			node = y;
			y = y->parent;
		}
		return y;

	}
	Node* Predecessor(Node* node) {
		if (node->parent != nullptr) {
			return node->parent;
		}
		else return root;

	}

	void Lrotate(Node* node) {
		if (!node || !node->Rchild) return;
		Node* y = node->Rchild;
		node->Rchild = y->Lchild;

		if (y->Lchild != nullptr) {
			y->Lchild->parent = node;
		}
		y->parent = node->parent;
		if (node->parent == nullptr) root = y;
		else if (node->parent->Lchild == node) node->parent->Lchild = y;
		else node->parent->Rchild = y;
		y->Lchild = node;
		node->parent = y;

	}
	void Rrotate(Node* node) {
		if (!node || !node->Lchild) return;
		Node* y = node->Lchild;
		node->Lchild = y->Rchild;
		if (y->Rchild != nullptr) {
			y->Rchild->parent = node;
		}
		y->parent = node->parent;
		if (node->parent == nullptr) root = y;
		else if (node->parent->Lchild == node) node->parent->Lchild = y;
		else node->parent->Rchild = y;
		y->Rchild = node;
		node->parent = y;

	}



	Node* find(Node* node, int id) {
		if (node == nullptr) return nullptr;
		else {
			if (id < node->key) return find(node->Lchild, id);
			if (id > node->key) return find(node->Rchild, id);
			if (id == node->key) return node;

		}


	}
	void insert_fixup(Node* node) {

		while (node != root && node->parent != nullptr && node->parent->col == red) {
			if (node->parent == node->parent->parent->Lchild)
			{
				// 3 случая, если вставляем в левое поддерево 
				Node* y = node->parent->parent->Rchild;
				if (y != nullptr && y->col == red) {
					node->parent->col = black;
					y->col = black;
					node->parent->parent->col = red;
					node = node->parent->parent;
				}
				else if (node == node->parent->Rchild) {
					node = node->parent;
					Lrotate(node);
					CountInsertRotate++;


				}
				else {
					node->parent->col = black;
					node->parent->parent->col = red;
					Rrotate(node->parent->parent);
					CountInsertRotate++;


				}
			}

			else {
				// 3 случая, если вставляем в правое поддерево 
				Node* y = node->parent->parent->Lchild;
				if (y != nullptr && y->col == red) {
					node->parent->col = black;
					y->col = black;
					node->parent->parent->col = red;
					node = node->parent->parent;
				}
				else if (node == node->parent->Lchild) {
					node = node->parent;
					Rrotate(node);
					CountInsertRotate++;



				}
				else {
					node->parent->col = black;
					node->parent->parent->col = red;
					Lrotate(node->parent->parent);
					CountInsertRotate++;


				}

			}
		}
		root->col = black;


	}
	void erase_fixup(Node* node) {
		while (node != nullptr && node != root && node->col == black) {
			if (node == node->parent->Lchild) {
				Node* w = node->parent->Rchild;
				if (w->col == red) {
					w->col = black;
					w->parent->col = red;
					Lrotate(node->parent);
					CountEraseRotate++;
					w = node->parent->Rchild;
				}
				if (w->Lchild->col == black && w->Rchild->col == black) {
					w->col = red;
					node = node->parent;
				}
				else if (w->Rchild->col == black) {
					w->Lchild->col = black;
					w->col = red;
					Rrotate(w);
					CountEraseRotate++;
					w = node->parent->Rchild;
				}
				else {
					w->col = node->parent->col;
					node->parent->col = black;
					w->Rchild->col = black;
					Lrotate(node->parent);
					CountEraseRotate++;
					node = root;
				}

			}
			else {
				Node* w = node->parent->Lchild;
				if (w->col == red) {
					w->col = black;
					w->parent->col = red;
					Rrotate(node->parent);
					CountEraseRotate++;
					w = node->parent->Lchild;
				}
				if (w->Lchild->col == black && w->Rchild->col == black) {
					w->col = red;
					node = node->parent;
				}
				else if (w->Lchild->col == black) {
					w->Rchild->col = black;
					w->col = red;
					Lrotate(w);
					CountEraseRotate++;
					w = node->parent->Lchild;
				}
				else {
					w->col = node->parent->col;
					node->parent->col = black;
					w->Lchild->col = black;
					Rrotate(node->parent);
					CountEraseRotate++;
					node = root;
				}

			}

		}


	}

	void print(Node* rt) {
		Node* cur = rt;
		if (cur) {
			print(cur->Lchild);
			std::cout << cur->key << '\n';
			cur->poly.print();
			print(cur->Rchild);
		}

	}
	void printToStream(Node* rt, std::ostringstream& oss) const {
		if (!rt) return;
		printToStream(rt->Lchild, oss);
		oss << "Ключ: " << rt->key << "\n";
		oss << "Полином: " << rt->poly.to_string() << "\n\n"; // или rt->poly.print(oss), если реализуешь
		printToStream(rt->Rchild, oss);
	}

public:
	RBtree() : root(nullptr) {}
	~RBtree() {
		if (!root) return;

		std::stack<Node*> st;
		Node* curr = root;
		Node* lastVisited = nullptr;

		while (!st.empty() || curr != nullptr) {
			if (curr != nullptr) {
				st.push(curr);
				curr = curr->Lchild;
			}
			else {
				Node* peekNode = st.top();
				if (peekNode->Rchild != nullptr && lastVisited != peekNode->Rchild) {
					// Спускаемся в правое поддерево
					curr = peekNode->Rchild;
				}
				else {
					// Оба поддерева посещены — можно удалить
					st.pop();
					lastVisited = peekNode;
					delete peekNode;
				}
			}
		}
		root = nullptr;
	}

	RBtree(const RBtree& other) : root(nullptr) {
		if (other.root != nullptr) {
			root = copyTree(other.root, nullptr);
		}
	}
	RBtree& operator=(const RBtree& other) {
		if (&other != this) {
			deleteTree(this->root);
			if (other.root != nullptr) this->root = copyTree(other.root, nullptr);
			else root = nullptr;

		}
		return *this;
	}

	int getSize() {
		return size;
	}


	/*	Node* getParent(Node* node) {
			return node->parent;
		}
		Node* getBrother(Node* node) {
			if (node->parent->Lchild != node) return node->parent->Lchild;
			else node->parent->Rchild;

		}
		Node* getGran(Node* node) {
			return node->parent->parent;
		}
		Node* getLNephew(Node* node) {
			return this->getBrother(node)->Lchild;

		}
		Node* getRNephew(Node* node) {
			return this->getBrother(node)->Lchild;

		}

	*/
	Node* getRoot() {
		return root;
	}
	std::vector<int> getInsertRotates() {
		return CounterInsertRotates;
	}
	std::vector<int> getEraseRotates() {
		return CounterEraseRotates;
	}



	void deleteTree(Node* node) {
		if (node == nullptr) return;
		deleteTree(node->Lchild);
		deleteTree(node->Rchild);
		delete(node);
	}

	Node* RB_insert(int key, Polynomials val) {
		CountInsertRotate = 0;
		size++;
		Node* node = new Node(val, nullptr, nullptr, nullptr, black, key);
		Node* y = nullptr;
		Node* x = root;
		while (x != nullptr) {
			y = x;
			if (node->key < x->key) x = x->Lchild;
			else x = x->Rchild;
		}
		node->parent = y;
		if (y == nullptr) root = node;
		else {
			if (node->key < y->key) y->Lchild = node;
			else y->Rchild = node;
		}
		node->Lchild = nullptr;
		node->Rchild = nullptr;
		node->col = red;
		insert_fixup(node);
		CounterInsertRotates.push_back(CountInsertRotate);
		return node;

	}
	Node* RB_erase(int key) {
		CountEraseRotate = 0;
		if (!RB_find(key)) return nullptr;
		Node* cur = this->RB_find(key);
		if (cur == nullptr) return nullptr;
		Node* y = nullptr;
		Node* x = nullptr;
		if (cur->Lchild == nullptr || cur->Rchild == nullptr) y = cur;
		else  y = this->Successor(cur);
		if (y->Lchild != nullptr)  x = y->Lchild;
		else x = y->Rchild;
		if (x != nullptr) x->parent = y->parent;
		if (y->parent == nullptr) this->root = x;
		else if (y == y->parent->Lchild) y->parent->Lchild = x;
		else  y->parent->Rchild = x;
		if (!(y == cur)) cur->key = y->key;
		if (y->col == black) erase_fixup(x);
		CounterEraseRotates.push_back(CountEraseRotate);
		return y;
	}
	Node* RB_find(int key) {
		return find(this->root, key);
	}


	void RB_print() {
		if (root) print(this->root);
		return;

	}

	std::string GetPoly() const {
		std::ostringstream oss;
		printToStream(root, oss);
		return oss.str();
	}


};