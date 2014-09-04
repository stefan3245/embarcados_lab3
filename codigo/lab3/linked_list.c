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

linked_list* list_get_requisicao(linked_list *list, int pos) {
  linked_list *aux = list;
  int counter = 0;
  while (aux != NULL) {
    if (counter == pos) return aux;
    counter++;
    aux = aux->next;
  }
  return NULL;
}

void list_append(linked_list **list, int andar, tipo_requisicao tipo) {
  linked_list *new_node = (linked_list*)malloc(sizeof(linked_list));
  new_node->andar = andar;
  new_node->tipo = tipo;
  new_node->next = NULL;
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

int list_remove_value(linked_list **list, int andar, tipo_requisicao tipo) {
  linked_list *aux = *list;
  if (aux == NULL) return -1;
  if (aux->andar == andar && aux->tipo == tipo) {
    free(*list);
    *list = NULL;
  }
  while (aux->next != NULL) {
    if (aux->next->andar == andar && aux->next->tipo == tipo) {
      linked_list *temp = aux->next;
      aux->next = aux->next->next;
      free(temp);
      return 0;
    }
    aux = aux->next;
  }
  return -1;
}

int list_has_value(linked_list *list, int andar, tipo_requisicao tipo) {
  linked_list *aux = list;
  while (aux != NULL) {
    if (aux->andar == andar && aux->tipo == tipo) return 1;
    aux = aux->next;
  }
  return 0;
}