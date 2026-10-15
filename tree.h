#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct TNode {
    int value;
    TNode* left;
    TNode* right;
    TNode(int v) : value(v), left(0), right(0) {}
};

struct CompleteBinaryTree {
    TNode* root;
    int count;
};

void tree_init(CompleteBinaryTree& t);
void tree_free(CompleteBinaryTree& t);

void tree_insert(CompleteBinaryTree& t, int value);
void tree_remove(CompleteBinaryTree& t, int value);   // удаление с сохранением BST
void tree_get(const CompleteBinaryTree& t, int value);
void tree_is_member(const CompleteBinaryTree& t, int value);
void tree_is_complete(const CompleteBinaryTree& t);
void tree_inorder(const CompleteBinaryTree& t);
void tree_preorder(const CompleteBinaryTree& t);
void tree_postorder(const CompleteBinaryTree& t);
void tree_levelorder(const CompleteBinaryTree& t);
void tree_len(const CompleteBinaryTree& t);
void tree_print(const CompleteBinaryTree& t);

void tree_save(const CompleteBinaryTree& t, ostream& os);
void tree_load(CompleteBinaryTree& t, istream& is);

#endif