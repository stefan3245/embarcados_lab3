#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include "uart_driver.h"
#include <cmsis_os.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 

#define CR 0xD

#define DEBUG 1

extern osTimerId timer_Tick;
void Tick_Handler(void const *arg);

extern osMailQId qid_filaEnvioMensagens;

typedef struct {  // Estrutura de dados da Msg Queue de envio
        char texto[10]; //Texto a ser enviado.
} MsgFilaEnvio_t;

/**
* Tarefa de recebimento de mensagens do computador
*/
void task_comunicacao(void const *arg);

/**
* Testa se o caractere é um número
*/
int is_number(char c);

/**
* Testa se o caractere equivale a um botão de andar
*/
int is_char_botao_andar(char c);

/**
* Retorna o caractere que corresponde ao botao externo do andar e à direção (subida/descida)
* @param andar
* @param direcao Direção atendida pelo elevador:
*	direcao >= 0 Para requisicao de subida
*	direcao <  0 Para requisicao de descida
*/
char get_char_botao_externo(int andar, int direcao);

/**
* Retorna o caractere que corresponde ao botao interno do andar
*/
char get_char_botao_interno(int andar);

/**
* Executa os procedimentos que indicam que uma requisição de andar foi atendida. 
* Isto envolve atualmente apenas apagar a(s) luz(es) internas (ou externas) do andar correspondente.
* @param andar O andar atendido pelo elevador
* @param direcao Direção atendida pelo elevador:
*	direcao >= 0 Para requisicao de subida
*	direcao <  0 Para requisicao de descida
*/
void comunicacao_envia_requisicao_atendida(int andar, int direcao);

/**
* Envia comando para ligar a luz do botão especificado
*/
void comunicacao_envia_comando_ligar_botao(char botao);

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

/**
* Envia uma requisição de qual andar o elevador está no momento.
*/
void comunicacao_envia_consulta_andar();

#endif