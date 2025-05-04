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
    return tree->lower_than(key1, key2) == 0 &&
           tree->lower_than(key2, key1) == 0;
}

TreeNode *createTreeNode(void *key, void *value)
{
    TreeNode *new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL)
        return NULL;

    new->pair = (Pair *)malloc(sizeof(Pair));
    if (new->pair == NULL)
    {
        free(new);
        return NULL;
    }

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

void insertTreeMap(TreeMap *tree, void *key, void *value)
{
    if (tree == NULL)
        return;

    if (searchTreeMap(tree, key) != NULL)
        return;

    TreeNode *padre = NULL;
    TreeNode *aux = tree->root;

    while (aux != NULL)
    {
        padre = aux;
        if (tree->lower_than(key, aux->pair->key))
            aux = aux->left;
        else
            aux = aux->right;
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

    // Caso 1: nodo sin hijos
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
    // Caso 2: nodo con un solo hijo
    else if ((node->left == NULL) != (node->right == NULL)) // solo uno no es NULL
    {
        TreeNode *hijo = (node->left != NULL) ? node->left : node->right;

        if (hijo != NULL)
            hijo->parent = padre;

        if (padre == NULL)
            tree->root = hijo;
        else if (padre->left == node)
            padre->left = hijo;
        else
            padre->right = hijo;

        free(node->pair);
        free(node);
    }
    // Caso 3: nodo con dos hijos
    else
    {
        TreeNode *siguiente = minimum(node->right);
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

    removeNode(tree, tree->current);
}

Pair *upperBound(TreeMap *tree, void *key)
{
    if (tree == NULL || tree->root == NULL)
        return NULL;

    TreeNode *aux = tree->root;
    TreeNode *nodo = NULL;

    while (aux != NULL)
    {
        if (tree->lower_than(key, aux->pair->key))
        {
            nodo = aux;
            aux = aux->left;
        }
        else if (tree->lower_than(aux->pair->key, key))
        {
            aux = aux->right;
        }
        else
        {
            return aux->pair;
        }
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

    TreeNode *aux = tree->current;
    TreeNode *siguiente = NULL;

    if (aux->right != NULL)
    {
        siguiente = minimum(aux->right);
    }
    else
    {
        TreeNode *padre = aux->parent;
        while (padre != NULL && aux == padre->right)
        {
            aux = padre;
            padre = padre->parent;
        }
        siguiente = padre;
    }

    tree->current = siguiente;
    return (siguiente != NULL) ? siguiente->pair : NULL;
}