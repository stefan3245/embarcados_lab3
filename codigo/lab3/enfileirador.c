#include "stdlib.h"

#include "linked_list.h"
#include "controle_elevador.h"

#include "enfileirador.h"

extern osThreadId tid_Controle;

osSemaphoreId requests_semaphore;
osSemaphoreDef(requests_semaphore);

linked_list *requisicoes = NULL;

void task_enfileirador(void const *arg) {
  osEvent evt; //Variável usada para ler eventos do SO.
  
  requests_semaphore = osSemaphoreCreate(osSemaphore(requests_semaphore), 1);
  
  while(1) {
    evt = osSignalWait(0, osWaitForever);
    if (evt.status == osEventSignal) {
      osSemaphoreWait(requests_semaphore, osWaitForever);
      //if (available > 0) {
      if (evt.value.signals & SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_T) {
        osSignalClear(osThreadGetId(), SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_T);
        if (list_has_value(requisicoes, 0, INTERNO)) {
          list_append(&requisicoes, 0, INTERNO);
          osSignalSet(tid_Controle, SIGNAL_CONTROLE_REQUISICAO_FEITA);
        }
      }
      if (evt.value.signals & SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_1) {
        osSignalClear(osThreadGetId(), SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_1);
        if (list_has_value(requisicoes, 1, INTERNO)) {
          list_append(&requisicoes, 1, INTERNO);
          osSignalSet(tid_Controle, SIGNAL_CONTROLE_REQUISICAO_FEITA);
        }
      }
      if (evt.value.signals & SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_2) {
        osSignalClear(osThreadGetId(), SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_2);
        if (list_has_value(requisicoes, 2, INTERNO)) {
          list_append(&requisicoes, 2, INTERNO);
          osSignalSet(tid_Controle, SIGNAL_CONTROLE_REQUISICAO_FEITA);
        }
      }
      if (evt.value.signals & SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_3) {
        osSignalClear(osThreadGetId(), SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_3);
        if (list_has_value(requisicoes, 3, INTERNO)) {
          list_append(&requisicoes, 3, INTERNO);
          osSignalSet(tid_Controle, SIGNAL_CONTROLE_REQUISICAO_FEITA);
        }
      }
      if (evt.value.signals & SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_3) {
        osSignalClear(osThreadGetId(), SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_3);
        if (list_has_value(requisicoes, 3, DESCIDA)) {
          list_append(&requisicoes, 3, DESCIDA);
          osSignalSet(tid_Controle, SIGNAL_CONTROLE_REQUISICAO_FEITA);
        }
      }
      if (evt.value.signals & SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_2) {
        osSignalClear(osThreadGetId(), SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_2);
        if (list_has_value(requisicoes, 2, DESCIDA)) {
          list_append(&requisicoes, 2, DESCIDA);
          osSignalSet(tid_Controle, SIGNAL_CONTROLE_REQUISICAO_FEITA);
        }
      }
      if (evt.value.signals & SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_1) {
        osSignalClear(osThreadGetId(), SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_1);
        if (list_has_value(requisicoes, 1, DESCIDA)) {
          list_append(&requisicoes, 1, DESCIDA);
          osSignalSet(tid_Controle, SIGNAL_CONTROLE_REQUISICAO_FEITA);
        }
      }
      if (evt.value.signals & SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_T) {
        osSignalClear(osThreadGetId(), SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_T);
        if (list_has_value(requisicoes, 0, SUBIDA)) {
          list_append(&requisicoes, 0, SUBIDA);
          osSignalSet(tid_Controle, SIGNAL_CONTROLE_REQUISICAO_FEITA);
        }
      }
      if (evt.value.signals & SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_1) {
        osSignalClear(osThreadGetId(), SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_1);
        if (list_has_value(requisicoes, 1, SUBIDA)) {
          list_append(&requisicoes, 1, SUBIDA);
          osSignalSet(tid_Controle, SIGNAL_CONTROLE_REQUISICAO_FEITA);
        }
      }
      if (evt.value.signals & SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_2) {
        osSignalClear(osThreadGetId(), SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_2);
        if (list_has_value(requisicoes, 2, SUBIDA)) {
          list_append(&requisicoes, 2, SUBIDA);
          osSignalSet(tid_Controle, SIGNAL_CONTROLE_REQUISICAO_FEITA);
        }
      }
      
      osSemaphoreRelease(requests_semaphore);
      //} else {
      //TODO: tratar erros e/ou problemas de temporização
      //}
    } else {
      //TODO: tratar erros
    }
  }
}