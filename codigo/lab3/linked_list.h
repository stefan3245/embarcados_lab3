#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef enum {
  INTERNO,
  SUBIDA,
  DESCIDA
} tipo_requisicao;

typedef struct requisicao {
  int andar;
  tipo_requisicao tipo;
  struct requisicao *next;
} linked_list;

int list_size(linked_list *list);
linked_list* list_get_requisicao(linked_list *list, int pos);
void list_append(linked_list **list, int andar, tipo_requisicao tipo);
int list_remove(linked_list **list, int pos);
int list_remove_value(linked_list **list, int andar, tipo_requisicao tipo);
int list_has_value(linked_list *list, int andar, tipo_requisicao tipo);
int list_maior_descida(linked_list *list);
int list_menor_subida(linked_list *list);
int list_maior_andar(linked_list *list);
int list_menor_andar(linked_list *list);

#endif //LINKED_LIST_H
