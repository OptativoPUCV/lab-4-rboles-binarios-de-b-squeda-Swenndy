#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode
{
    Pair *pair;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
};

struct TreeMap
{
    TreeNode *root;
    TreeNode *current;
    int (*lower_than)(void *key1, void *key2);
};

int is_equal(TreeMap *tree, void *key1, void *key2)
{
    if (tree->lower_than(key1, key2) == 0 &&
        tree->lower_than(key2, key1) == 0)
        return 1;
    else
        return 0;
}

TreeNode *createTreeNode(void *key, void *value)
{
    TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL)
        return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap *createTreeMap(int (*lower_than)(void *key1, void *key2))
{
    TreeMap *new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL)
    {
        return NULL;
    }
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;

    return new;
}

void insertTreeMap(TreeMap *tree, void *key, void *value)
{
    if (tree->root == NULL)
    {
        TreeNode *newNode = createTreeNode(key, value);
        tree->root = newNode;
        return;
    }

    TreeNode *aux = tree->root;
    while (aux != NULL)
    {
        if (is_equal(tree, aux->pair->key, key))
        {
            return;
        }
        if (tree->lower_than(key, aux->pair->key))
        {
            if (aux->left == NULL)
            {
                TreeNode *newNode = createTreeNode(key, value);
                aux->left = newNode;
                newNode->parent = aux;
                tree->current = newNode;
                return;
            }
            else
            {
                aux = aux->left;
            }
        }
        else
        {
            if (aux->right == NULL)
            {
                TreeNode *newNode = createTreeNode(key, value);
                aux->right = newNode;
                newNode->parent = aux;
                tree->current = newNode;
                return;
            }
            else
            {
                aux = aux->right;
            }
        }
    }
}

TreeNode *minimum(TreeNode *x)
{

    return NULL;
}

void removeNode(TreeMap *tree, TreeNode *node)
{
    return NULL;
}

void eraseTreeMap(TreeMap *tree, void *key)
{
    if (tree == NULL || tree->root == NULL)
        return;

    if (searchTreeMap(tree, key) == NULL)
        return;
    TreeNode *node = tree->current;
    removeNode(tree, node);
}

Pair *searchTreeMap(TreeMap *tree, void *key)
{
    TreeNode *aux = tree->root;

    while (aux != NULL)
    {
        if (is_equal(tree, aux->pair->key, key))
        {
            tree->current = aux;
            return aux->pair;
        }
        if (tree->lower_than(key, aux->pair->key))
        {
            aux = aux->left;
        }
        else
        {
            aux = aux->right;
        }
    }
    return NULL;
}

Pair *upperBound(TreeMap *tree, void *key)
{
    return NULL;
}

Pair *firstTreeMap(TreeMap *tree)
{
    return NULL;
}

Pair *nextTreeMap(TreeMap *tree)
{
    return NULL;
}
