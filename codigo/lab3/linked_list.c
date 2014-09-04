#include "stdlib.h"

#include "linked_list.h"

int list_size(linked_list *list) {
  linked_list *aux = list;
  int counter = 0;
  while (aux != NULL) {
    counter ++;
    aux = aux->next;
  }
  return counter;
}

int list_get_value(linked_list *list, int pos) {
  linked_list *aux = list;
  int counter = 0;
  while (aux != NULL) {
    if (counter == pos) return aux->value;
    counter++;
    aux = aux->next;
  }
  return -1;
}

void list_append(linked_list **list, int value) {
  linked_list *new_node = (linked_list*)malloc(sizeof(linked_list));
  linked_list *aux = *list;
  if (aux == NULL) {
    (*list) = new_node;
    return;
  }
  while (aux->next != NULL) aux = aux->next;
  aux->next = new_node;
}

int list_remove(linked_list **list, int pos) {
  linked_list *aux = *list;
  if (aux == NULL) return -1;
  if (pos == 0) {
    free(*list);
    *list = NULL;
  }
  int counter = 0;
  while (aux->next != NULL) {
    counter++;
    if (counter == pos) {
      linked_list *temp = aux->next;
      aux->next = aux->next->next;
      free(temp);
      return 0;
    }
    aux = aux->next;
  }
  return -1;
}
