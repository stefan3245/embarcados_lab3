
#include "comunicacao.h"

void task_comunicacao(void const *arg){
	//Inicializa a UART
    UART_init(115200);
	
	char c; //Vari�vel que cont�m cada caractere recebido.
	
	//A cada itera��o, verifica se existem caracteres recebidos da UART, e interpreta os caracteres recebidos.
	//Tamb�m, envia pela UART quaisquer mensagens que estejam na fila de espera.
	for(;;)
	{
		//L� os caracteres em espera na UART
		while(UART_read(&c) > 0){
			switch(c){
				case 'A':
					//TODO: Avisar � tarefa de controle do elevador que as portas est�o abertas
					break;
				case 'F':
					//TODO: Avisar � tarefa de controle do elevador que as portas est�o fechadas
					break;
				case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': 
					//TODO: Avisar � tarefa de controle do elevador que recebeu uma requisi��o de bot�o de andar
					//TODO: ligar a luz do bot�o do andar
					break;
			}
		}
		//Envia os caracteres da Message Box de envio
		while(msgbox_com_elementos){
			
		}
	}
}

/**
* Envia comando de inicializa��o do elevador (andar 0, portas abertas)
*/
void comunicacao_inicializa_elevador(){

}

/**
* Executa os procedimentos que indicam que uma requisi��o de andar foi atendida. 
* Isto envolve atualmente apenas apagar a(s) luz(es) internas (ou externas) do andar correspondente.
* @param andar O andar atendido pelo elevador
* @param direcao Dire��o atendida pelo elevador:
*	direcao >= 0 Para requisicao de subida
*	direcao <  0 Para requisicao de descida
*/
void comunicacao_envia_requisicao_atendida(int andar, int direcao){
	//TODO enviar mensagem para a UART
}

/**
* Envia um comando de movimento (subida/descida/parada) ao elevador.
* @param direcao :
*	direcao > 0 para subida
*	direcao < 0 para descida
*	direcao = 0 para parada
*/
void comunicacao_envia_comando_movimento(int direcao){
	//TODO enviar mensagem para a UART
}

/**
* Envia um comando de movimento (abertura/fechamento) das portas.
* @param direcao :
*	 direcao >= 0 para abertura
*    direcao <  0 para fechamento
*/
void comunicacao_envia_comando_portas(int direcao){
	//TODO enviar mensagem para a UART
}

/**
* Envia uma requisi��o de qual andar o elevador est� no momento.
*/
void comunicacao_envia_consulta_andar(){
	//TODO enviar mensagem para a UART
}



