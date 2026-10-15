#include "list.h"

void dlist_init(DoublyList& l) {
    l.head = l.tail = 0;
    l.count = 0;
}

void dlist_free(DoublyList& l) {
    LNode* cur = l.head;
    while (cur) {
        LNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    l.head = l.tail = 0;
    l.count = 0;
}

void dlist_push(DoublyList& l, const string& value) {
    LNode* n = new LNode;
    n->value = value;
    n->prev = l.tail;
    n->next = 0;
    if (!l.head) { l.head = l.tail = n; }
    else { l.tail->next = n; l.tail = n; }
    ++l.count;
    cout << "OK" << endl;
}

void dlist_push_head(DoublyList& l, const string& value) {
    LNode* n = new LNode;
    n->value = value;
    n->prev = 0;
    n->next = l.head;
    if (!l.head) { l.head = l.tail = n; }
    else { l.head->prev = n; l.head = n; }
    ++l.count;
    cout << "OK" << endl;
}

void dlist_push_after(DoublyList& l, const string& value, const string& afterValue) {
    LNode* cur = l.head;
    while (cur && cur->value != afterValue) cur = cur->next;
    if (!cur) { cout << "ERROR: not found" << endl; return; }
    LNode* n = new LNode;
    n->value = value;
    n->prev = cur;
    n->next = cur->next;
    if (cur->next) cur->next->prev = n;
    else l.tail = n;
    cur->next = n;
    ++l.count;
    cout << "OK" << endl;
}

void dlist_push_before(DoublyList& l, const string& value, const string& beforeValue) {
    LNode* cur = l.head;
    while (cur && cur->value != beforeValue) cur = cur->next;
    if (!cur) { cout << "ERROR: not found" << endl; return; }
    LNode* n = new LNode;
    n->value = value;
    n->next = cur;
    n->prev = cur->prev;
    if (cur->prev) cur->prev->next = n;
    else l.head = n;
    cur->prev = n;
    ++l.count;
    cout << "OK" << endl;
}

void dlist_del_first(DoublyList& l) {
    if (!l.head) { cout << "ERROR: empty" << endl; return; }
    LNode* tmp = l.head;
    l.head = l.head->next;
    if (l.head) l.head->prev = 0;
    else l.tail = 0;
    delete tmp;
    --l.count;
    cout << "OK" << endl;
}

void dlist_del_last(DoublyList& l) {
    if (!l.tail) { cout << "ERROR: empty" << endl; return; }
    LNode* tmp = l.tail;
    l.tail = l.tail->prev;
    if (l.tail) l.tail->next = 0;
    else l.head = 0;
    delete tmp;
    --l.count;
    cout << "OK" << endl;
}

void dlist_del_value(DoublyList& l, const string& value) {
    LNode* cur = l.head;
    while (cur && cur->value != value) cur = cur->next;
    if (!cur) { cout << "ERROR: not found" << endl; return; }
    if (cur->prev) cur->prev->next = cur->next;
    else l.head = cur->next;
    if (cur->next) cur->next->prev = cur->prev;
    else l.tail = cur->prev;
    delete cur;
    --l.count;
    cout << "OK" << endl;
}

// Удалить узел ПЕРЕД узлом с индексом index.
void dlist_del_before(DoublyList& l, int index) {
    if (!l.head) { cout << "ERROR: empty list" << endl; return; }
    if (index <= 0) { cout << "ERROR: index out of range" << endl; return; }

    LNode* cur = l.head;
    int i = 0;
    while (cur && i < index) { cur = cur->next; ++i; }
    if (!cur) { cout << "ERROR: index out of range" << endl; return; }

    LNode* tmp = cur->prev;
    if (!tmp) { cout << "ERROR: nothing before this node" << endl; return; }

    if (tmp->prev) tmp->prev->next = cur;
    else l.head = cur;
    cur->prev = tmp->prev;

    delete tmp;
    --l.count;
    cout << "OK" << endl;
}

// Удалить узел ПОСЛЕ узла с индексом index.
void dlist_del_after(DoublyList& l, int index) {
    if (!l.head) { cout << "ERROR: empty list" << endl; return; }

    LNode* cur = l.head;
    int i = 0;
    while (cur && i < index) { cur = cur->next; ++i; }
    if (!cur) { cout << "ERROR: index out of range" << endl; return; }

    LNode* tmp = cur->next;
    if (!tmp) { cout << "ERROR: nothing after this node" << endl; return; }

    cur->next = tmp->next;
    if (tmp->next) tmp->next->prev = cur;
    else l.tail = cur;

    delete tmp;
    --l.count;
    cout << "OK" << endl;
}

void dlist_get(const DoublyList& l, const string& value) {
    LNode* cur = l.head;
    int idx = 0;
    while (cur) {
        if (cur->value == value) {
            cout << "FOUND at index " << idx << endl;
            return;
        }
        cur = cur->next;
        ++idx;
    }
    cout << "NOT FOUND" << endl;
}

void dlist_get_at(const DoublyList& l, int index) {
    LNode* cur = l.head;
    int i = 0;
    while (cur && i < index) { cur = cur->next; ++i; }
    if (!cur) { cout << "ERROR: index out of range" << endl; return; }
    cout << cur->value << endl;
}

void dlist_len(const DoublyList& l) { cout << l.count << endl; }

void dlist_print(const DoublyList& l) {
    cout << "[ ";
    LNode* cur = l.head;
    while (cur) { cout << cur->value << " "; cur = cur->next; }
    cout << "]" << endl;
}

void dlist_print_reverse(const DoublyList& l) {
    cout << "[ ";
    LNode* cur = l.tail;
    while (cur) { cout << cur->value << " "; cur = cur->prev; }
    cout << "]" << endl;
}

void dlist_save(const DoublyList& l, ostream& os) {
    os << l.count << "\n";
    LNode* cur = l.head;
    while (cur) { os << cur->value << "\n"; cur = cur->next; }
}

void dlist_load(DoublyList& l, istream& is) {
    dlist_free(l);
    int n; is >> n;
    for (int i = 0; i < n; ++i) {
        string s;
        is >> s;
        LNode* node = new LNode;
        node->value = s;
        node->prev = l.tail;
        node->next = 0;
        if (!l.head) l.head = l.tail = node;
        else { l.tail->next = node; l.tail = node; }
        ++l.count;
    }
}