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
    TreeMap *mapa = (TreeMap *)malloc(sizeof(TreeMap));
    if (mapa == NULL)
        return NULL;

    mapa->root = NULL;
    mapa->current = NULL;
    mapa->lower_than = lower_than;
    return mapa;
}

void insertTreeMap(TreeMap *tree, void *key, void *value)
{
    if (tree == NULL)
        return;
    if (searchTreeMap(tree, key) != NULL)
        return;

    TreeNode *padre = NULL;
    TreeNode *auxiliar = tree->root;

    while (auxiliar != NULL)
    {
        padre = auxiliar;
        if (tree->lower_than(key, auxiliar->pair->key))
            auxiliar = auxiliar->left;
        else
            auxiliar = auxiliar->right;
    }

    TreeNode *newNode = createTreeNode(key, value);
    if (newNode == NULL)
        return;
    newNode->parent = padre;
    if (padre == NULL)
        tree->root = newNode;

    else if (tree->lower_than(key, padre->pair->key))
        padre->left = newNode;

    else
        padre->right = newNode;

    tree->current = newNode;
    return;
}

TreeNode *minimum(TreeNode *x)
{
    if (x == NULL)
        return NULL;
    while (x->left != NULL)
        x = x->left;
    return x;
}

void removeNode(TreeMap *tree, TreeNode *node)
{
    if (tree == NULL || node == NULL)
        return;

    TreeNode *padre = node->parent;
    if (node->left == NULL && node->right == NULL)
    {
        if (padre == NULL)
            tree->root = NULL;
        else if (padre->left == node)
            padre->left = NULL;
        else
            padre->right = NULL;

        free(node->pair);
        free(node);
    }
    else if (node->left == NULL || node->right == NULL)
    {
        TreeNode *hijo = (node->left != NULL) ? node->left : node->right;
        hijo->parent = padre;

        if (padre == NULL)
            tree->root = hijo;
        else if (padre->left == node)
            padre->left = hijo;
        else
            padre->right = hijo;
    }
    else
    {
        TreeNode *siguiente = minimum(node->right);
        free(node->pair);
        node->pair = siguiente->pair;
        removeNode(tree, siguiente);
    }
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
    if (tree == NULL || tree->root == NULL)
        return NULL;
    TreeNode *aux = tree->root;

    while (aux != NULL)
    {
        if (is_equal(tree, key, aux->pair->key))
        {
            tree->current = aux;
            return aux->pair;
        }

        if (tree->lower_than(key, aux->pair->key))
            aux = aux->left;

        else
            aux = aux->right;
    }

    tree->current = NULL;
    return NULL;
}

Pair *upperBound(TreeMap *tree, void *key)
{
    if (tree == NULL || tree->root == NULL)
        return NULL;

    TreeNode *auxiliar = tree->root;
    TreeNode *nodo = NULL;

    while (auxiliar != NULL)
    {
        if (tree->lower_than(key, auxiliar->pair->key))
        {
            nodo = auxiliar;
            auxiliar = auxiliar->left;
        }
        else if (tree->lower_than(auxiliar->pair->key, key))
            auxiliar = auxiliar->right;
        else
            return auxiliar->pair;
    }
    return (nodo != NULL) ? nodo->pair : NULL;
}

Pair *firstTreeMap(TreeMap *tree)
{
    if (tree == NULL || tree->root == NULL)
        return NULL;
    TreeNode *minimo = minimum(tree->root);
    tree->current = minimo;
    return (minimo != NULL) ? minimo->pair : NULL;
}

Pair *nextTreeMap(TreeMap *tree)
{
    if (tree == NULL || tree->current == NULL)
        return NULL;
    TreeNode *auxiliar = tree->current;
    TreeNode *siguiente = NULL;

    if (auxiliar->right != NULL)
        siguiente = minimum(auxiliar->right);
    else
    {
        TreeNode *padre = auxiliar->parent;
        while (padre != NULL && auxiliar == padre->right)
        {
            auxiliar = padre;
            padre = padre->parent;
        }
        siguiente = padre;
    }
    tree->current = siguiente;
    return (siguiente != NULL) ? siguiente->pair : NULL;
}