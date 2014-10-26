#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define High(x, bits)       (x >> (bits >> 1))
#define Low(x, bits)        (x & ((1 << (bits >> 1)) - 1))
#define Index(x, y, bits)   ((x << (bits >> 1)) + y)

struct veb_tree {
    unsigned int        u;
    int                 min, max;
    struct veb_tree     *summary;
    struct veb_tree     **subtree; 
};

struct veb_tree * build(unsigned int u) {
    struct veb_tree *ptr = (struct veb_tree *)malloc(sizeof(struct veb_tree));
    ptr->u = u;
    ptr->min = ptr->max = -1;
    if (u == 0)
        fprintf(stderr, "fuck!!!!!\n");
    if (u == 1) {
        ptr->subtree = NULL;
        ptr->summary = NULL;
    }
    else {
        unsigned int i;
        unsigned int u_down = u >> 1;
        unsigned int u_up = u - u_down;
        ptr->summary = build(u_up);
        ptr->subtree = (struct veb_tree **)malloc((1 << u_up) * sizeof(struct veb_tree *));
        for (i = 0; i < (1 << u_up); ++ i)
            ptr->subtree[i] = build(u_down);
    }
    return ptr;
}

void destroy(struct veb_tree *root) {
    if (root->summary != NULL)
        destroy(root->summary);
    if (root->subtree != NULL) {
        unsigned int i;
        unsigned int u_up = 1 << (root->u - (root->u >> 1));
        for (i = 0; i < u_up; ++ i)
            destroy(root->subtree[i]);
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
    else if (root->u == 1)
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
    if (root->u > 1) {
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
    if (root->u == 1) {
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
    if (root->u == 1) {
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
    if (root->u == 1) {
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
    int u;
    scanf("%d", &u);

    
    int U = 0;
    int temp;
    for (temp = 1; temp <= u; temp <<= 1, ++ U);

    int n;
    scanf("%d", &n);
    tree = build(U);

    while (n --) {
        int k, x;
        scanf("%d", &k);
        //printf("%d -->\n", k);
        if (k == 0) {
            scanf("%d", &x);
            if (!find(tree, x))
                insert(tree, x);
        }
        else if (k == 1) {
            scanf("%d", &x);
            if (find(tree, x))
                delete(tree, x);
        }
        else if (k == 2) {
            scanf("%d", &x);
            printf("%d\n", find(tree, x) ? 1 : 0);
        }
        else if (k == 3) {
            scanf("%d", &x);
            printf("%d\n", predecessor(tree, x));
        }
        else if (k == 4) {
            scanf("%d", &x);
            printf("%d\n", successor(tree, x));
        }
        else if (k == 5) {
            printf("%d\n", maximum(tree));
        }
        else {
            printf("%d\n", minimum(tree));
        }
    }

    destroy(tree);
    return 0;
}

