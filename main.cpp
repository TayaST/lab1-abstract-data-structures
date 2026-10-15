#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "array.h"
#include "forward_list.h"
#include "list.h"
#include "stack.h"
#include "queue.h"
#include "tree.h"

// ===================== РЕЕСТР ИМЕНОВАННЫХ СТРУКТУР =====================

struct Entry {
    string name;
    char type;                  // 'M' | 'F' | 'L' | 'S' | 'Q' | 'T'
    MyArray arr;
    ForwardList fl;
    DoublyList dl;
    Stack st;
    Queue q;
    CompleteBinaryTree tr;
    Entry* next;
};

struct Registry {
    Entry* head;
    Entry* tail;
    int count;
};

static void registry_init(Registry& r) {
    r.head = r.tail = 0;
    r.count = 0;
}

static Entry* registry_find(Registry& r, const string& name) {
    Entry* cur = r.head;
    while (cur) {
        if (cur->name == name) return cur;
        cur = cur->next;
    }
    return 0;
}

static Entry* registry_create(Registry& r, const string& name, char type) {
    Entry* e = new Entry;
    e->name = name;
    e->type = type;
    array_init(e->arr);
    flist_init(e->fl);
    dlist_init(e->dl);
    stack_init(e->st);
    queue_init(e->q);
    tree_init(e->tr);
    e->next = 0;
    if (!r.head) r.head = r.tail = e;
    else { r.tail->next = e; r.tail = e; }
    ++r.count;
    return e;
}

// Найти или создать структуру нужного типа.
// Если есть запись с таким именем, но другого типа — ошибка.
static Entry* registry_get_or_create(Registry& r, const string& name, char type) {
    Entry* e = registry_find(r, name);
    if (e) {
        if (e->type != type) {
            cout << "ERROR: name '" << name << "' is used for another structure type" << endl;
            return 0;
        }
        return e;
    }
    return registry_create(r, name, type);
}

static void registry_free(Registry& r) {
    Entry* cur = r.head;
    while (cur) {
        Entry* nxt = cur->next;
        array_free(cur->arr);
        flist_free(cur->fl);
        dlist_free(cur->dl);
        stack_free(cur->st);
        queue_free(cur->q);
        tree_free(cur->tr);
        delete cur;
        cur = nxt;
    }
    r.head = r.tail = 0;
    r.count = 0;
}

// ===================== СОХРАНЕНИЕ / ЗАГРУЗКА ФАЙЛА =====================
// Формат: <префикс><имя> : elem1,elem2,elem3
// Префиксы: M_, F_, L_, S_, Q_, T_

static string typePrefix(char t) {
    switch (t) {
        case 'M': return "M_";
        case 'F': return "F_";
        case 'L': return "L_";
        case 'S': return "S_";
        case 'Q': return "Q_";
        case 'T': return "T_";
    }
    return "";
}

static void saveRegistry(const string& file, Registry& r) {
    ofstream os(file.c_str());
    if (!os.is_open()) return;
    Entry* cur = r.head;
    while (cur) {
        os << typePrefix(cur->type) << cur->name << " : ";
        bool first = true;
        switch (cur->type) {
            case 'M': {
                for (int i = 0; i < cur->arr.length; ++i) {
                    if (!first) os << ",";
                    os << cur->arr.data[i];
                    first = false;
                }
                break;
            }
            case 'F': {
                FNode* n = cur->fl.head;
                while (n) {
                    if (!first) os << ",";
                    os << n->value;
                    first = false;
                    n = n->next;
                }
                break;
            }
            case 'L': {
                LNode* n = cur->dl.head;
                while (n) {
                    if (!first) os << ",";
                    os << n->value;
                    first = false;
                    n = n->next;
                }
                break;
            }
            case 'S': {
                // Хотим хранить снизу-вверх
                int n = cur->st.count;
                string* buf = new string[n > 0 ? n : 1];
                SNode* p = cur->st.top;
                for (int i = n - 1; i >= 0; --i) {
                    buf[i] = p->value;
                    p = p->next;
                }
                for (int i = 0; i < n; ++i) {
                    if (!first) os << ",";
                    os << buf[i];
                    first = false;
                }
                delete[] buf;
                break;
            }
            case 'Q': {
                QNode* n = cur->q.head;
                while (n) {
                    if (!first) os << ",";
                    os << n->value;
                    first = false;
                    n = n->next;
                }
                break;
            }
            case 'T': {
                // BST: pre-order через запятую
                TNode* stack[1024];
                int sp = 0;
                if (cur->tr.root) stack[sp++] = cur->tr.root;
                while (sp > 0) {
                    TNode* n = stack[--sp];
                    if (!first) os << ",";
                    os << n->value;
                    first = false;
                    if (n->right) stack[sp++] = n->right;
                    if (n->left) stack[sp++] = n->left;
                }
                break;
            }
        }
        os << "\n";
        cur = cur->next;
    }
    os.close();
}

// Разбор строки "имя : elem1,elem2,..."
static bool parseLine(const string& line, string& rawName, string& content) {
    int colon = -1;
    for (int i = 0; i < (int)line.size(); ++i) {
        if (line[i] == ':') { colon = i; break; }
    }
    if (colon < 0) return false;

    rawName = "";
    int i = 0;
    while (i < colon && line[i] == ' ') ++i;
    while (i < colon && line[i] != ' ') rawName += line[i++];

    content = "";
    int j = colon + 1;
    while (j < (int)line.size() && line[j] == ' ') ++j;
    while (j < (int)line.size()) content += line[j++];
    while (!content.empty() && content[content.size()-1] == ' ') content.erase(content.size()-1);
    return !rawName.empty();
}

// Разбор содержимого "a,b,c" в массив строк
static int splitContent(const string& s, string items[], int maxItems) {
    int n = 0;
    string cur = "";
    for (int i = 0; i < (int)s.size(); ++i) {
        if (s[i] == ',') {
            if (n < maxItems) items[n++] = cur;
            cur = "";
        } else {
            cur += s[i];
        }
    }
    if (!cur.empty() && n < maxItems) items[n++] = cur;
    return n;
}

static void loadRegistry(const string& file, Registry& r) {
    ifstream is(file.c_str());
    if (!is.is_open()) return;

    string line;
    while (getline(is, line)) {
        if (line.empty()) continue;

        string rawName, content;
        if (!parseLine(line, rawName, content)) continue;

        char type = 0;
        string name = rawName;
        if (rawName.size() >= 2 && rawName[1] == '_') {
            char c = rawName[0];
            if (c == 'M' || c == 'F' || c == 'L' || c == 'S' || c == 'Q' || c == 'T') {
                type = c;
                name = rawName.substr(2);
            }
        }
        if (type == 0) continue;

        Entry* e = registry_create(r, name, type);

        string items[4096];
        int n = splitContent(content, items, 4096);

        switch (type) {
            case 'M':
                for (int i = 0; i < n; ++i) array_push(e->arr, items[i]);
                break;
            case 'F':
                for (int i = 0; i < n; ++i) flist_push(e->fl, items[i]);
                break;
            case 'L':
                for (int i = 0; i < n; ++i) dlist_push(e->dl, items[i]);
                break;
            case 'S':
                for (int i = 0; i < n; ++i) stack_push(e->st, items[i]);
                break;
            case 'Q':
                for (int i = 0; i < n; ++i) queue_push(e->q, items[i]);
                break;
            case 'T':
                for (int i = 0; i < n; ++i) {
                    int v = 0, k = 0, sign = 1;
                    if (k < (int)items[i].size() && items[i][k] == '-') { sign = -1; ++k; }
                    for (; k < (int)items[i].size(); ++k) {
                        if (items[i][k] < '0' || items[i][k] > '9') break;
                        v = v * 10 + (items[i][k] - '0');
                    }
                    tree_insert(e->tr, v * sign);
                }
                break;
        }
    }
    is.close();
}

// ===================== ВСПОМОГАТЕЛЬНОЕ =====================

static int split(const string& s, string tok[], int maxTok) {
    int n = 0;
    int i = 0, len = (int)s.size();
    while (i < len && n < maxTok) {
        while (i < len && s[i] == ' ') ++i;
        if (i >= len) break;
        string cur;
        while (i < len && s[i] != ' ') cur += s[i++];
        tok[n++] = cur;
    }
    return n;
}

static int toInt(const string& s) {
    int v = 0, i = 0, sign = 1;
    if (i < (int)s.size() && s[i] == '-') { sign = -1; ++i; }
    for (; i < (int)s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') break;
        v = v * 10 + (s[i] - '0');
    }
    return v * sign;
}

static void printHelp() {
    cout << "=== Commands (name-aware) ===" << endl;
    cout << "Array (M):   MPUSH <name> v | MPUSHAT <name> i v | MDEL <name> i | MGET <name> i | MSET <name> i v | MLEN <name>" << endl;
    cout << "FwdList (F): FPUSH <name> v | FPUSHHEAD <name> v | FPUSHAFTER <name> v a | FPUSHBEFORE <name> v b" << endl;
    cout << "             FDELFIRST <name> | FDELLAST <name> | FDEL <name> v | FDELBEFORE <name> i | FDELAFTER <name> i" << endl;
    cout << "             FGET <name> v | FGETAT <name> i | FLEN <name>" << endl;
    cout << "DblList (L): LPUSH <name> v | LPUSHHEAD <name> v | LPUSHAFTER <name> v a | LPUSHBEFORE <name> v b" << endl;
    cout << "             LDELFIRST <name> | LDELLAST <name> | LDEL <name> v | LDELBEFORE <name> i | LDELAFTER <name> i" << endl;
    cout << "             LGET <name> v | LGETAT <name> i | LPRINTREV <name> | LLEN <name>" << endl;
    cout << "Stack (S):   SPUSH <name> v | SPOP <name> | SPEEK <name> | SLEN <name>" << endl;
    cout << "Queue (Q):   QPUSH <name> v | QPOP <name> | QPEEK <name> | QLEN <name>" << endl;
    cout << "Tree (T):    TINSERT <name> v | TREMOVE <name> v | TGET <name> v | ISMEMBER <name> v" << endl;
    cout << "             TCOMPLETE <name> | INORDER <name> | PREORDER <name> | POSTORDER <name> | LEVELORDER <name> | TLEN <name>" << endl;
    cout << "Common:      PRINT <name> | HELP | EXIT" << endl;
}

// ===================== ОБРАБОТКА КОМАНД =====================

static void processCommand(const string& query, Registry& r, const string& filename) {
    string tok[16];
    int n = split(query, tok, 16);
    if (n == 0) return;
    string cmd = tok[0];

    // ---- Массив ----
    if (cmd == "MPUSH" && n >= 3) {
        Entry* e = registry_get_or_create(r, tok[1], 'M');
        if (e) array_push(e->arr, tok[2]);
    }
    else if (cmd == "MPUSHAT" && n >= 4) {
        Entry* e = registry_get_or_create(r, tok[1], 'M');
        if (e) array_push_at(e->arr, toInt(tok[2]), tok[3]);
    }
    else if (cmd == "MDEL" && n >= 3) {
        Entry* e = registry_get_or_create(r, tok[1], 'M');
        if (e) array_remove_at(e->arr, toInt(tok[2]));
    }
    else if (cmd == "MGET" && n >= 3) {
        Entry* e = registry_get_or_create(r, tok[1], 'M');
        if (e) array_get(e->arr, toInt(tok[2]));
    }
    else if (cmd == "MSET" && n >= 4) {
        Entry* e = registry_get_or_create(r, tok[1], 'M');
        if (e) array_set(e->arr, toInt(tok[2]), tok[3]);
    }
    else if (cmd == "MLEN" && n >= 2) {
        Entry* e = registry_get_or_create(r, tok[1], 'M');
        if (e) array_len(e->arr);
    }

    // ---- Односвязный ----
    else if (cmd == "FPUSH" && n >= 3) {
        Entry* e = registry_get_or_create(r, tok[1], 'F');
        if (e) flist_push(e->fl, tok[2]);
    }
    else if (cmd == "FPUSHHEAD" && n >= 3) {
        Entry* e = registry_get_or_create(r, tok[1], 'F');
        if (e) flist_push_head(e->fl, tok[2]);
    }
    else if (cmd == "FPUSHAFTER" && n >= 4) {
        Entry* e = registry_get_or_create(r, tok[1], 'F');
        if (e) flist_push_after(e->fl, tok[2], tok[3]);
    }
    else if (cmd == "FPUSHBEFORE" && n >= 4) {
        Entry* e = registry_get_or_create(r, tok[1], 'F');
        if (e) flist_push_before(e->fl, tok[2], tok[3]);
    }
    else if (cmd == "FDELFIRST" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'F') flist_del_first(e->fl);
        else cout << "ERROR: forward list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "FDELLAST" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'F') flist_del_last(e->fl);
        else cout << "ERROR: forward list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "FDEL" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'F') flist_del_value(e->fl, tok[2]);
        else cout << "ERROR: forward list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "FDELBEFORE" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'F') flist_del_before(e->fl, toInt(tok[2]));
        else cout << "ERROR: forward list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "FDELAFTER" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'F') flist_del_after(e->fl, toInt(tok[2]));
        else cout << "ERROR: forward list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "FGET" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'F') flist_get(e->fl, tok[2]);
        else cout << "ERROR: forward list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "FGETAT" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'F') flist_get_at(e->fl, toInt(tok[2]));
        else cout << "ERROR: forward list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "FLEN" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'F') flist_len(e->fl);
        else cout << "ERROR: forward list '" << tok[1] << "' not found" << endl;
    }

    // ---- Двусвязный ----
    else if (cmd == "LPUSH" && n >= 3) {
        Entry* e = registry_get_or_create(r, tok[1], 'L');
        if (e) dlist_push(e->dl, tok[2]);
    }
    else if (cmd == "LPUSHHEAD" && n >= 3) {
        Entry* e = registry_get_or_create(r, tok[1], 'L');
        if (e) dlist_push_head(e->dl, tok[2]);
    }
    else if (cmd == "LPUSHAFTER" && n >= 4) {
        Entry* e = registry_get_or_create(r, tok[1], 'L');
        if (e) dlist_push_after(e->dl, tok[2], tok[3]);
    }
    else if (cmd == "LPUSHBEFORE" && n >= 4) {
        Entry* e = registry_get_or_create(r, tok[1], 'L');
        if (e) dlist_push_before(e->dl, tok[2], tok[3]);
    }
    else if (cmd == "LDELFIRST" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'L') dlist_del_first(e->dl);
        else cout << "ERROR: doubly list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "LDELLAST" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'L') dlist_del_last(e->dl);
        else cout << "ERROR: doubly list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "LDEL" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'L') dlist_del_value(e->dl, tok[2]);
        else cout << "ERROR: doubly list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "LDELBEFORE" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'L') dlist_del_before(e->dl, toInt(tok[2]));
        else cout << "ERROR: doubly list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "LDELAFTER" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'L') dlist_del_after(e->dl, toInt(tok[2]));
        else cout << "ERROR: doubly list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "LGET" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'L') dlist_get(e->dl, tok[2]);
        else cout << "ERROR: doubly list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "LGETAT" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'L') dlist_get_at(e->dl, toInt(tok[2]));
        else cout << "ERROR: doubly list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "LPRINTREV" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'L') dlist_print_reverse(e->dl);
        else cout << "ERROR: doubly list '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "LLEN" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'L') dlist_len(e->dl);
        else cout << "ERROR: doubly list '" << tok[1] << "' not found" << endl;
    }

    // ---- Стек ----
    else if (cmd == "SPUSH" && n >= 3) {
        Entry* e = registry_get_or_create(r, tok[1], 'S');
        if (e) stack_push(e->st, tok[2]);
    }
    else if (cmd == "SPOP" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'S') stack_pop(e->st);
        else cout << "ERROR: stack '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "SPEEK" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'S') stack_peek(e->st);
        else cout << "ERROR: stack '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "SLEN" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'S') stack_len(e->st);
        else cout << "ERROR: stack '" << tok[1] << "' not found" << endl;
    }

    // ---- Очередь ----
    else if (cmd == "QPUSH" && n >= 3) {
        Entry* e = registry_get_or_create(r, tok[1], 'Q');
        if (e) queue_push(e->q, tok[2]);
    }
    else if (cmd == "QPOP" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'Q') queue_pop(e->q);
        else cout << "ERROR: queue '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "QPEEK" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'Q') queue_peek(e->q);
        else cout << "ERROR: queue '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "QLEN" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'Q') queue_len(e->q);
        else cout << "ERROR: queue '" << tok[1] << "' not found" << endl;
    }

    // ---- Дерево (BST) ----
    else if (cmd == "TINSERT" && n >= 3) {
        Entry* e = registry_get_or_create(r, tok[1], 'T');
        if (e) tree_insert(e->tr, toInt(tok[2]));
    }
    else if (cmd == "TREMOVE" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'T') tree_remove(e->tr, toInt(tok[2]));
        else cout << "ERROR: tree '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "TGET" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'T') tree_get(e->tr, toInt(tok[2]));
        else cout << "ERROR: tree '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "ISMEMBER" && n >= 3) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'T') tree_is_member(e->tr, toInt(tok[2]));
        else cout << "ERROR: tree '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "TCOMPLETE" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'T') tree_is_complete(e->tr);
        else cout << "ERROR: tree '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "INORDER" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'T') tree_inorder(e->tr);
        else cout << "ERROR: tree '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "PREORDER" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'T') tree_preorder(e->tr);
        else cout << "ERROR: tree '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "POSTORDER" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'T') tree_postorder(e->tr);
        else cout << "ERROR: tree '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "LEVELORDER" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'T') tree_levelorder(e->tr);
        else cout << "ERROR: tree '" << tok[1] << "' not found" << endl;
    }
    else if (cmd == "TLEN" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (e && e->type == 'T') tree_len(e->tr);
        else cout << "ERROR: tree '" << tok[1] << "' not found" << endl;
    }

    // ---- Общее ----
    else if (cmd == "PRINT" && n >= 2) {
        Entry* e = registry_find(r, tok[1]);
        if (!e) { cout << "ERROR: '" << tok[1] << "' not found" << endl; return; }
        switch (e->type) {
            case 'M': array_print(e->arr); break;
            case 'F': flist_print(e->fl); break;
            case 'L': dlist_print(e->dl); break;
            case 'S': stack_print(e->st); break;
            case 'Q': queue_print(e->q); break;
            case 'T': tree_print(e->tr); break;
        }
    }
    else if (cmd == "HELP") printHelp();
    else cout << "ERROR: unknown command '" << cmd << "'" << endl;

    if (!filename.empty()) saveRegistry(filename, r);
}

// ===================== MAIN =====================

int main(int argc, char* argv[]) {
    Registry reg;
    registry_init(reg);

    string filename;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) filename = argv[++i];
    }

    if (!filename.empty()) loadRegistry(filename, reg);

    // Одиночный запрос
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--query" && i + 1 < argc) {
            processCommand(argv[++i], reg, filename);
            registry_free(reg);
            return 0;
        }
    }

    // Интерактивный режим
    cout << "=== Lab 1: DBMS-like (BST + named structures) ===" << endl;
    cout << "Type HELP for command list, EXIT to quit." << endl;

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;
        if (line.empty()) continue;

        string tok[16];
        split(line, tok, 16);
        if (tok[0] == "EXIT" || tok[0] == "exit") {
            if (!filename.empty()) saveRegistry(filename, reg);
            break;
        }
        processCommand(line, reg, filename);
    }

    registry_free(reg);
    return 0;
}