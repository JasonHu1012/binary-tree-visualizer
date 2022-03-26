#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    red,
    black
}color_type;

typedef struct rbtree_node
{
    int key;
    color_type color;
    struct rbtree_node *parent;
    struct rbtree_node *left;
    struct rbtree_node *right;
}rbtree_node;

typedef struct rbtree
{
    rbtree_node *root;
    rbtree_node *nil;
}rbtree;

rbtree *new_rbtree()
{
    rbtree *output = (rbtree *)malloc(sizeof(rbtree));
    output->nil = (rbtree_node *)malloc(sizeof(rbtree_node));
    output->nil->color = black;
    output->root = output->nil;
    return output;
}

rbtree_node *new_rbtree_node(int n)
{
    rbtree_node *output = (rbtree_node *)malloc(sizeof(rbtree_node));
    output->key = n;
    output->color = red;
    return output;
}

static void left_rotate(rbtree *tree, rbtree_node *node)
{
    rbtree_node *child = node->right;
    node->right = child->left;
    if(child->left != tree->nil)
        child->left->parent = node;
    child->parent = node->parent;
    (node->parent == tree->nil) ? (tree->root = child) : ((node == node->parent->left) ? (node->parent->left = child) : (node->parent->right = child));
    /*
    if(node->parent == &nil)
        *root = child;
    else if(node == node->parent->left)
        node->parent->left = child;
    else
        node->parent->right = child;
    */
    child->left = node;
    node->parent = child;
}

static void right_rotate(rbtree *tree, rbtree_node *node)
{
    rbtree_node *child = node->left;
    node->left = child->right;
    if(child->right != tree->nil)
        child->right->parent = node;
    child->parent = node->parent;
    (node->parent == tree->nil) ? (tree->root = child) : ((node == node->parent->left) ? (node->parent->left = child) : (node->parent->right = child));
    /*
    if(node->parent == &nil)
        *root = child;
    else if(node == node->parent->left)
        node->parent->left = child;
    else
        node->parent->right = child;
    */
    child->right = node;
    node->parent = child;
}

void rbtree_insert(rbtree *tree, rbtree_node *node)
{
    node->parent = tree->nil;
    node->left = tree->nil;
    node->right = tree->nil;
    if(tree->root == tree->nil)
    {
        node->color = black;
        tree->root = node;
        return;
    }
    rbtree_node *traverse = tree->root;
    while(1)
    {
        if(traverse->key <= node->key)
        {
            if(traverse->right != tree->nil)
                traverse = traverse->right;
            else
            {
                traverse->right = node;
                break;
            }
        }
        else
        {
            if(traverse->left != tree->nil)
                traverse = traverse->left;
            else
            {
                traverse->left = node;
                break;
            }
        }
    }
    node->parent = traverse;
    rbtree_node *in_law;
    while(node->parent->color == red)
    {
        if(node->parent == node->parent->parent->left)
        {
            in_law = node->parent->parent->right;
            if(in_law->color == red)
            {
                node->parent->color = black;
                in_law->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            }
            else
            {
                if(node == node->parent->right)
                {
                    node = node->parent;
                    left_rotate(tree, node);
                }
                node->parent->color = black;
                node->parent->parent->color = red;
                right_rotate(tree, node->parent->parent);
            }
        }
        else
        {
            in_law = node->parent->parent->left;
            if(in_law->color == red)
            {
                node->parent->color = black;
                in_law->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            }
            else
            {
                if(node == node->parent->left)
                {
                    node = node->parent;
                    right_rotate(tree, node);
                }
                node->parent->color = black;
                node->parent->parent->color = red;
                left_rotate(tree, node->parent->parent);
            }
        }
    }
    tree->root->color = black;
}

rbtree *t;

void in(rbtree_node *root)
{
    if(root == t->nil)
        return;
    in(root->left);
    printf("%d ", root->key);
    in(root->right);
}

void pre(rbtree_node *root)
{
    if(root == t->nil)
        return;
    printf("%d ", root->key);
    pre(root->left);
    pre(root->right);
}

int main()
{
    srand(time(NULL));
    int i, j, k, n, number_of_nodes;
    int arr[1000];
    int size;
    for(j = 0; j < 10; j++)
    {
        t = new_rbtree();
        size = 0;
        number_of_nodes = rand() % 10 + 15;
        for(i = 0; i < number_of_nodes; i++)
        {
            do
            {
                n = rand() % 100000 - 50000;
                for(k = 0; k < size; k++)
                {
                    if(arr[k] == n)
                        break;
                }
            }while(k != size);
            arr[size++] = n;
            rbtree_insert(t, new_rbtree_node(n));
        }
        in(t->root);
        printf("\n");
        pre(t->root);
        printf("\n");
    }
    printf("e");
    return 0;
}
