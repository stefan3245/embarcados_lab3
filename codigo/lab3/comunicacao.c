#include "comunicacao.h"

void task_comunicacao(void const *arg){
	char c; //Variável que contém cada caractere recebido.
	char str_numero_recebido[10]; //Contém a string dos números recebidos
	int numero_recebido; //Contém o número inteiro recebido
	
	osEvent evt; //Variável usada para ler mensagens da Mail Queue.
	
	//A cada iteração, verifica se existem caracteres recebidos da UART, e interpreta os caracteres recebidos.
	//Também, envia pela UART quaisquer mensagens que estejam na fila de espera.
	for(;;)
	{
		//
		//Lê um caractere em espera na UART
		//
		if(UART_read(&c) > 0){
			if(is_number(c)){ //Informação de posicionamento do elevador
				int i = 0;
				//Verifica os próximos caracteres do buffer, para ver se há mais algarismos.
				do{
					str_numero_recebido[i] = c;
					i++;
				} while(i < 4 && UART_read(&c) > 0 && is_number(c)); //Verifica se o próximo caractere é um algarismo também (podem ter no máximo 4 algarismos em uma mensagem).
				str_numero_recebido[i] = '\0'; //Termina a string.
                //Converte a string para int (esta variável agora indica qual é a posição do elevador)
				numero_recebido = atoi(str_numero_recebido); 
				//TODO: avisar à tarefa ControleElevador sobre a posição atual
			} //Não tem 'else' aqui, pois os próximos IFs devem ser testados sempre.
            
			if(c == 'A'){ //Informação de portas abertas
				//TODO: Avisar à tarefa de ControleElevador que as portas estão abertas
                
			} else if (c == 'F'){ //Informação de portas fechadas
                //TODO: Avisar à tarefa de ControleElevador que as portas estão fechadas	
                
            } else if (is_char_botao_andar(c)){ //Botão pressionado
                //TODO: Avisar à tarefa Enfileirador que recebeu uma requisição de botão de andar
                
                //liga a luz do botão correspondente
                comunicacao_envia_comando_ligar_botao(c);
                if(DEBUG) printf("[Comunicacao] Enviando: %c\n", c);
            }
		}
		//
		//Envia pela UART a próxima string que exista na Mail Queue de envio
		//
        evt = osMailGet(qid_filaEnvioMensagens, 1);
		if(evt.status == osEventMail){
			MsgFilaEnvio_t* msg = (MsgFilaEnvio_t*) evt.value.p; //Extrai os dados da mensagem
			int i;
			for(i = 0; i<strlen(msg->texto); i++){//Escreve a string na UART
				UART_write(msg->texto[i]); //Escreve o caractere na UART
			}
            UART_write(CR);//Escreve um caractere de fim de linha
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
* Retorna o caractere que corresponde ao botao externo do andar e à direção (subida/descida)
* @param andar
* @param direcao Direção atendida pelo elevador:
*	direcao >= 0 Para requisicao de subida
*	direcao <  0 Para requisicao de descida
*/
char get_char_botao_externo(int andar, int direcao){
    switch (andar){
    case 0:
        return 'a';
        break;
    case 1:
        if(direcao < 0) return 'b';
        else return 'c';
        break;
    case 2:
        if(direcao < 0) return 'd';
        else return 'e';
        break;
    case 3:
    default:
        return 'f';
        break;
    }
}

/**
* Retorna o caractere que corresponde ao botao interno do andar
*/
char get_char_botao_interno(int andar){
    return andar+'g';
}


/**
* Envia comando de inicialização do elevador (andar 0, portas abertas)
*/
void comunicacao_envia_comando_inicializa_elevador(){
	//Adiciona o comando 'R' à Mail Queue
	MsgFilaEnvio_t* msg = (MsgFilaEnvio_t*) osMailAlloc(qid_filaEnvioMensagens, 0);
	msg->texto[0] = 'R';
    msg->texto[1] = '\0';
	osMailPut(qid_filaEnvioMensagens, (void*) msg);
}

/**
* Executa os procedimentos que indicam que uma requisição de andar foi atendida. 
* Basicamente, apenas apaga as luzes internas e externas dos botões do andar (e direção) correspondente.
* @param andar O andar atendido pelo elevador
* @param direcao Direção atendida pelo elevador:
*	direcao >= 0 Para requisicao de subida
*	direcao <  0 Para requisicao de descida
*/
void comunicacao_envia_requisicao_atendida(int andar, int direcao){
    //Adiciona os comandos de desligar luz dos botões à Mail Queue
    //Desliga o botão interno
	MsgFilaEnvio_t* msg_interno = (MsgFilaEnvio_t*) osMailAlloc(qid_filaEnvioMensagens, 0);
	msg_interno->texto[0] = 'D';
    msg_interno->texto[1] = get_char_botao_interno(andar);
    msg_interno->texto[2] = '\0';
	osMailPut(qid_filaEnvioMensagens, (void*) msg_interno);
    //Desliga o botão externo
    MsgFilaEnvio_t* msg_externo = (MsgFilaEnvio_t*) osMailAlloc(qid_filaEnvioMensagens, 0);
	msg_externo->texto[0] = 'D';
    msg_externo->texto[1] = get_char_botao_externo(andar, direcao);
    msg_externo->texto[2] = '\0';
	osMailPut(qid_filaEnvioMensagens, (void*) msg_externo);
}

/**
* Envia comando para ligar a luz do botão especificado
*/
void comunicacao_envia_comando_ligar_botao(char botao){
    //Adiciona o comando à Mail Queue
	MsgFilaEnvio_t* msg = (MsgFilaEnvio_t*) osMailAlloc(qid_filaEnvioMensagens, 0);
    msg->texto[0] = 'L';
	msg->texto[1] = botao;
    msg->texto[2] = '\0';
	osMailPut(qid_filaEnvioMensagens, (void*) msg);
}

/**
* Envia um comando de movimento (subida/descida/parada) ao elevador.
* @param direcao :
*	direcao > 0 para subida
*	direcao < 0 para descida
*	direcao = 0 para parada
*/
void comunicacao_envia_comando_movimento(int direcao){
	char comando;
    if(direcao > 0)
        comando = 's';
    else if(direcao < 0)
        comando = 'd';
    else
        comando = 'p';
    
    //Adiciona o comando à Mail Queue
	MsgFilaEnvio_t* msg = (MsgFilaEnvio_t*) osMailAlloc(qid_filaEnvioMensagens, 0);
	msg->texto[0] = comando;
    msg->texto[1] = '\0';
	osMailPut(qid_filaEnvioMensagens, (void*) msg);
}

/**
* Envia um comando de movimento (abertura/fechamento) das portas.
* @param direcao :
*       direcao >= 0 para abertura
*       direcao <  0 para fechamento
*/
void comunicacao_envia_comando_portas(int direcao){
    char comando;
    if(direcao >=0)
        comando = 'a';
    else
        comando = 'f';
    
    //Adiciona o comando à Mail Queue
	MsgFilaEnvio_t* msg = (MsgFilaEnvio_t*) osMailAlloc(qid_filaEnvioMensagens, 0);
	msg->texto[0] = comando;
    msg->texto[1] = '\0';
	osMailPut(qid_filaEnvioMensagens, (void*) msg);
}

/**
* Envia uma requisição para saber qual é a posição atual do elevador.
*/
void comunicacao_envia_consulta_andar(){
	//Adiciona o comando à Mail Queue
	MsgFilaEnvio_t* msg = (MsgFilaEnvio_t*) osMailAlloc(qid_filaEnvioMensagens, 0);
	msg->texto[0] = 'x';
    msg->texto[1] = '\0';
	osMailPut(qid_filaEnvioMensagens, (void*) msg);
}



