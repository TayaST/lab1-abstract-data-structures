#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct MyArray {
    string* data;
    int capacity;
    int length;
};

void array_init(MyArray& a);
void array_free(MyArray& a);

void array_push(MyArray& a, const string& value);
void array_push_at(MyArray& a, int index, const string& value);
void array_remove_at(MyArray& a, int index);
void array_get(const MyArray& a, int index);
void array_set(MyArray& a, int index, const string& value);
void array_len(const MyArray& a);
void array_print(const MyArray& a);

void array_save(const MyArray& a, ostream& os);
void array_load(MyArray& a, istream& is);

#endif