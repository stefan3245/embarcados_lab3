#include "cbuf.h"

//Inicializa o buffer
void cbuf_init(struct cbuf_s* cbuf, char* data, int size){
    cbuf->data = data;
    cbuf->size = size;
    cbuf->start = 0;
    cbuf->end = 0;
    cbuf->num_data = 0;
}

//Inicializa o buffer, preenchendo ele com um valor inicial
void cbuf_init_with_value(struct cbuf_s* cbuf, char* data, int size, char initial_value){
    int i;
    for(i = 0; i < size; i++){
        data[i] = initial_value;
    }
    cbuf_init(cbuf, data, size);
}

//Insere um valor ao final buffer
void cbuf_push(struct cbuf_s* cbuf, char c){
    
    cbuf->data[cbuf->end] = c;
    cbuf->end = (cbuf->end + 1) % cbuf->size; //Incrementa o contador de fim, e retorna ao início caso tenha chego ao final do buffer.
    if(cbuf->num_data >= cbuf->size){
        cbuf->start = (cbuf->start + 1) % cbuf->size;
    }else{
        cbuf->num_data++;
    }
}

//Retira e retorna o valor do início do buffer
char cbuf_pop(struct cbuf_s* cbuf){
    if(cbuf->num_data <= 0) //Caso não hajam valores no buffer, retorna '\0'
        return '\0';
    char c = cbuf->data[cbuf->start];
    cbuf->start = (cbuf->start + 1) % cbuf->size;
    cbuf->num_data--;
    return c;
}

//Limpa o buffer
void cbuf_clear(struct cbuf_s* cbuf){
    cbuf->start = 0;
    cbuf->end = 0;
    cbuf->num_data = 0;
}