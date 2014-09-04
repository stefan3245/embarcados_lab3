#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct list_node {
  int value;
  struct list_node *next;
} linked_list;

int list_size(linked_list *list);
int list_get_value(linked_list *list, int pos);
void list_append(linked_list **list, int value);
int list_remove(linked_list **list, int pos);

#endif //LINKED_LIST_H
