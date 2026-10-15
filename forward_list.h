#ifndef FORWARD_LIST_H
#define FORWARD_LIST_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct FNode {
    string value;
    FNode* next;
};

struct ForwardList {
    FNode* head;
    FNode* tail;
    int count;
};

void flist_init(ForwardList& l);
void flist_free(ForwardList& l);

void flist_push(ForwardList& l, const string& value);
void flist_push_head(ForwardList& l, const string& value);
void flist_push_after(ForwardList& l, const string& value, const string& afterValue);
void flist_push_before(ForwardList& l, const string& value, const string& beforeValue);
void flist_del_first(ForwardList& l);
void flist_del_last(ForwardList& l);
void flist_del_value(ForwardList& l, const string& value);
void flist_del_before(ForwardList& l, int index);
void flist_del_after(ForwardList& l, int index);
void flist_get(const ForwardList& l, const string& value);
void flist_get_at(const ForwardList& l, int index);
void flist_len(const ForwardList& l);
void flist_print(const ForwardList& l);

void flist_save(const ForwardList& l, ostream& os);
void flist_load(ForwardList& l, istream& is);

#endif