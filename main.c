#include "tree.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define prnt(x) printf("%s\n", x)

#define check_until(x, y)                                                      \
  do {                                                                         \
    if (x != '\n') {                                                           \
      if (!y(x))                                                               \
        prnt("Symbol has incorrect type, use alpha for the first symbol, and " \
             "digit for two others");                                          \
    }                                                                          \
    x = getchar();                                                             \
  } while (!y(x))

#define fill_data_basic(data, S, N1, N2)                                       \
  (data).S = S;                                                                \
  (data).N1 = N1 - '0';                                                        \
  (data).N2 = N2 - '0'

#define input_data                                                             \
  prnt("Enter identifier: ");                                                  \
  check_until(S, isalpha);                                                     \
  check_until(N1, isdigit);                                                    \
  check_until(N2, isdigit);\
  prnt("Data filled successfully")

int main() {
  struct BinaryTree *tree = NULL;
  struct Data data;
  enum Menu { WAIT = 0, OUT = 1, ADD = 2, FIND = 3, DELETE = 4, EXIT = 5 };

  char *menu = "=== Menu ===\n"
               "1) Output tree\n"
               "2) Add node to tree\n"
               "3) Find node in tree\n"
               "4) Delete node in tree\n"
               "5) Exit\n"
               "Use <m> to display menu";

  char choice = WAIT;
  prnt(menu);
  do {
    choice = getchar();
    choice -= '0';
    char S = 'a';
    char N1 = '0';
    char N2 = '0';
    switch (choice) {
    case OUT:
      output_tree(tree, 0, true, true, false);
      printf("\n");
      break;
    case ADD:
      input_data;
      fill_data_basic(data, S, N1, N2);
      tree = add(tree, data);
      break;
    case FIND:
      input_data;
      fill_data_basic(data, S, N1, N2);
      struct SearchData *search = find(tree, data);
      output_search(search);
      break;
    case DELETE:
      input_data;
      fill_data_basic(data, S, N1, N2);
      tree = rm(tree, data);
      break;
    case WAIT:
    default:
      if (choice + '0' != '\n' && choice + '0' != 'm')
        prnt("Select another menu choice");
      break;
    case EXIT:
      free_tree(tree);
    };
    if (choice + '0' == 'm')
      prnt(menu);
  } while (choice != EXIT);
  return 0;
}