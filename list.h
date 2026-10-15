#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct LNode {
    string value;
    LNode* prev;
    LNode* next;
};

struct DoublyList {
    LNode* head;
    LNode* tail;
    int count;
};

void dlist_init(DoublyList& l);
void dlist_free(DoublyList& l);

void dlist_push(DoublyList& l, const string& value);
void dlist_push_head(DoublyList& l, const string& value);
void dlist_push_after(DoublyList& l, const string& value, const string& afterValue);
void dlist_push_before(DoublyList& l, const string& value, const string& beforeValue);
void dlist_del_first(DoublyList& l);
void dlist_del_last(DoublyList& l);
void dlist_del_value(DoublyList& l, const string& value);
void dlist_del_before(DoublyList& l, int index);
void dlist_del_after(DoublyList& l, int index);
void dlist_get(const DoublyList& l, const string& value);
void dlist_get_at(const DoublyList& l, int index);
void dlist_len(const DoublyList& l);
void dlist_print(const DoublyList& l);
void dlist_print_reverse(const DoublyList& l);

void dlist_save(const DoublyList& l, ostream& os);
void dlist_load(DoublyList& l, istream& is);

#endif