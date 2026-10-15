#include "tree.h"

void tree_init(CompleteBinaryTree& t) {
    t.root = 0;
    t.count = 0;
}

// Освобождение памяти рекурсивно
static void free_node(TNode* n) {
    if (!n) return;
    free_node(n->left);
    free_node(n->right);
    delete n;
}

void tree_free(CompleteBinaryTree& t) {
    free_node(t.root);
    t.root = 0;
    t.count = 0;
}

// ===================== ВСТАВКА (BST) =====================
static TNode* insert_rec(TNode* node, int value, bool& added) {
    if (!node) {
        TNode* n = new TNode(value);
        added = true;
        return n;
    }
    if (value < node->value) {
        node->left = insert_rec(node->left, value, added);
    } else if (value > node->value) {
        node->right = insert_rec(node->right, value, added);
    }
    // Равные значения игнорируем (в BST дубликаты не хранят)
    return node;
}

void tree_insert(CompleteBinaryTree& t, int value) {
    bool added = false;
    t.root = insert_rec(t.root, value, added);
    if (added) {
        ++t.count;
        cout << "OK" << endl;
    } else {
        cout << "ERROR: duplicate value" << endl;
    }
}

// ===================== ПОИСК =====================
static TNode* find_min(TNode* node) {
    while (node && node->left) node = node->left;
    return node;
}

void tree_get(const CompleteBinaryTree& t, int value) {
    TNode* cur = t.root;
    int depth = 0;
    while (cur) {
        if (value == cur->value) {
            cout << "FOUND (depth " << depth << ")" << endl;
            return;
        }
        cur = (value < cur->value) ? cur->left : cur->right;
        ++depth;
    }
    cout << "NOT FOUND" << endl;
}

void tree_is_member(const CompleteBinaryTree& t, int value) {
    TNode* cur = t.root;
    while (cur) {
        if (value == cur->value) { cout << "TRUE" << endl; return; }
        cur = (value < cur->value) ? cur->left : cur->right;
    }
    cout << "FALSE" << endl;
}

// ===================== УДАЛЕНИЕ (BST) =====================
static TNode* remove_rec(TNode* node, int value, bool& removed) {
    if (!node) return 0;
    if (value < node->value) {
        node->left = remove_rec(node->left, value, removed);
    } else if (value > node->value) {
        node->right = remove_rec(node->right, value, removed);
    } else {
        removed = true;
        // Случай 1 и 2: 0 или 1 потомок
        if (!node->left) {
            TNode* r = node->right;
            delete node;
            return r;
        }
        if (!node->right) {
            TNode* l = node->left;
            delete node;
            return l;
        }
        // Случай 3: два потомка — заменяем минимумом из правого поддерева
        TNode* mn = find_min(node->right);
        node->value = mn->value;
        bool tmp = false;
        node->right = remove_rec(node->right, mn->value, tmp);
    }
    return node;
}

void tree_remove(CompleteBinaryTree& t, int value) {
    bool removed = false;
    t.root = remove_rec(t.root, value, removed);
    if (removed) {
        --t.count;
        cout << "OK" << endl;
    } else {
        cout << "NOT FOUND" << endl;
    }
}

// ===================== ПРОВЕРКА НА COMPLETE =====================
// Complete Binary Tree: все уровни, кроме последнего, заполнены полностью,
// последний — слева направо без пропусков.
//
// Алгоритм: BFS с флагом "уже видели узел без одного/двух потомков".
// Как только встретили узел без правого или без обоих потомков — все
// следующие узлы должны быть листьями.
struct QItem {
    TNode* node;
    QItem* next;
    QItem(TNode* n) : node(n), next(0) {}
};

void tree_is_complete(const CompleteBinaryTree& t) {
    if (!t.root) { cout << "TRUE (empty tree)" << endl; return; }

    // Простая очередь на указателях
    QItem* head = new QItem(t.root);
    QItem* tail = head;

    bool seenGap = false;      // видели узел с неполным набором потомков
    bool ok = true;

    while (head && ok) {
        QItem* cur = head;
        head = head->next;
        if (!head) tail = 0;

        TNode* n = cur->node;
        delete cur;

        // Проверка левого потомка
        if (n->left) {
            if (seenGap) { ok = false; break; }
            QItem* item = new QItem(n->left);
            if (!tail) { head = tail = item; }
            else { tail->next = item; tail = item; }
        } else {
            seenGap = true;
        }

        // Проверка правого потомка
        if (n->right) {
            if (seenGap) { ok = false; break; }
            QItem* item = new QItem(n->right);
            if (!tail) { head = tail = item; }
            else { tail->next = item; tail = item; }
        } else {
            seenGap = true;
        }
    }

    // Очистка очереди, если вышли раньше
    while (head) {
        QItem* nxt = head->next;
        delete head;
        head = nxt;
    }

    cout << (ok ? "TRUE" : "FALSE") << endl;
}

// ===================== ОБХОДЫ =====================
static void inorder_rec(TNode* n) {
    if (!n) return;
    inorder_rec(n->left);
    cout << n->value << " ";
    inorder_rec(n->right);
}

static void preorder_rec(TNode* n) {
    if (!n) return;
    cout << n->value << " ";
    preorder_rec(n->left);
    preorder_rec(n->right);
}

static void postorder_rec(TNode* n) {
    if (!n) return;
    postorder_rec(n->left);
    postorder_rec(n->right);
    cout << n->value << " ";
}

void tree_inorder(const CompleteBinaryTree& t) { inorder_rec(t.root); cout << endl; }
void tree_preorder(const CompleteBinaryTree& t) { preorder_rec(t.root); cout << endl; }
void tree_postorder(const CompleteBinaryTree& t) { postorder_rec(t.root); cout << endl; }

void tree_levelorder(const CompleteBinaryTree& t) {
    if (!t.root) { cout << endl; return; }
    QItem* head = new QItem(t.root);
    QItem* tail = head;
    while (head) {
        QItem* cur = head;
        head = head->next;
        if (!head) tail = 0;
        TNode* n = cur->node;
        delete cur;
        cout << n->value << " ";
        if (n->left) {
            QItem* item = new QItem(n->left);
            if (!tail) { head = tail = item; }
            else { tail->next = item; tail = item; }
        }
        if (n->right) {
            QItem* item = new QItem(n->right);
            if (!tail) { head = tail = item; }
            else { tail->next = item; tail = item; }
        }
    }
    cout << endl;
}

void tree_len(const CompleteBinaryTree& t) { cout << t.count << endl; }

// ===================== PRINT =====================
// Печатаем массив + уровни (для отображения формы)
static void collect_levels(TNode* n, int depth, int level, string out[], int maxLevel) {
    if (!n || depth > maxLevel) return;
    if (out[depth].empty()) out[depth] = "";
    // добавляем значения слева-направо
    collect_levels(n->left, depth + 1, level, out, maxLevel);
}

// Простой вывод: in-order + список
void tree_print(const CompleteBinaryTree& t) {
    if (!t.root) { cout << "(empty tree)" << endl; return; }

    cout << "Inorder (sorted): ";
    inorder_rec(t.root);
    cout << endl;

    cout << "Preorder: ";
    preorder_rec(t.root);
    cout << endl;

    cout << "Level-order (BFS): ";
    tree_levelorder(t);

    cout << "Count: " << t.count << endl;
}

// ===================== ФАЙЛ =====================
// Формат: в одну строку in-order (отсортированные значения через запятую)
void tree_save(const CompleteBinaryTree& t, ostream& os) {
    // Собираем значения в in-order
    // (для BST — это отсортированный порядок, при загрузке вставим в том же порядке)
    os << t.count << "\n";
    // in-order через стек
    // Проще: сохранить в preorder, при загрузке вставлять в том же порядке — дерево
    // восстановится не идентично, но останется корректным BST.
    // Сохраним в pre-order, чтобы восстановить исходную форму.
    // Стек для preorder без рекурсии:
    TNode* stack[1024];
    int sp = 0;
    if (t.root) stack[sp++] = t.root;
    while (sp > 0) {
        TNode* n = stack[--sp];
        os << n->value << "\n";
        if (n->right) stack[sp++] = n->right;
        if (n->left) stack[sp++] = n->left;
    }
}

void tree_load(CompleteBinaryTree& t, istream& is) {
    tree_free(t);
    int n; is >> n;
    for (int i = 0; i < n; ++i) {
        int v; is >> v;
        bool added = false;
        t.root = insert_rec(t.root, v, added);
        if (added) ++t.count;
    }
}