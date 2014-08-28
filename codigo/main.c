#include "lab3/comunicacao.h"
#include "cmsis_os.h"
#include "mcu_regs.h"
#include "systick_clock.h"

osTimerId timer_Tick;
osTimerDef(Tick, Tick_Handler);

osThreadId tid_Comunicacao;
osThreadDef(task_comunicacao, osPriorityNormal, 1, 0);

osMailQId qid_filaEnvioMensagens;
osMailQDef(filaEnvioMensagens, 128, MsgFilaEnvio_t);

int main()
{
    SystemCoreClockUpdate();
    //Inicializa a UART
    UART_init(115200);
    printf("Inicializando...\n");
    //Inicializa as Threads
    tid_Comunicacao = osThreadCreate(osThread(task_comunicacao), NULL);
     if (tid_Comunicacao == NULL) {
         printf("Erro ao criar Thread de Comunicacao!\n");
    }
    //Inicializa a Mail Queue de envio de mensagens (Tarefa de Comunicacao)
	qid_filaEnvioMensagens = osMailCreate(osMailQ(filaEnvioMensagens), tid_Comunicacao);
    //comunicacao_envia_comando_ligar_botao('a'); //Testa a funcionalidade de envio de mensagens
    //Aqui não tem problema dar return, pois as outras threads vão continuar rodando mesmo que a Main finalize (isso foi testado com sucesso).
    return 0;
}
