#include "forward_list.h"

void flist_init(ForwardList& l) {
    l.head = 0;
    l.tail = 0;
    l.count = 0;
}

void flist_free(ForwardList& l) {
    FNode* cur = l.head;
    while (cur) {
        FNode* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    l.head = l.tail = 0;
    l.count = 0;
}

void flist_push(ForwardList& l, const string& value) {
    FNode* n = new FNode;
    n->value = value;
    n->next = 0;
    if (!l.head) { l.head = l.tail = n; }
    else { l.tail->next = n; l.tail = n; }
    ++l.count;
    cout << "OK" << endl;
}

void flist_push_head(ForwardList& l, const string& value) {
    FNode* n = new FNode;
    n->value = value;
    n->next = l.head;
    l.head = n;
    if (!l.tail) l.tail = n;
    ++l.count;
    cout << "OK" << endl;
}

void flist_push_after(ForwardList& l, const string& value, const string& afterValue) {
    FNode* cur = l.head;
    while (cur && cur->value != afterValue) cur = cur->next;
    if (!cur) { cout << "ERROR: not found" << endl; return; }
    FNode* n = new FNode;
    n->value = value;
    n->next = cur->next;
    cur->next = n;
    if (cur == l.tail) l.tail = n;
    ++l.count;
    cout << "OK" << endl;
}

void flist_push_before(ForwardList& l, const string& value, const string& beforeValue) {
    if (!l.head) { cout << "ERROR: empty list" << endl; return; }
    if (l.head->value == beforeValue) { flist_push_head(l, value); return; }
    FNode* prev = l.head;
    while (prev->next && prev->next->value != beforeValue) prev = prev->next;
    if (!prev->next) { cout << "ERROR: not found" << endl; return; }
    FNode* n = new FNode;
    n->value = value;
    n->next = prev->next;
    prev->next = n;
    ++l.count;
    cout << "OK" << endl;
}

void flist_del_first(ForwardList& l) {
    if (!l.head) { cout << "ERROR: empty list" << endl; return; }
    FNode* tmp = l.head;
    l.head = l.head->next;
    if (!l.head) l.tail = 0;
    delete tmp;
    --l.count;
    cout << "OK" << endl;
}

void flist_del_last(ForwardList& l) {
    if (!l.head) { cout << "ERROR: empty list" << endl; return; }
    if (l.head == l.tail) {
        delete l.head;
        l.head = l.tail = 0;
        --l.count;
        cout << "OK" << endl;
        return;
    }
    FNode* cur = l.head;
    while (cur->next != l.tail) cur = cur->next;
    delete l.tail;
    l.tail = cur;
    l.tail->next = 0;
    --l.count;
    cout << "OK" << endl;
}

void flist_del_value(ForwardList& l, const string& value) {
    if (!l.head) { cout << "ERROR: empty list" << endl; return; }
    if (l.head->value == value) { flist_del_first(l); return; }
    FNode* prev = l.head;
    while (prev->next && prev->next->value != value) prev = prev->next;
    if (!prev->next) { cout << "ERROR: not found" << endl; return; }
    FNode* tmp = prev->next;
    prev->next = tmp->next;
    if (tmp == l.tail) l.tail = prev;
    delete tmp;
    --l.count;
    cout << "OK" << endl;
}

void flist_del_before(ForwardList& l, int index) {
    if (!l.head) { cout << "ERROR: empty list" << endl; return; }
    if (index <= 0) { cout << "ERROR: index out of range" << endl; return; }

    if (index == 1) {
        flist_del_first(l);
        return;
    }

    // Идём до узла с индексом (index - 2), чтобы его next
    // был удаляемым (узел с индексом index - 1)
    FNode* prev = l.head;
    int i = 0;
    while (prev && i < index - 2) { prev = prev->next; ++i; }

    if (!prev || !prev->next) {
        cout << "ERROR: index out of range" << endl;
        return;
    }
    FNode* tmp = prev->next;
    prev->next = tmp->next;
    if (tmp == l.tail) l.tail = prev;
    delete tmp;
    --l.count;
    cout << "OK" << endl;
}

void flist_del_after(ForwardList& l, int index) {
    if (!l.head) { cout << "ERROR: empty list" << endl; return; }

    FNode* cur = l.head;
    int i = 0;
    while (cur && i < index) { cur = cur->next; ++i; }

    if (!cur) { cout << "ERROR: index out of range" << endl; return; }
    if (!cur->next) { cout << "ERROR: nothing after this node" << endl; return; }

    FNode* tmp = cur->next;
    cur->next = tmp->next;
    if (tmp == l.tail) l.tail = cur;
    delete tmp;
    --l.count;
    cout << "OK" << endl;
}

void flist_get(const ForwardList& l, const string& value) {
    FNode* cur = l.head;
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

void flist_get_at(const ForwardList& l, int index) {
    FNode* cur = l.head;
    int i = 0;
    while (cur && i < index) { cur = cur->next; ++i; }
    if (!cur) { cout << "ERROR: index out of range" << endl; return; }
    cout << cur->value << endl;
}

void flist_len(const ForwardList& l) { cout << l.count << endl; }

void flist_print(const ForwardList& l) {
    cout << "[ ";
    FNode* cur = l.head;
    while (cur) { cout << cur->value << " "; cur = cur->next; }
    cout << "]" << endl;
}

void flist_save(const ForwardList& l, ostream& os) {
    os << l.count << "\n";
    FNode* cur = l.head;
    while (cur) { os << cur->value << "\n"; cur = cur->next; }
}

void flist_load(ForwardList& l, istream& is) {
    flist_free(l);
    int n; is >> n;
    for (int i = 0; i < n; ++i) {
        string s;
        is >> s;
        FNode* node = new FNode;
        node->value = s;
        node->next = 0;
        if (!l.head) l.head = l.tail = node;
        else { l.tail->next = node; l.tail = node; }
        ++l.count;
    }
}