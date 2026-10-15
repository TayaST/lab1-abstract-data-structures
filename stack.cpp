#include "stack.h"

void stack_init(Stack& s) {
    s.top = 0;
    s.count = 0;
}

void stack_free(Stack& s) {
    while (s.top) {
        SNode* n = s.top->next;
        delete s.top;
        s.top = n;
    }
    s.count = 0;
}

void stack_push(Stack& s, const string& value) {
    SNode* n = new SNode;
    n->value = value;
    n->next = s.top;
    s.top = n;
    ++s.count;
    cout << "OK" << endl;
}

void stack_pop(Stack& s) {
    if (!s.top) { cout << "ERROR: stack is empty" << endl; return; }
    cout << s.top->value << endl;
    SNode* tmp = s.top;
    s.top = s.top->next;
    delete tmp;
    --s.count;
}

void stack_peek(const Stack& s) {
    if (!s.top) { cout << "ERROR: stack is empty" << endl; return; }
    cout << s.top->value << endl;
}

void stack_len(const Stack& s) { cout << s.count << endl; }

void stack_print(const Stack& s) {
    cout << "[ ";
    SNode* cur = s.top;
    while (cur) { cout << cur->value << " "; cur = cur->next; }
    cout << "]" << endl;
}

void stack_save(const Stack& s, ostream& os) {
    os << s.count << "\n";
    SNode* cur = s.top;
    while (cur) { os << cur->value << "\n"; cur = cur->next; }
}

void stack_load(Stack& s, istream& is) {
    stack_free(s);
    int n; is >> n;
    // читаем в обратном порядке, чтобы верх стека оказался первым
    string* buf = new string[n > 0 ? n : 1];
    for (int i = 0; i < n; ++i) is >> buf[i];
    for (int i = n - 1; i >= 0; --i) {
        SNode* node = new SNode;
        node->value = buf[i];
        node->next = s.top;
        s.top = node;
        ++s.count;
    }
    delete[] buf;
}