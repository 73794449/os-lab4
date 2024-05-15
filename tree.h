#include <stddef.h>
#include <stdbool.h>
#ifndef TREE_H
#define TREE_H

#define left node1
#define right node2
#define left_of_right node2->node1
#define left_of_left node1->node1
#define right_of_left node1->node2
#define right_of_right node2->node2

#define malloc_struct(x, k) (x) malloc(sizeof(x) * k)

#define fill_data(data_dest, data_source)        \
    data_dest = malloc_struct(struct Data *, 1); \
    data_dest->S = data_source.S;                \
    data_dest->N1 = data_source.N1;              \
    data_dest->N2 = data_source.N2

#define fill_data_link(data_dest, data_source)   \
    data_dest = malloc_struct(struct Data *, 1); \
    data_dest->S = data_source->S;               \
    data_dest->N1 = data_source->N1;             \
    data_dest->N2 = data_source->N2

#define swap(x, y)              \
    do                          \
    {                           \
        void *temp = (void *)x; \
        x = y;                  \
        y = (typeof(y))temp;    \
    } while (0)

/*CMP*/
enum Level
{
    BIGGER,
    LOWER,
    SAME
};

struct Data
{
    char S;
    char N1;
    char N2;
};

struct SearchData
{
    struct Data **values;
    size_t count;
};

struct BinaryTree
{
    struct BinaryTree *node1;
    struct BinaryTree *node2;
    struct Data *data;
    size_t height;
};

struct BinaryTree *create(struct Data data);
void free_tree(struct BinaryTree *node);
struct BinaryTree *add(struct BinaryTree *node, struct Data data);
struct BinaryTree *rm(struct BinaryTree *node, struct Data data);
struct SearchData *find(struct BinaryTree *node, struct Data data);
void update_height(struct BinaryTree *node);
struct BinaryTree *rebalance(struct BinaryTree *node);
struct BinaryTree *rotate_right(struct BinaryTree *node);
struct BinaryTree *rotate_left(struct BinaryTree *node);
size_t get_height(struct BinaryTree *node);
enum Level compare(struct Data *data1, struct Data *data2);
struct BinaryTree *children_left(struct BinaryTree *node);
struct BinaryTree *children_right(struct BinaryTree *node);
signed short int calculate_delta(struct BinaryTree *node);
void output_tree(struct BinaryTree *node, size_t depth, bool alone, size_t first, size_t i_am_right);
void output_search(struct SearchData *data);

#endif