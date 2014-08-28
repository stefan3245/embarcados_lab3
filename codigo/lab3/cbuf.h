#ifndef _cbuf_
#define _cbuf_

//
//Implementação de buffer circular
//
struct cbuf_s {
	char* data;
        int size; //Tamanho máximo do buffer
	int start;
	int end;
        int num_data; //Numero de elementos no buffer
};

//Inicializa o buffer
void cbuf_init(struct cbuf_s* cbuf, char* data, int size);

//Inicializa o buffer, preenchendo ele com um valor inicial
void cbuf_init_with_value(struct cbuf_s* cbuf, char* data, int size, char initial_value);

//Insere um valor no buffer
void cbuf_push(struct cbuf_s* cbuf, char c);

//Retira e retorna um valor do buffer
char cbuf_pop(struct cbuf_s* cbuf);

//Limpa o buffer
void cbuf_clear(struct cbuf_s* cbuf);

#endif