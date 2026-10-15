#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct SNode {
    string value;
    SNode* next;
};

struct Stack {
    SNode* top;
    int count;
};

void stack_init(Stack& s);
void stack_free(Stack& s);

void stack_push(Stack& s, const string& value);
void stack_pop(Stack& s);
void stack_peek(const Stack& s);
void stack_len(const Stack& s);
void stack_print(const Stack& s);

void stack_save(const Stack& s, ostream& os);
void stack_load(Stack& s, istream& is);

#endif