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
    if (x == NULL)
        return NULL;
    while (x->left != NULL)
    {
        x = x->left;
    }
    return x;
}

void removeNode(TreeMap *tree, TreeNode *node)
{
    // Si el nodo no existe no ocurre nada
    if (node == NULL)
        return;

    // CASO 1: EL NODO NO TIENE HIJOS
    if (node->left == NULL && node->right == NULL)
    {
        // Si el padre es NULL estamos en la raiz
        if (node->parent == NULL)
        {
            tree->root = NULL;
        }
        // Si no estamos en la raiz tenemos que identificar si es hijo izq. o der.
        // El nodo actual (node) es el hijo izquierdo de su padre (parent)?
        else if (node->parent->left == node)
        {
            node->parent->left = NULL;
        }
        // Aqui estamos cuando el nodo sea el hijo derecho
        else
        {
            node->parent->right = NULL;
        }
        free(node->pair);
        free(node);
    }
    // CASO 2: EL NODO TIENE UN SOLO HIJO
    else if ((node->left == NULL && node->right != NULL) || (node->left != NULL && node->right == NULL))
    {
        TreeNode *child = (node->left != NULL) ? node->left : node->right;

        // Si estamos en la raiz
        if (node->parent == node)
        {
            tree->root = child;
            child->parent = NULL;
            // la raiz del arbol ahora sera el hijo que encontramos
            // y ahora a este child su padre lo dejamos en NULL, porque es la nueva raizzz
        }
        else if (node->parent->left == node)
        {
            node->parent->left = child;
            child->parent = node->parent;
        }
        else
        {
            node->parent->right = child;
            child->parent = node->parent;
        }
        free(node->pair);
        free(node);
    }

    // CASO 3: NODO CON DOS HIJOS
    else
    {
        TreeNode *min = minimum(node->right); // Buscamos el sucesor inmediato!!!

        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min);
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
    if (tree->root == NULL)
    {
        return NULL;
    }

    Pair *aux = searchTreeMap(tree, key);
    if (aux != NULL)
    {
        return aux;
    }

    TreeNode *current = tree->root;
    TreeNode *candidate = NULL;

    while (current != NULL)
    {
        if (!tree->lower_than(current->pair->key, key))
        {
            candidate = current;
            current = current->left;
        }
        else
        {

            current = current->right;
        }
    }

    if (candidate != NULL)
    {
        tree->current = candidate;
        return candidate->pair;
    }

    return NULL;
}

Pair *firstTreeMap(TreeMap *tree)
{
    if (tree->root == NULL)
    {
        return NULL;
    }
    TreeNode *min = minimum(tree->root);
    tree->current = min;
    return tree->current->pair;
}

Pair *nextTreeMap(TreeMap *tree)
{
    if (tree->current->right != NULL)
    {
        TreeNode *next = minimum(tree->current->right);
        tree->current = next;
        return next->pair;
    }
    else
    {

        TreeNode *aux = tree->current;
        while (aux->parent != NULL && aux == aux->parent->right)
        {
            aux = aux->parent;
        }

        if (aux->parent == NULL)
            return NULL;

        tree->current = aux->parent;
        return tree->current->pair;
    }
}