#include "lab3/comunicacao.h"
#include "lab3/enfileirador.h"
#include "lab3/controle_elevador.h"
#include "cmsis_os.h"
#include "mcu_regs.h"
#include "systick_clock.h"

//Definição do timer de ticks do sistema
//osTimerId timer_Tick;
//osTimerDef(Tick, Tick_Handler);

//Defifições de Threads
osThreadId tid_Comunicacao;
osThreadDef(task_comunicacao, osPriorityNormal, 1, 0);
osThreadId tid_Enfileirador;
osThreadDef(task_enfileirador, osPriorityNormal, 1, 0);
osThreadId tid_Controle;
osThreadDef(task_controle_elevador, osPriorityNormal, 1, 0);

//Definições de Mail Queues
osMailQId qid_filaEnvioMensagens;
osMailQDef(filaEnvioMensagens, 128, MsgFilaEnvio_t);//Fila de envio de mensagens (tarefa Comunicacao)

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
    tid_Enfileirador = osThreadCreate(osThread(task_enfileirador), NULL);
    if (tid_Enfileirador == NULL) {
         printf("Erro ao criar Thread de Enfileirador!\n");
    }
    tid_Controle = osThreadCreate(osThread(task_controle_elevador), NULL);
    if (tid_Controle == NULL) {
         printf("Erro ao criar Thread de Controle do elevador!\n");
    }
    //Inicializa a Mail Queue de envio de mensagens (Tarefa de Comunicacao)
	qid_filaEnvioMensagens = osMailCreate(osMailQ(filaEnvioMensagens), tid_Comunicacao);
    //Inicializa o kernel
    osKernelStart();
    //Inicializa o elevador na posicao inicial
    comunicacao_envia_comando_inicializa_elevador();
    //
    //Testes (remover quando for utilizar o sistema em produção)
    //
    /*
    osDelay(2000);
    comunicacao_envia_comando_portas(-1);
    osDelay(2000);
    comunicacao_envia_comando_movimento(1);
    while(1){
        osDelay(1000);
        comunicacao_envia_consulta_posicao();
    }
    */
    
    //while(1);
    
    //Aqui não tem problema dar return, pois as outras threads vão continuar rodando mesmo que a Main finalize (fato comprovado na prática).
    return 0;
}
