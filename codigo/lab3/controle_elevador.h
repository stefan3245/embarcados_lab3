#ifndef CONTROLE_ELEVADOR_H
#define CONTROLE_ELEVADOR_H

#define SIGNAL_CONTROLE_REQUISICAO_FEITA 0x0001

#define SIGNAL_CONTROLE_PORTAS_ABERTAS 0x0002
#define SIGNAL_CONTROLE_PORTAS_FECHADAS 0x0004

#define SIGNAL_CONTROLE_ANDAR_T 0x0008
#define SIGNAL_CONTROLE_ANDAR_1 0x0010
#define SIGNAL_CONTROLE_ANDAR_2 0x0020
#define SIGNAL_CONTROLE_ANDAR_3 0x0040
#define SIGNAL_CONTROLE_ANDAR_ALL 0x0078 //Soma dos quatro anteriores

#define SIGNAL_CONTROLE_TIME_DELAY 0x8000 //Sinal nunca � gerado, usado para fazer generic wait

void task_controle_elevador(void const *arg);

#endif //CONTROLE_ELEVADOR_H
