#include "lab3/comunicacao.h"
#include "cmsis_os.h"
#include "mcu_regs.h"
#include "systick_clock.h"

osThreadId tid_Comunicacao;
osThreadDef(task_comunicacao, osPriorityNormal, 1, 2048);

int main()
{
    SystemCoreClockUpdate();
    //Inicializa a UART
    UART_init(115200);
    //Inicializa as Threads
    tid_Comunicacao = osThreadCreate(osThread(task_comunicacao), NULL);
    osDelay(1000);
    comunicacao_envia_comando_ligar_botao('a');
    //TODO: A função main deve virar uma thread. Isso é feito chamando-se diretamente a função da task (sem usar osThreadCreate)
    for(;;){
        osDelay(2000);
    }
}
