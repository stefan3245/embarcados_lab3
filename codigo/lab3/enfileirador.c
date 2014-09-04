#include "stdlib.h"

#include "linked_list.h"

#include "enfileirador.h"

osSemaphoreId requests_semaphore;
osSemaphoreDef(requests_semaphore);

void task_enfileirador(void const *arg) {
  osEvent evt; //Variável usada para ler eventos do SO.
  linked_list *requisicoes = NULL;

  requests_semaphore = osSemaphoreCreate(osSemaphore(requests_semaphore), 1);
  
  while(1) {
    evt = osSignalWait(0, osWaitForever);
    if (evt.status == osEventSignal) {
      if (evt.value.signals | SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_T)
        list_append(&requisicoes, REQUISICAO_BOTAO_INTERNO_T);
      if (evt.value.signals | SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_1)
        list_append(&requisicoes, REQUISICAO_BOTAO_INTERNO_1);
      if (evt.value.signals | SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_2)
        list_append(&requisicoes, REQUISICAO_BOTAO_INTERNO_2);
      if (evt.value.signals | SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_3)
        list_append(&requisicoes, REQUISICAO_BOTAO_INTERNO_3);
      if (evt.value.signals | SIGNAL_ENFILEIRADOR_BOTAO_DESCE_3)
        list_append(&requisicoes, REQUISICAO_BOTAO_DESCE_3);
      if (evt.value.signals | SIGNAL_ENFILEIRADOR_BOTAO_DESCE_2)
        list_append(&requisicoes, REQUISICAO_BOTAO_DESCE_2);
      if (evt.value.signals | SIGNAL_ENFILEIRADOR_BOTAO_DESCE_1)
        list_append(&requisicoes, REQUISICAO_BOTAO_DESCE_1);
      if (evt.value.signals | SIGNAL_ENFILEIRADOR_BOTAO_SOBE_T)
        list_append(&requisicoes, REQUISICAO_BOTAO_SOBE_T);
      if (evt.value.signals | SIGNAL_ENFILEIRADOR_BOTAO_SOBE_1)
        list_append(&requisicoes, REQUISICAO_BOTAO_SOBE_1);
      if (evt.value.signals | SIGNAL_ENFILEIRADOR_BOTAO_SOBE_2)
        list_append(&requisicoes, REQUISICAO_BOTAO_SOBE_2);
    } else {
      //TODO: tratar erros
    }
  }
}