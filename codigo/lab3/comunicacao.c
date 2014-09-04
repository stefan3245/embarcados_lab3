#include "comunicacao.h"

#include "enfileirador.h"

extern osThreadId tid_Enfileirador;

void task_comunicacao(void const *arg){
	char c[10]; //Variável que contém a string de cada comando recebido do simulador.
	osEvent evt; //Variável usada para ler mensagens da Mail Queue.
	
	//A cada iteração, verifica se existem caracteres recebidos da UART, e interpreta os caracteres recebidos.
	//Também, envia pela UART quaisquer mensagens que estejam na fila de espera.
	for(;;)
	{
		//
		//Lê um caractere em espera na UART (caso haja)
		//
		if(UART_read(&c[0]) > 0){
            if(is_number(c[0])){
                //Lê tudo até chegar no LF
                //(Cada comando é terminado por CR LF)
                int k = 1;
                while(UART_read(&c[k]) > 0){ 
                    if(c[k] == LF) break;
                    k++;
                }
                c[k-1] = '\0'; //Substitui o CR por \0, dessa forma a string acaba antes dele.
                
                if(strlen(c) > 1){ //Caso a string do comando seja maior que 1, isso indica que é uma informação de posição do elevador
                    //Converte a string para int
                    int posicao = atoi(c); 
                    //TODO: avisar à tarefa ControleElevador sobre a posição atual
                    
                    if(DEBUG) printf("[Comunicacao] Posicao %d.\n", posicao);
                } else { //Informação de que chegou em um andar (só tem um algarismo)
                    //Converte a string para int
                    int andar = atoi(c);
                    //TODO: Avisar à tarefa de ControleElevador que chegou em um andar
                    
                    if(DEBUG) printf("[Comunicacao] Chegou no andar %d.\n", andar);
                }
            } else if(c[0] == 'A'){ //Informação de portas abertas
				//TODO: Avisar à tarefa de ControleElevador que as portas estão abertas
                //(NOTE: o simulador tem um bug, pois não avisa quando a porta está aberta)
                
                if(DEBUG) printf("[Comunicacao] Portas abertas.\n");
			} else if (c[0] == 'F'){ //Informação de portas fechadas
                //TODO: Avisar à tarefa de ControleElevador que as portas estão fechadas	
                //(NOTE: o simulador tem um bug, pois não avisa quando a porta está fechada)
                
                if(DEBUG) printf("[Comunicacao] Portas fechadas.\n");
            } else if (is_char_botao_andar(c[0])){ //Botão pressionado
                int32_t temp_signal;
                switch (c[0]) {
                case 'a':
                    temp_signal = SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_T;
                break;
                case 'b':
                    temp_signal = SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_1;
                break;
                case 'c':
                    temp_signal = SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_1;
                break;
                case 'd':
                    temp_signal = SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_2;
                break;
                case 'e':
                    temp_signal = SIGNAL_ENFILEIRADOR_BOTAO_SUBIDA_2;
                break;
                case 'f':
                    temp_signal = SIGNAL_ENFILEIRADOR_BOTAO_DESCIDA_3;
                break;
                case 'g':
                    temp_signal = SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_T;
                break;
                case 'h':
                    temp_signal = SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_1;
                break;
                case 'i':
                    temp_signal = SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_2;
                break;
                case 'j':
                    temp_signal = SIGNAL_ENFILEIRADOR_BOTAO_INTERNO_3;
                break;
              default:
                break;
                //TODO: tratar o erro
              }
              osSignalSet(tid_Enfileirador, temp_signal);
                
                //liga a luz do botão correspondente
                comunicacao_envia_comando_ligar_botao(c[0]);
                if(DEBUG) printf("[Comunicacao] Botao pressionado: %c\n", c[0]);
            }
            //Cede a execução para outras Threads
            //TODO: verificar se essa é a melhor maneira de coordenar as threads
            osThreadYield();
		}
		//
		//Envia para a UART a próxima string que esteja na Mail Queue
		//
        //TODO: verificar se o timeout de 1ms "osMailGet(qid_filaEnvioMensagens, 1)" não afeta a performance do sistema
        evt = osMailGet(qid_filaEnvioMensagens, 1);
		if(evt.status == osEventMail){
			MsgFilaEnvio_t* msg = (MsgFilaEnvio_t*) evt.value.p; //Extrai os dados da mensagem
			int i;
			for(i = 0; i<strlen(msg->texto); i++){//Escreve a string na UART
				UART_write(msg->texto[i]); //Escreve o caractere na UART
			}
            UART_write(CR);//Escreve um caractere de fim de linha
		}
        //Cede a execução para outras Threads
        //TODO: verificar se essa é a melhor maneira de coordenar as threads
        osThreadYield();
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
void comunicacao_envia_consulta_posicao(){
	//Adiciona o comando à Mail Queue
	MsgFilaEnvio_t* msg = (MsgFilaEnvio_t*) osMailAlloc(qid_filaEnvioMensagens, 0);
	msg->texto[0] = 'x';
    msg->texto[1] = '\0';
	osMailPut(qid_filaEnvioMensagens, (void*) msg);
}



