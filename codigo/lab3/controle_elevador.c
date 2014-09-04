#include "cmsis_os.h"

#include "comunicacao.h"
#include "enfileirador.h"
#include "linked_list.h"

#include "controle_elevador.h"

extern linked_list *requisicoes;
extern osSemaphoreId requests_semaphore;

typedef enum {
  //S_INICIALIZANDO,
  S_OCIOSO,
  S_SUBINDO_PARADO,
  S_SUBINDO_SUBINDO,
  S_SUBINDO_PORTAS_ABERTAS,
  S_DESCENDO_PARADO,
  S_DESCENDO_DESCENDO,
  S_DESCENDO_PORTAS_ABERTAS
} state;
  

void task_controle_elevador(void const *arg) {
  osEvent evt; //Variável usada para ler eventos do SO.
  state curr_state = S_OCIOSO; //Estado atual do sistema
  
  int andar = 0; //Último andar onde o elevador parou
  
  while(1) {
    switch (curr_state) {
    case S_OCIOSO:
      evt = osSignalWait(0, osWaitForever);
      if (evt.status == osEventSignal) {
        if (evt.value.signals | SIGNAL_CONTROLE_REQUISICAO_FEITA) {
          osSemaphoreWait(requests_semaphore, osWaitForever);
          linked_list *destino = list_get_requisicao(requisicoes, 0);
          osSemaphoreRelease(requests_semaphore);
          if (destino->andar > andar) {
            curr_state = S_SUBINDO_PARADO;
            comunicacao_envia_comando_portas(-1);
          } else if (destino->andar < andar) {
            curr_state = S_DESCENDO_PARADO;
            comunicacao_envia_comando_portas(-1);
          } else {
            //TODO: mandar requisição atendida
          }
        }
      } else {
        //TODO: Tratar erros
      }
      break;
      
    case S_SUBINDO_PARADO:
      evt = osSignalWait(SIGNAL_CONTROLE_PORTAS_FECHADAS, osWaitForever);
      if (evt.status == osEventSignal) {
        curr_state = S_SUBINDO_SUBINDO;
        osSignalWait(SIGNAL_CONTROLE_TIME_DELAY, 500);
        comunicacao_envia_comando_movimento(1);
      } else {
        //TODO: tratar erros
      }
      break;
      
    case S_SUBINDO_SUBINDO:
      evt = osSignalWait(0, osWaitForever);
      if (evt.status == osEventSignal) {
      }
    }
  }
}