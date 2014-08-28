
#include "comunicacao.h"

osMailQId qid_filaEnvioMensagens;
osMailQDef(filaEnvioMensagens, 128, MsgFilaEnvio_t);

void task_comunicacao(void const *arg){
	//Inicializa a UART
    UART_init(115200);
	//Inicializa a Msg Queue de envio de mensagens
	qid_filaEnvioMensagens = osMailCreate(osMailQ(filaEnvioMensagens), NULL);
	
	char c; //Variável que contém cada caractere recebido.
	char str_numero_recebido[10]; //Contém a string dos números recebidos
	int numero_recebido; //Contém o número inteiro recebido
	
	osEvent evt; //Variável usada para ler mensagens da Msg Queue.
	
	//A cada iteração, verifica se existem caracteres recebidos da UART, e interpreta os caracteres recebidos.
	//Também, envia pela UART quaisquer mensagens que estejam na fila de espera.
	for(;;)
	{
		//
		//Lê os caracteres em espera na UART
		//
		while(UART_read(&c) > 0){
			if(is_number(c)){
				int i = 0;
				//Verifica os próximos caracteres do buffer, para ver se há mais algarismos.
				do{
					str_numero_recebido[i] = c;
					i++;
				} while(UART_read(&c) > 0 && is_number(c)); //Verifica se o próximo caractere é um algarismo também.
				str_numero_recebido[i] = '\0'; //Termina a string.
				numero_recebido = atoi(str_numero_recebido); //Converte a string para int
				//TODO: avisar à tarefa ControleElevador sobre a posição atual
			} //Não tem 'else' aqui, pois os próximos IFs devem ser testados sempre.
			if(c == 'A'){
				//TODO: Avisar à tarefa de ControleElevador que as portas estão abertas
			} else 
			if (c == 'F') {
				//TODO: Avisar à tarefa de ControleElevador que as portas estão fechadas	
			} else
			if (is_char_botao_andar(c)){
					//TODO: Avisar à tarefa Enfileirador que recebeu uma requisição de botão de andar
					//TODO: ligar a luz do botão do andar
			}
		}
		//
		//Envia pela UART os caracteres que existem na Message Queue de envio
		//
                evt = osMailGet(qid_filaEnvioMensagens, 0);
		while(evt.status == osEventMail){
			MsgFilaEnvio_t* msg = (MsgFilaEnvio_t*) evt.value.p; //Extrai os dados da mensagem
			UART_write(msg->c); //Escreve o caractere na UART
                        evt = osMailGet(qid_filaEnvioMensagens, 0); //Verifica se há mais mensagens na fila
		}
	}
}

/**
* Testa se o caractere é um número
*/
int is_number(char c){
	if(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')
		return 1;
	else
		return 0;
}

/**
* Testa se o caractere equivale a um botão de andar
*/
int is_char_botao_andar(char c){
	if(c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || c == 'h' || c == 'i' || c == 'j')
		return 1;
	else
		return 0;
}


/**
* Envia comando de inicialização do elevador (andar 0, portas abertas)
*/
void comunicacao_inicializa_elevador(){
	//TODO enviar comando para UART;
}

/**
* Executa os procedimentos que indicam que uma requisição de andar foi atendida. 
* Isto envolve atualmente apenas apagar a(s) luz(es) internas (ou externas) do andar correspondente.
* @param andar O andar atendido pelo elevador
* @param direcao Direção atendida pelo elevador:
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
* Envia uma requisição de qual andar o elevador está no momento.
*/
void comunicacao_envia_consulta_andar(){
	//TODO enviar mensagem para a UART
}



