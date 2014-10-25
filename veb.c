#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef unsigned int bool;
#define true 1
#define false 0

struct veb_tree {
    unsigned int        u;
    int                 min, max;
    unsigned int        u_down, u_up;
    struct veb_tree     *summary;
    struct veb_tree     **subtree; 
};

//int __builtin_popcount (unsigned int x);
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

unsigned int High(unsigned int x, unsigned int u) {
    return (x / (unsigned int)sqrt(u));
}

unsigned int Low(unsigned int x, unsigned int u) {
    return (x % (unsigned int)sqrt(u));
}

unsigned int Index(unsigned int x, unsigned int y, unsigned int u) {
    return x * (unsigned int)sqrt(u) + y;
}

struct veb_tree * new_tree(unsigned int u) {
    struct veb_tree *ptr = (struct veb_tree *)malloc(sizeof(struct veb_tree));
    ptr->u = u;
    ptr->min = ptr->max = -1;
    if (u <= 2) {
        ptr->subtree = NULL;
        ptr->summary = NULL;
    }
    else {
        unsigned int i;
        ptr->u_down = (unsigned int)sqrt(u);
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
        unsigned int i;
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

bool find(struct veb_tree *root, unsigned int key) {
    if (key == root->min || key == root->max)
        return true;
    else if (root->u == 2)
        return false;
    return find(root->subtree[High(key, root->u)], Low(key, root->u));
}

void insert(struct veb_tree *root, unsigned int key) {
    if (root->min == -1) {
        root->min = root->max = key;
        return;
    }
    if (key < root->min) {
        int k = key; key = root->min; root->min = k;
    }
    if (root->u > 2) {
        struct veb_tree *next = root->subtree[High(key, root->u)];
        if (minimum(next) == -1) {
            insert(root->summary, High(key, root->u));
            next->max = next->min = Low(key, root->u);
        }
        else
            insert(next, Low(key, root->u));
    }
    if (key > root->max)
        root->max = key;
}

void delete(struct veb_tree *root, unsigned int key) {
    if (root->min == root->max) {
        root->min = root->max = -1;
        return;
    }
    if (root->u == 2) {
        root->max = root->min = (key == 0) ? 1 : 0;
        return;
    }
    if (key == root->min) {
        unsigned int next= minimum(root->summary);
        key = Index(next, minimum(root->subtree[next]), root->u);
        root->min = key;
    }
    unsigned int high = High(key, root->u);
    unsigned int low = Low(key, root->u);
    struct veb_tree *next = root->subtree[high];
    delete(next, low); 
    if (minimum(next) == -1) {
        delete(root->summary, high);
        if (key == root->max) {
            unsigned int summary_max = maximum(root->summary);
            if (summary_max == -1)
                root->max = root->min;
            else
                root->max = Index(summary_max, maximum(root->subtree[summary_max]), root->u);
        }
    }
    else if (key == root->max)
        root->max = Index(high, maximum(root->subtree[high]), root->u);
}

int successor(struct veb_tree *root, unsigned int key) {
    if (root->u == 2) {
        if (key == 0 && root->max == 1)
            return 1;
        return -1;
    }
    if (root->min != -1 && key < root->min)
        return root->min;
    int high = High(key, root->u);
    int low = Low(key, root->u);
    int max_low = maximum(root->subtree[high]);
    if (max_low != -1 && low < max_low) {
        int offset = successor(root->subtree[high], low);
        return Index(high, offset, root->u);
    }
    int next = successor(root->summary, high);
    if (next == -1)
        return -1;
    int offset = minimum(root->subtree[next]);
    return Index(next, offset, root->u);
}

int predecessor(struct veb_tree *root, unsigned int key) {
    if (root->u == 2) {
        if (key == 1 && root->min == 0)
            return 0;
        return -1;
    }
    if (root->max != -1 && key > root->max)
        return root->max;
    int high = High(key, root->u);
    int low = Low(key, root->u);
    int min_low = minimum(root->subtree[high]);
    if (min_low != -1 && low > min_low) {
        int offset = predecessor(root->subtree[high], low);
        return Index(high, offset, root->u);
    }
    int next = predecessor(root->summary, high);
    if (next == -1) {
        if (root->min != -1 && key > root->min)
            return root->min;
        return -1;
    }
    int offset = maximum(root->subtree[next]);
    return Index(next, offset, root->u);
}


struct veb_tree     *tree;

int main() {
    tree = new_tree(100);
    delete_tree(tree);
    return 0;
}

