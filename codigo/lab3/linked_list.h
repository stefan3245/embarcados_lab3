#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct list_node {
  int value;
  struct list_node *next;
} linked_list;

int list_size(linked_list *list);
int get_value(linked_list *list, int pos);
void append(linked_list **list, int value);
int remove(linked_list **list, int pos);

#endif //LINKED_LIST_H
