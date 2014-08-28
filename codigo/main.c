#include "lab3/comunicacao.h"
#include "cmsis_os.h"
#include "mcu_regs.h"
#include "systick_clock.h"

osTimerId timer_Tick;
osTimerDef(Tick, Tick_Handler);

osThreadId tid_Comunicacao;
osThreadDef(task_comunicacao, osPriorityNormal, 1, 2048);

osMailQId qid_filaEnvioMensagens;
osMailQDef(filaEnvioMensagens, 128, MsgFilaEnvio_t);

int main()
{
    SystemCoreClockUpdate();
    //Inicializa a UART
    UART_init(115200);
    UART_write('k');
    //Inicializa as Threads
    tid_Comunicacao = osThreadCreate(osThread(task_comunicacao), NULL);
    //Inicializa a Msg Queue de envio de mensagens
	qid_filaEnvioMensagens = osMailCreate(osMailQ(filaEnvioMensagens), tid_Comunicacao);
    osDelay(1000);
    comunicacao_envia_comando_ligar_botao('a');
    //TODO: A função main deve virar uma thread. Isso é feito chamando-se diretamente a função da task (sem usar osThreadCreate)
    for(;;){
        osDelay(2000);
    }
}
