#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

struct BinaryTree *create(struct Data data) {
  struct BinaryTree *node = malloc_struct(struct BinaryTree *, 1);
  fill_data(node->data, data);
  node->node1 = NULL;
  node->node2 = NULL;
  node->height = 1;
}

struct BinaryTree *add(struct BinaryTree *node, struct Data data) {
  if (node == NULL) {
    node = create(data);
    return node;
  }

  if (compare(&data, node->data) ==
      LOWER) // When is LOWER = 1 reference /*CMP*/
    node->left = add(node->left, data);
  else if (compare(&data, node->data) == BIGGER)
    node->right = add(node->right, data);
  else {
    printf("Error: same identifier already in this directory");
    return node;
  }

  update_height(node);
  return rebalance(node);
}

void update_height(struct BinaryTree *node) {
  if (node->left > node->right)
    node->height = node->left->height + 1;
  else if (node->right != NULL)
    node->height = node->right->height + 1;
}

size_t get_height(struct BinaryTree *node) {
  if (node == NULL)
    return 0;

  return node->height;
}

struct BinaryTree *rebalance(struct BinaryTree *node) {
  signed short int delta = calculate_delta(node);
  if (delta > 1 && compare(node->data, node->left->data) == BIGGER)
    return rotate_right(node);

  if (delta < -1 && compare(node->data, node->right->data) == LOWER)
    return rotate_left(node);

  if (delta > 1 && compare(node->data, node->left->data) == LOWER) {
    node->left = rotate_left(node->left);
    return rotate_right(node);
  }

  if (delta < -1 && compare(node->data, node->right->data) == BIGGER) {
    node->right = rotate_right(node->right);
    return rotate_left(node);
  }

  return node;
}

struct BinaryTree *rotate_left(struct BinaryTree *node) {
  struct BinaryTree *y = node->right;
  struct BinaryTree *T2 = y->left;

  y->left = node;
  node->right = T2;

  update_height(node);
  update_height(y);
  return y;
}

struct BinaryTree *rotate_right(struct BinaryTree *node) {
  struct BinaryTree *x = node->left;
  struct BinaryTree *T2 = node->right;

  x->right = node;
  node->left = T2;

  update_height(node);
  update_height(x);
  return x;
}

void free_tree(struct BinaryTree *node) {
  if (get_height(node) == 1) {
    node->height = 0;
    free(node);
    free(node->data);
  } else {
    if (node->node1 != NULL)
      free_tree(node->node1);
    if (node->node2 != NULL)
      free_tree(node->node2);
    node->height = 0;
    free(node);
    free(node->data);
  }
}

struct SearchData *find(struct BinaryTree *node, struct Data data) {
  struct Data **path = malloc_struct(struct Data **, 1);
  struct SearchData *search = malloc_struct(struct SearchData *, 1);

  // if node null no search value
  if (node == NULL) {
    search->values = NULL;
    search->count = 0;
    return search;
  }

  // if node->id == id
  if (compare(&data, node->data) == SAME) {
    fill_data(path[0], data);
    search->values = path;
    search->count = 1;
    return search;
  }

  // if node->id != id
  fill_data_link(path[0], node->data);
  search->count = 1;

  // also add depth ids
  struct SearchData *under_path;
  if (compare(&data, node->data)) // When is LOWER = 1 reference /*CMP*/
    under_path = find(node->left, data);
  else
    under_path = find(node->right, data);

  for (size_t i = 0; i < under_path->count; i++) {
    fill_data_link(path[search->count], under_path->values[i]);
    search->count++;
  }

  search->values = path;
  return search;
}

enum Level compare(struct Data *data1, struct Data *data2) {
  if (data1->S < data2->S)
    return LOWER;
  else if (data1->S > data2->S)
    return BIGGER;
  else if (data1->N1 < data2->N1)
    return LOWER;
  else if (data1->N1 > data2->N1)
    return BIGGER;
  else if (data1->N2 < data2->N2)
    return LOWER;
  else if (data1->N2 > data2->N2)
    return BIGGER;
  else
    return SAME;
}

struct BinaryTree *children_left(struct BinaryTree *node) {
  if (node == NULL)
    return NULL;

  while (node->left != NULL)
    node = node->left;

  return node;
}

struct BinaryTree *children_right(struct BinaryTree *node) {
  if (node == NULL)
    return NULL;

  while (node->right != NULL)
    node = node->right;

  return node;
}

signed short int calculate_delta(struct BinaryTree *node) {
  if (node == NULL)
    return 0;

  return get_height(node->left) - get_height(node->right);
}

struct BinaryTree *rm(struct BinaryTree *node, struct Data data) {
  if (node == NULL)
    return node;

  enum Level cmp = compare(&data, node->data);
  if (cmp == LOWER)
    node->left = rm(node->left, data);
  else if (cmp == BIGGER)
    node->right = rm(node->right, data);
  else if (cmp == SAME) {
    if (node->left == NULL || node->right == NULL) {
      struct BinaryTree *temp = node->left ? node->left : node->right;

      if (temp == NULL) {
        temp = node;
        node = NULL;
      } else {
        *node = *temp;
      }
      free(temp);
    } else {
      struct BinaryTree *temp = children_left(node->right);
      node->data = temp->data;
      struct Data data = {temp->data->S, temp->data->N1, temp->data->N2};
      node->right = rm(node->right, data);
    }
  }
  if (node == NULL)
    return node;

  update_height(node);
  return rebalance(node);
}

#define indent printf("\t")
#define newl printf("\n")
#define p_down_right printf("↳")
#define p_up_right printf("↱")
#define p_right printf("→")
#define p_up_down_right printf("⇰")
#define p_out printf("↪")
#define out_node_data(x)                                                       \
  printf("%c%d%d", (x)->data->S, (x)->data->N1, (x)->data->N2)

void output_tree(struct BinaryTree *node, size_t depth, bool alone,
                 size_t first, size_t i_am_right) {

  if (node != NULL) {
    newl;
    if (first) {
      printf("Tree:\n");
      first = false;
      p_down_right;
      out_node_data(node);
    } else {
      for (size_t i = 0; i < depth; i++) {
        indent;
      }
      if (!i_am_right) {
        if (alone) {
          p_down_right;
        } else {
          p_down_right;
        }
        out_node_data(node);
      } else {
        p_down_right;
        out_node_data(node);
      }
    }
    alone = (node->node1 == NULL || node->node2 == NULL) ? true : false;
    output_tree(node->node1, depth + 1, alone, false, false);
    output_tree(node->node2, depth + 1, alone, false, true);
    if (node->node1 == NULL && node->node2 == NULL) {
      first = true;
      depth = 0;
    }

  } else {
    if (first)
      printf("Tree is empty\n");
  }
}

void output_search(struct SearchData *data) {
  printf("Search path: \n");
  for (size_t i = 0; i < data->count; i++) {
    if (i != 0)
      printf("->");

    printf("(%c%d%d)", data->values[i]->S, data->values[i]->N1,
           data->values[i]->N2);
  }
  printf("\n");
}