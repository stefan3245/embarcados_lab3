#include "cmsis_os.h"

#include "comunicacao.h"
#include "enfileirador.h"
#include "linked_list.h"

#include "controle_elevador.h"

extern linked_list *requisicoes;
extern osSemaphoreId requests_semaphore;
extern osThreadId tid_Enfileirador;

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
  linked_list *destino = NULL;
  
  while(1) {
    switch (curr_state) {
    case S_OCIOSO:
      evt = osSignalWait(0, osWaitForever);
      if (evt.status == osEventSignal) {
        if (evt.value.signals & SIGNAL_CONTROLE_REQUISICAO_FEITA) {
          osSignalClear(osThreadGetId(), SIGNAL_CONTROLE_REQUISICAO_FEITA);
          osSemaphoreWait(requests_semaphore, osWaitForever);
          destino = list_get_requisicao(requisicoes, 0);
          if (destino->andar > andar) {
            curr_state = S_SUBINDO_PARADO;
            comunicacao_envia_comando_portas(-1);
          } else if (destino->andar < andar) {
            curr_state = S_DESCENDO_PARADO;
            comunicacao_envia_comando_portas(-1);
          } else {
            list_remove_value(&requisicoes, andar, INTERNO);
            comunicacao_envia_requisicao_atendida(andar, 0);
            list_remove_value(&requisicoes, andar, SUBIDA);
            comunicacao_envia_requisicao_atendida(andar, 1);
            list_remove_value(&requisicoes, andar, DESCIDA);
            comunicacao_envia_requisicao_atendida(andar, -1);
          }
          osSemaphoreRelease(requests_semaphore);
        }
      } else {
        //TODO: Tratar erros
      }
      break;
      
    case S_SUBINDO_PARADO:
      evt = osSignalWait(SIGNAL_CONTROLE_PORTAS_FECHADAS, osWaitForever);
      if (evt.status == osEventSignal) {
        osSignalClear(osThreadGetId(), SIGNAL_CONTROLE_PORTAS_FECHADAS);
        curr_state = S_SUBINDO_SUBINDO;
        osSignalWait(SIGNAL_CONTROLE_TIME_DELAY, 500); //Espera 500ms
        osSignalClear(osThreadGetId(), SIGNAL_CONTROLE_TIME_DELAY);
        comunicacao_envia_comando_movimento(1);
      } else {
        //TODO: tratar erros
      }
      break;
      
    case S_SUBINDO_SUBINDO:
      evt = osSignalWait(0, osWaitForever);
      if (evt.status == osEventSignal) {
        int aux = -1;
        switch (evt.value.signals) {
        case SIGNAL_CONTROLE_ANDAR_T:
          aux = 0;
          break;
        case SIGNAL_CONTROLE_ANDAR_1:
          aux = 1;
          break;
        case SIGNAL_CONTROLE_ANDAR_2:
          aux = 2;
          break;
        case SIGNAL_CONTROLE_ANDAR_3:
          aux = 3;
          break;
        default:
          aux = -2;
          break;
        }
        osSemaphoreWait(requests_semaphore, osWaitForever);
        if ((list_has_value(requisicoes, aux, INTERNO)) ||
            (list_has_value(requisicoes, aux, SUBIDA)) ||
              (aux == list_maior_descida(requisicoes)) ) {
                comunicacao_envia_comando_movimento(0);
                osSignalWait(SIGNAL_CONTROLE_TIME_DELAY, 500); //Espera 500ms
                osSignalClear(osThreadGetId(), SIGNAL_CONTROLE_TIME_DELAY);
                curr_state = S_SUBINDO_PORTAS_ABERTAS;
                comunicacao_envia_comando_portas(1);
                andar = aux;
                list_remove_value(&requisicoes, andar, INTERNO);
                comunicacao_envia_requisicao_atendida(andar, 0);
                list_remove_value(&requisicoes, andar, SUBIDA);
                comunicacao_envia_requisicao_atendida(andar, 1);
                if (aux == list_maior_descida(requisicoes)) {
                  list_remove_value(&requisicoes, andar, DESCIDA);
                  comunicacao_envia_requisicao_atendida(andar, -1);
                }
              }
      }
      osSemaphoreRelease(requests_semaphore);
      break;
      
    case S_SUBINDO_PORTAS_ABERTAS:
      osSignalWait(SIGNAL_CONTROLE_TIME_DELAY, 5000); //Espera 5 segundos
      osSignalClear(osThreadGetId(), SIGNAL_CONTROLE_TIME_DELAY);
      osSemaphoreWait(requests_semaphore, osWaitForever);
      destino = list_get_requisicao(requisicoes, 0);
      osSemaphoreRelease(requests_semaphore);
      if (destino == NULL) {
        curr_state = S_OCIOSO;
      } else {
        if (destino->andar > andar) {
          curr_state = S_SUBINDO_PARADO;
          comunicacao_envia_comando_portas(-1);
        } else if (destino->andar < andar) {
          curr_state = S_DESCENDO_PARADO;
          comunicacao_envia_comando_portas(-1);
        } else {
          list_remove_value(&requisicoes, andar, INTERNO);
          comunicacao_envia_requisicao_atendida(andar, 0);
          list_remove_value(&requisicoes, andar, SUBIDA);
          comunicacao_envia_requisicao_atendida(andar, 1);
          list_remove_value(&requisicoes, andar, DESCIDA);
          comunicacao_envia_requisicao_atendida(andar, -1);
        }
      }
      break;
      
    case S_DESCENDO_PARADO:
      evt = osSignalWait(SIGNAL_CONTROLE_PORTAS_FECHADAS, osWaitForever);
      osSignalClear(osThreadGetId(), SIGNAL_CONTROLE_PORTAS_FECHADAS);
      if (evt.status == osEventSignal) {
        curr_state = S_DESCENDO_DESCENDO;
        osSignalWait(SIGNAL_CONTROLE_TIME_DELAY, 500); //Espera 500ms
        osSignalClear(osThreadGetId(), SIGNAL_CONTROLE_TIME_DELAY);
        comunicacao_envia_comando_movimento(-1);
      } else {
        //TODO: tratar erros
      }
      break;
      
    case S_DESCENDO_DESCENDO:
      evt = osSignalWait(0, osWaitForever);
      if (evt.status == osEventSignal) {
        osSignalClear(osThreadGetId(), SIGNAL_CONTROLE_ANDAR_ALL);
        int aux = -1;
        switch (evt.value.signals) {
        case SIGNAL_CONTROLE_ANDAR_T:
          aux = 0;
          break;
        case SIGNAL_CONTROLE_ANDAR_1:
          aux = 1;
          break;
        case SIGNAL_CONTROLE_ANDAR_2:
          aux = 2;
          break;
        case SIGNAL_CONTROLE_ANDAR_3:
          aux = 3;
          break;
        default:
          aux = -2;
          break;
        }
        osSemaphoreWait(requests_semaphore, osWaitForever);
        if ((list_has_value(requisicoes, aux, INTERNO)) ||
            (list_has_value(requisicoes, aux, DESCIDA)) ||
              (aux == list_menor_subida(requisicoes)) ) {
                comunicacao_envia_comando_movimento(0);
                osSignalWait(SIGNAL_CONTROLE_TIME_DELAY, 500); //Espera 500ms
                osSignalClear(osThreadGetId(), SIGNAL_CONTROLE_TIME_DELAY);
                curr_state = S_DESCENDO_PORTAS_ABERTAS;
                comunicacao_envia_comando_portas(1);
                andar = aux;
                list_remove_value(&requisicoes, andar, INTERNO);
                comunicacao_envia_requisicao_atendida(andar, 0);
                list_remove_value(&requisicoes, andar, DESCIDA);
                comunicacao_envia_requisicao_atendida(andar, -1);
                if (aux == list_menor_subida(requisicoes)) {
                  list_remove_value(&requisicoes, andar, DESCIDA);
                  comunicacao_envia_requisicao_atendida(andar, 1);
                }
              }
        osSemaphoreRelease(requests_semaphore);
      }
      
    case S_DESCENDO_PORTAS_ABERTAS:
      osSignalWait(SIGNAL_CONTROLE_TIME_DELAY, 5000); //Espera 5 segundos
      osSignalClear(osThreadGetId(), SIGNAL_CONTROLE_TIME_DELAY);
      osSemaphoreWait(requests_semaphore, osWaitForever);
      destino = list_get_requisicao(requisicoes, 0);
      osSemaphoreRelease(requests_semaphore);
      if (destino == NULL) {
        curr_state = S_OCIOSO;
      } else {
        if (destino->andar > andar) {
          curr_state = S_SUBINDO_PARADO;
          comunicacao_envia_comando_portas(-1);
        } else if (destino->andar < andar) {
          curr_state = S_DESCENDO_PARADO;
          comunicacao_envia_comando_portas(-1);
        } else {
          list_remove_value(&requisicoes, andar, INTERNO);
          comunicacao_envia_requisicao_atendida(andar, 0);
          list_remove_value(&requisicoes, andar, SUBIDA);
          comunicacao_envia_requisicao_atendida(andar, 1);
          list_remove_value(&requisicoes, andar, DESCIDA);
          comunicacao_envia_requisicao_atendida(andar, -1);
        }
      }
      break;
      
    default:
      //TODO: tratar erro
      break;
    } 
  }
}