#pragma once
#include <iostream>



template <class T>


class tlist {
	struct Node {
		T data;
		Node* next;
		Node(T val = T(), Node* n = nullptr) :data(val), next(n) {}
		Node& operator = (const Node other) {
			if (this != &other) {
				this->data = other.data;
				delete next;
				if (other.next) {
					this->next = new Node(*other.next);
				}
				else next = nullptr;
			}
			return *this;
		}
	};

	Node* first;
public:
	tlist() : first(nullptr) {}
	tlist(int n) {
		if (n < 0) throw "can create list with negative length";
		T val{};
		first = new Node(val, nullptr);
		Node* current = first;
		for (int i = 1; i < n; ++i) {
			Node* tmp = new Node(val, nullptr);
			current->next = tmp;
			current = current->next;
		}
	}
	tlist(int n, T value) {
		if (n < 0) throw "can create list with negative length";
		T val = value;
		first = new Node(val, nullptr);
		Node* current = first;
		for (int i = 1; i < n; ++i) {
			Node* tmp = new Node(val, nullptr);
			current->next = tmp;
			current = current->next;
		}
	}

	~tlist() {
		while (first) {
			Node* second = first->next;
			delete first;
			first = second;
		}
	}
	size_t sz() {
		size_t size = 0;
		Node* current = first;
		while (current) {
			size++;
			current = current->next;
		}
		return size;
	};

	tlist(const tlist& other) {
		//if (!other.first) throw "no values in list";
		if (other.first) {
			this->first = new Node(other.first->data, nullptr);
			Node* current = first;
			Node* current_copy = other.first->next;
			while (current_copy) {
				current->next = new Node(current_copy->data, nullptr);
				current = current->next;
				current_copy = current_copy->next;
			}
		}
		else first = nullptr;

	};
	T get_data(Node* node) const {
		return node->data;

	}

	Node* get_first() {
		return first;
	}
	Node* get_next(Node* node) {
		return node->next;

	}

	T& operator [](int index) {
		if (index < 0 || index >= this->sz()) throw "wrong index";
		Node* current = first;
		for (int i = 0; i < index; ++i) {
			current = current->next;
		}
		return current->data;

	}


	tlist& operator =(const tlist& other) {
		if (&other == this) return (*this);
		while (first) {
			Node* tmp = first->next;
			delete first;
			first = tmp;
		}
		if (other.first) {
			first = new Node(other.first->data, nullptr);
			Node* current = first;
			Node* cur_copy = other.first->next;
			while (cur_copy) {
				current->next = new Node(cur_copy->data, nullptr);
				current = current->next;
				cur_copy = cur_copy->next;
			}

		}
		else first = nullptr;

		return *this;

	};

	void print() {
		Node* current = first;
		while (current) {
			std::cout << current->data << " ";
			current = current->next;
		}
	};

	Node* insert_front(T val) {
		Node* first_first = new Node();
		first_first->next = first;
		first_first->data = val;
		first = first_first;
		return first;

	};
	Node* insert_back(T val) {
		Node* newNode = new Node(val, nullptr);
		if (!first) first = newNode;
		else {
			Node* current = first;
			while (current->next) {
				current = current->next;
			}
			current->next = newNode;
		}
		return newNode;
	}

	Node* insert(T val, Node* prev) {
		if (!prev) throw "wrong prev";
		Node* tmp = new Node(val, prev->next);
		prev->next = tmp;
		return tmp;

	};


	Node* reverseList(Node* first) {
		Node* prev = nullptr;
		Node* cur = first;
		while (cur != nullptr) {
			Node* nextNode = cur->next;
			cur->next = prev;
			prev = cur;
			cur = nextNode;
		}
		return prev;

	}

	void reverse() {
		first = reverseList(first);
	}

	Node* erase_front() {
		if (!first) throw "list is empty";
		Node* tmp = first;
		first = tmp->next;
		delete tmp;
		return first;
	};
	Node* erase(Node* prev) {
		Node* tmp = prev->next;
		prev->next = tmp->next;
		delete tmp;
		return prev->next;

	}

	Node* find(T val) {
		Node* current = first;
		while (current) {
			if (current->data == val) return current;
			current = current->next;
		}
		return nullptr;

	};

	class Iterator {
	private:
		Node* cur;
	public:
		Iterator(Node* node = nullptr) : cur(node) {}
		Iterator& operator++() {
			if (cur) {
				cur = cur->next;
			}
			return *this;
		}
		Iterator operator++(int) {
			Iterator tmp = *this;
			++(*this);
			return tmp;

		}
		T& operator*() const {
			return cur->data;

		}
		T* operator->() const {
			return &(cur->data);
		}
		bool operator ==(const Iterator& another) const {
			return this->cur == another.cur;

		}
		bool operator !=(const Iterator& another) const {
			return !(*this == another);
		}
		Node* getCurrent() {
			return this->cur;

		}
	};
	Iterator begin() {
		return Iterator(first);
	}
	Iterator end() {
		return Iterator(nullptr);
	}



};



