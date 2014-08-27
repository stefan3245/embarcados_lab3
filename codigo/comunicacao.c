
#include "comunicacao.h"

void task_comunicacao(void const *arg){
	//Inicializa a UART
    UART_init(115200);
	
	char c; //Vari�vel que cont�m cada caractere recebido.
	char[10] str_numero_recebido; //Cont�m a string dos n�meros recebidos
	int numero_recebido; //Cont�m o n�mero inteiro recebido
	
	//A cada itera��o, verifica se existem caracteres recebidos da UART, e interpreta os caracteres recebidos.
	//Tamb�m, envia pela UART quaisquer mensagens que estejam na fila de espera.
	for(;;)
	{
		//
		//L� os caracteres em espera na UART
		//
		while(UART_read(&c) > 0){
			if(is_number(c)){
				int i = 0;
				//Verifica os pr�ximos caracteres do buffer, para ver se h� mais algarismos.
				do{
					str_numero_recebido[i] = c;
					i++;
				} while(UART_read(&c) > 0 && is_number(c)); //Verifica se o pr�ximo caractere � um algarismo tamb�m.
				str_numero_recebido[i] = '\0'; //Termina a string.
				numero_recebido = atoi(str_numero_recebido); //Converte a string para int
				//TODO: avisar � tarefa ControleElevador sobre a posi��o atual
			} //N�o tem 'else' aqui, pois os pr�ximos IFs devem ser testados sempre.
			if(c == 'A'){
				//TODO: Avisar � tarefa de ControleElevador que as portas est�o abertas
			} else 
			if (c == 'F') {
				//TODO: Avisar � tarefa de ControleElevador que as portas est�o fechadas	
			} else
			if (is_char_botao_andar(c)){
					//TODO: Avisar � tarefa Enfileirador que recebeu uma requisi��o de bot�o de andar
					//TODO: ligar a luz do bot�o do andar
			}
		}
		//
		//Envia pela UART os caracteres que existem na Message Box de envio
		//
		while(msgbox_com_elementos){
			
		}
	}
}

/**
* Testa se o caractere � um n�mero
*/
int is_number(char c){
	if(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
		return 1;
	else
		return 0;
}

/**
* Testa se o caractere equivale a um bot�o de andar
*/
int is_char_botao_andar(char c){
	if(c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || c == 'h' || c == 'i' || c == 'j')
		return 1;
	else
		return 0;
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



