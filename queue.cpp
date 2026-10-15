#include "queue.h"

void queue_init(Queue& q) {
    q.head = q.tail = 0;
    q.count = 0;
}

void queue_free(Queue& q) {
    while (q.head) {
        QNode* n = q.head->next;
        delete q.head;
        q.head = n;
    }
    q.tail = 0;
    q.count = 0;
}

void queue_push(Queue& q, const string& value) {
    QNode* n = new QNode;
    n->value = value;
    n->next = 0;
    if (!q.tail) { q.head = q.tail = n; }
    else { q.tail->next = n; q.tail = n; }
    ++q.count;
    cout << "OK" << endl;
}

void queue_pop(Queue& q) {
    if (!q.head) { cout << "ERROR: queue is empty" << endl; return; }
    cout << q.head->value << endl;
    QNode* tmp = q.head;
    q.head = q.head->next;
    if (!q.head) q.tail = 0;
    delete tmp;
    --q.count;
}

void queue_peek(const Queue& q) {
    if (!q.head) { cout << "ERROR: queue is empty" << endl; return; }
    cout << q.head->value << endl;
}

void queue_len(const Queue& q) { cout << q.count << endl; }

void queue_print(const Queue& q) {
    cout << "[ ";
    QNode* cur = q.head;
    while (cur) { cout << cur->value << " "; cur = cur->next; }
    cout << "]" << endl;
}

void queue_save(const Queue& q, ostream& os) {
    os << q.count << "\n";
    QNode* cur = q.head;
    while (cur) { os << cur->value << "\n"; cur = cur->next; }
}

void queue_load(Queue& q, istream& is) {
    queue_free(q);
    int n; is >> n;
    for (int i = 0; i < n; ++i) {
        string s;
        is >> s;
        QNode* node = new QNode;
        node->value = s;
        node->next = 0;
        if (!q.tail) q.head = q.tail = node;
        else { q.tail->next = node; q.tail = node; }
        ++q.count;
    }
}