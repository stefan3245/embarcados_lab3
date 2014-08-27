
#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include "uart_driver.h"

/**
* Tarefa de recebimento de mensagens do computador
*/
void task_comunicacao(void const *arg);

/**
* Executa os procedimentos que indicam que uma requisi��o de andar foi atendida. 
* Isto envolve atualmente apenas apagar a(s) luz(es) internas (ou externas) do andar correspondente.
* @param andar O andar atendido pelo elevador
* @param direcao Dire��o atendida pelo elevador:
*	direcao >= 0 Para requisicao de subida
*	direcao <  0 Para requisicao de descida
*/
void comunicacao_envia_requisicao_atendida(int andar, int direcao);

/**
* Envia um comando de movimento (subida/descida/parada) ao elevador.
* @param direcao :
*	direcao > 0 para subida
*	direcao < 0 para descida
*	direcao = 0 para parada
*/
void comunicacao_envia_comando_movimento(int direcao);

/**
* Envia um comando de movimento (abertura/fechamento) das portas.
* @param direcao :
*	 direcao >= 0 para abertura
*    direcao <  0 para fechamento
*/
void comunicacao_envia_comando_portas(int direcao);

#endif