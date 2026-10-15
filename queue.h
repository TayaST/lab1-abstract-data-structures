#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct QNode {
    string value;
    QNode* next;
};

struct Queue {
    QNode* head;
    QNode* tail;
    int count;
};

void queue_init(Queue& q);
void queue_free(Queue& q);

void queue_push(Queue& q, const string& value);
void queue_pop(Queue& q);
void queue_peek(const Queue& q);
void queue_len(const Queue& q);
void queue_print(const Queue& q);

void queue_save(const Queue& q, ostream& os);
void queue_load(Queue& q, istream& is);

#endif