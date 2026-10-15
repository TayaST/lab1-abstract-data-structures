#include "array.h"

static void array_reserve(MyArray& a, int newCap) {
    if (newCap <= a.capacity) return;
    string* nd = new string[newCap];
    for (int i = 0; i < a.length; ++i) nd[i] = a.data[i];
    delete[] a.data;
    a.data = nd;
    a.capacity = newCap;
}

void array_init(MyArray& a) {
    a.data = 0;
    a.capacity = 0;
    a.length = 0;
}

void array_free(MyArray& a) {
    delete[] a.data;
    a.data = 0;
    a.capacity = 0;
    a.length = 0;
}

void array_push(MyArray& a, const string& value) {
    if (a.length == a.capacity) array_reserve(a, a.capacity == 0 ? 4 : a.capacity * 2);
    a.data[a.length++] = value;
    cout << "OK" << endl;
}

void array_push_at(MyArray& a, int index, const string& value) {
    if (index < 0 || index > a.length) {
        cout << "ERROR: index out of range" << endl;
        return;
    }
    if (a.length == a.capacity) array_reserve(a, a.capacity == 0 ? 4 : a.capacity * 2);
    for (int i = a.length; i > index; --i) a.data[i] = a.data[i - 1];
    a.data[index] = value;
    ++a.length;
    cout << "OK" << endl;
}

void array_remove_at(MyArray& a, int index) {
    if (index < 0 || index >= a.length) {
        cout << "ERROR: index out of range" << endl;
        return;
    }
    for (int i = index; i < a.length - 1; ++i) a.data[i] = a.data[i + 1];
    --a.length;
    cout << "OK" << endl;
}

void array_get(const MyArray& a, int index) {
    if (index < 0 || index >= a.length) {
        cout << "ERROR: index out of range" << endl;
        return;
    }
    cout << a.data[index] << endl;
}

void array_set(MyArray& a, int index, const string& value) {
    if (index < 0 || index >= a.length) {
        cout << "ERROR: index out of range" << endl;
        return;
    }
    a.data[index] = value;
    cout << "OK" << endl;
}

void array_len(const MyArray& a) {
    cout << a.length << endl;
}

void array_print(const MyArray& a) {
    cout << "[ ";
    for (int i = 0; i < a.length; ++i) cout << a.data[i] << " ";
    cout << "]" << endl;
}

void array_save(const MyArray& a, ostream& os) {
    os << a.length << "\n";
    for (int i = 0; i < a.length; ++i) os << a.data[i] << "\n";
}

void array_load(MyArray& a, istream& is) {
    array_free(a);
    int n; is >> n;
    for (int i = 0; i < n; ++i) {
        string s;
        is >> s;
        array_push(a, s);
    }
}