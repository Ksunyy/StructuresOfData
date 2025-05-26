#include <iostream>
#include <string>
#include <utility>
#include <cctype>
#include <stack>
#include "Polynomials.h"
#include "Storage.h"



class Parser {
private:
    std::pair<std::string, std::string> stringExpr;
public:
    std::pair<std::string, std::string> FirstParse(const std::string& data) {
        stringExpr.first.clear();
        stringExpr.second.clear();

        int chek = 0;
        for (int i = 0; i < data.length(); i++) {
            if (data[i] == '=') chek = 1;
            if (!chek) stringExpr.first += data[i];
            if (chek && data[i] != '=')   stringExpr.second += data[i];
        }

        for (char c : stringExpr.first) {
            if (c == 'x' || c == 'y' || c == 'z') {
                throw std::runtime_error("Variable names cannot contain x, y, or z.");
            }
        }


        return stringExpr;
    }

    int Priority(char oper) {
        if (oper == '+' || oper == '-') return 1;
        if (oper == '*') return 2;
        return 0;

    }

    Polynomials applyOp(Polynomials& a, Polynomials& b, char op) {
        switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        default: throw std::runtime_error("Unknown operator");
        }
    }
    Polynomials parser(const std::string& str, Storage& st) {
        std::stack<Polynomials> operands;
        std::stack<char> operators;
        std::string token;

        for (int i = 0; i < str.size(); i++) {
            char ch = str[i];
            if (std::isspace(ch)) continue;

            if (std::isalnum(ch) || ch == '^' || ch == 'x' || ch == 'y' || ch == 'z' || ch == '.') {
                token.clear();
                token += ch;

                while (i + 1 < (int)str.size()) {
                    char next_ch = str[i + 1];
                    if (std::isalnum(next_ch) || next_ch == '^' || next_ch == 'x' || next_ch == 'y' || next_ch == 'z' || next_ch == '.') {
                        token += next_ch;
                        i++;
                    }
                    else break;
                }

                // Проверка — имя переменной или сам полином
                if (st.find(token)) {
                    operands.push(st.get(token));
                }
                else {
                    try {
                        operands.push(Polynomials(token));
                    }
                    catch (...) {
                        throw std::runtime_error("Unknown variable or invalid polynomial: " + token);
                    }
                }
            }

            else if (ch == '(') {
                operators.push(ch);
            }

            else if (ch == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    auto right = operands.top(); operands.pop();
                    auto left = operands.top(); operands.pop();
                    char op = operators.top(); operators.pop();
                    operands.push(applyOp(left, right, op));
                }
                if (!operators.empty()) operators.pop(); // Remove '('
            }

            else if (ch == '+' || ch == '-' || ch == '*') {
                while (!operators.empty() && Priority(operators.top()) >= Priority(ch)) {
                    auto right = operands.top(); operands.pop();
                    auto left = operands.top(); operands.pop();
                    char op = operators.top(); operators.pop();
                    operands.push(applyOp(left, right, op));
                }
                operators.push(ch);
            }

            else {
                throw std::runtime_error(std::string("Invalid character in expression: ") + ch);
            }
        }

        while (!operators.empty()) {
            auto right = operands.top(); operands.pop();
            auto left = operands.top(); operands.pop();
            char op = operators.top(); operators.pop();
            operands.push(applyOp(left, right, op));
        }

        if (operands.empty()) {
            throw std::runtime_error("Empty or invalid expression");
        }

        return operands.top();
    }


    void Parse(const std::string& str, Storage& st) {
        stringExpr = this->FirstParse(str);

        Polynomials p = this->parser(stringExpr.second, st);
        st.insert(stringExpr.first, p);

    }


};


