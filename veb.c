#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct veb_tree {
    unsigned int        u;
    unsigned int        min, max;
    unsigned int        u_down, u_up;
    struct veb_tree     *summary;
    struct veb_tree     **subtree; 
};

/*
unsigned int count_bit(unsigned int k) {
    unsigned int c = 0;
    for (c = 0; k; ++ c)
        k &= k - 1;
    return c;
}
*/

/*
unsigned int count_bit(unsigned int k) {
    k = k - ((k >> 1) & 0x55555555);
    k = (k & 0x33333333) + ((k >> 2) & 0x33333333);
    return (((k + (k >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}
*/

unsigned int high(unsigned int x, unsigned int u) {
    return (x / (int)sqrt(u));
}

unsigned int low(unsigned int x, unsigned int u) {
    return (x % (int)sqrt(u));
}

struct veb_tree * new_tree(int u) {
    struct veb_tree *ptr = (struct veb_tree *)malloc(sizeof(struct veb_tree));
    ptr->u = u;
    ptr->min = ptr->max = -1;
    if (u <= 2) {
        ptr->subtree = NULL;
        ptr->summary = NULL;
    }
    else {
        int i;
        ptr->u_down = (int)sqrt(u);
        ptr->u_up = u - ptr->u_down;
        ptr->summary = new_tree(ptr->u_up);
        ptr->subtree = (struct veb_tree **)malloc(ptr->u_up * sizeof(struct veb_tree *));
        for (i = 0; i < ptr->u_up; ++ i)
            ptr->subtree[i] = new_tree(ptr->u_down);
    }
    return ptr;
}

void delete_tree(struct veb_tree *root) {
    if (root->summary != NULL)
        delete_tree(root->summary);
    if (root->subtree != NULL) {
        int i;
        for (i = 0; i < root->u_up; ++ i)
            delete_tree(root->subtree[i]);
    }
    free(root);
}

int minimum(struct veb_tree *root) {
    return root->min;
}

int maximum(struct veb_tree *root) {
    return root->max;
}


struct veb_tree     *tree;

int main() {
    tree = new_tree(100);
    delete_tree(tree);
    return 0;
}

