#include "lab3/comunicacao.h"
#include "cmsis_os.h"

osThreadId tid_Comunicacao;
osThreadDef(task_comunicacao, osPriorityNormal, 1, 1024);

int main()
{
    //Inicializa as Threads
    tid_Comunicacao = osThreadCreate(osThread(task_comunicacao), NULL);
    return 0;
}
