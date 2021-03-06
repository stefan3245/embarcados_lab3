#ifndef ENFILEIRADOR_H
#define ENFILEIRADOR_H

#include "cmsis_os.h" 

#define SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_T 0x0001
#define SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_1 0x0002
#define SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_2 0x0004
#define SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_3 0x0008
#define SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_T 0x0010
#define SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_1 0x0020
#define SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_2 0x0040
#define SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_1 0x0080
#define SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_2 0x0100
#define SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_3 0x0200

void task_enfileirador(void const *arg);

#endif //ENFILEIRADOR_H
