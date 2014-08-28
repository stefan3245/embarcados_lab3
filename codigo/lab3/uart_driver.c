#include "LPC17xx.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_pinsel.h"
#include "mcu_regs.h"
#include "cbuf.h"
#include "uart_driver.h"


#define BUFFER_RX_SIZE 32
#define BUFFER_TX_SIZE 64
#define SystemFrequency SystemCoreClock


volatile uint32_t UARTStatus;
//Buffers circulares de RX e TX
struct cbuf_s UART_RxBuffer;
struct cbuf_s UART_TxBuffer;
char UART_RxBuffer_Data[BUFFER_RX_SIZE];
char UART_TxBuffer_Data[BUFFER_TX_SIZE];
//Indica se a FIFO de TX está pronta para receber um novo dado
volatile uint8_t TxReady = 1;

#define LPC_UART            LPC_UART0
#define UART_IRQn           UART0_IRQn
#define CLKPWR_PCONP_PCUART CLKPWR_PCONP_PCUART0
#define CLKPWR_PCLKSEL_UART CLKPWR_PCLKSEL_UART0


//Tratamento de interrupcao da UART
void UART_IRQHandler(void)
{
    //TESTE COM SCOPE
    LPC_GPIO1->FIOSET |= (1<<10); //Seta a porta p1.10
    
    //Verifica qual é o tipo da interrupção
    uint8_t interrupt_type = LPC_UART->IIR;
    //Desconsidera o primeiro bit (flag global de interrupção)
    interrupt_type >>= 1;
    //Considera só os 3 primeiros bits do resultado (que representam qual é o tipo da interrupção)
    interrupt_type &= 0x07;
    
    //Interrupção RLS (receive line status), que é o tipo com maior prioridade (normalmente indica erro no recebimento)
    if (interrupt_type == IIR_RLS)
    {        
        uint8_t LSRValue = LPC_UART->LSR;
        //Erros de recebimento
        if (LSRValue & (LSR_OE | LSR_PE | LSR_FE | LSR_RXFE | LSR_BI))
        {
            //Limpa a interrupção, fazendo uma leitura que será descartada
            UARTStatus = LSRValue;
            uint8_t Dummy = LPC_UART->RBR;
            return;
        }
        //Dados disponíveis em RX (RDR = Receive Data Ready)
        else if (LSRValue & LSR_RDR)
        {
            while (LPC_UART->LSR & LSR_RDR)
            { // lê tudo da FIFO e armazena no buffer circular de leitura
                cbuf_push(&UART_RxBuffer, LPC_UART->RBR);
            }
        }
    }
    //Interrupção do tipo RDA (receive data available), que é a segunda em prioridade. 
    //Ela é ativada quando o número de caracteres no buffer passa do nível de trigger (no caso 8 caracteres).
    else if (interrupt_type == IIR_RDA)
    {
        //faz a leitura dos dados da fila
        while (LPC_UART->LSR & LSR_RDR)
        {
            cbuf_push(&UART_RxBuffer, LPC_UART->RBR);
        }
    }
    //Interrupção do tipo CTI (caracter timeout indicator).
    //Ela é ativada se passar muito tempo sem receber caracteres (de 3.5 a 4.5 vezes o tempo de um caractere)
    else if (interrupt_type == IIR_CTI)
    {            
        //faz a leitura dos dados da fila
        while (LPC_UART->LSR & LSR_RDR)
        {
            cbuf_push(&UART_RxBuffer, LPC_UART->RBR);
        }
    }
    //Interrupção do tipo THRE (transmit holding register empty)
    //Ela ocorre quando a FIFO de transmissão da UART fica vazia
    else if (interrupt_type == IIR_THRE)
    {
        //O THR está livre (o bit THRE está setado no registrador LSR)
        if (LPC_UART->LSR & LSR_THRE)
        {
            //Se não existirem dados para transmitir...
            if(UART_TxBuffer.num_data <= 0){
                //Indica que a transmissão está livre
                TxReady = 1;
            }else{
                //Enquanto existirem dados para transmitir...
                int count = 0;
                while(count < UART_TX_FIFO_SIZE && UART_TxBuffer.num_data > 0){
                    //Insere dados na FIFO
                    LPC_UART->THR = cbuf_pop(&UART_TxBuffer);
                    count++;
                }
                //Indica que a transmissão está ocupada
                TxReady = 0;
            }
        }
        else
        {
            //O THRE ainda possui dados válidos.
            //Indica que a transmissão está ocupada
            TxReady = 0;
        }
    }
    //TESTE COM SCOPE
    LPC_GPIO1->FIOCLR |= (1<<10); //Limpa a porta p1.10
    return;
}


//Inicializa a UART
void UART_init(uint32_t baudrate)
{
    
    //TESTE COM SCOPE
    LPC_GPIO1->FIODIR |= (1<<10); //seta a porta p1.10 como output
    LPC_GPIO1->FIOCLR |= (1<<10); //Clear na porta p1.10
    
    
    //Inicializa os buffers circulares
    cbuf_init_with_value(&UART_RxBuffer, UART_RxBuffer_Data, BUFFER_RX_SIZE, ' ');
    cbuf_init_with_value(&UART_TxBuffer, UART_TxBuffer_Data, BUFFER_TX_SIZE, ' ');
    
    uint32_t FDiv;
    uint32_t regVal = 1;
    
    //Limpa a IRQ da UART
    NVIC_DisableIRQ(UART_IRQn);
    
    //Inicializa os pinos da UART
    PINSEL_CFG_Type PinCfg;
    PinCfg.Pinmode = 0;
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;	
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 2;
    PINSEL_ConfigPin(&PinCfg);
    PinCfg.Pinnum = 3;
    PINSEL_ConfigPin(&PinCfg);
    
    //Configura o divisor de clock da UART para ser 1
    CLKPWR_SetPCLKDiv (CLKPWR_PCLKSEL_UART0, CLKPWR_PCLKSEL_CCLK_DIV_1);
    
    //Inicializa o clock da UART
    CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCUART, ENABLE);
    
    //Define 8 bits, 1 stop bit, sem paridade (isso é definido no Line Control Register)
    LPC_UART->LCR = 0x03;
    
    //Configura a baud rate.
    //Primeiramente, habilita o Divisor Latch Access Bit (DLAB) no registrador LCR
    LPC_UART->LCR |= 0x80;
    //Depois, configura o FDR (Fractional Divider Register), bits 0:3, para que não seja utilizado.
    //De acordo com o manual, o fator MULVAL (bits 7:4) deve ser >= 1, mesmo que o FDR não seja usado.
    LPC_UART->FDR = 0x10;
    
    //Calcula a baud rate
    FDiv = (((SystemCoreClock)/regVal)/16)/baudrate ;
    LPC_UART->DLM = FDiv / 256;
    LPC_UART->DLL = FDiv % 256;
    //DLAB = 0
    LPC_UART->LCR = 0x03;
    //Ativa e limpa as filas FIFO de TX e RX
    //Configura o trigger do RX para 8 caracteres
    LPC_UART->FCR = 0x07 | 0x80;
    
    //Limpa o LSR, fazendo-se uma leitura dele
    regVal = LPC_UART->LSR;
        
    //Habilita a interrupção da UART
    NVIC_EnableIRQ(UART_IRQn);
    LPC_UART->IER |= IER_RBR | IER_THRE | IER_RLS;
    
    return;
}


//Escreve um caractere no buffer circular de TX (e já manda-o pela UART, caso a FIFO de TX esteja livre)
void UART_write(char c)
{
    //Insere o conteúdo no buffer circular de transmissão
    cbuf_push(&UART_TxBuffer, c);
    
    //Escreve o caractere na FIFO TX da UART caso esteja livre
    if (TxReady)
    {
        LPC_UART->THR = cbuf_pop(&UART_TxBuffer);
        TxReady = 0;
    }    
    
    return;
}


//Retira e retorna um caractere (se houver) do buffer circular de RX
int UART_read(char* c)
{
    if (UART_RxBuffer.num_data > 0)
    {
        *c = cbuf_pop(&UART_RxBuffer);
        return 1;
    }
    return 0;
}

uint8_t UART_GetNumCharsTX()
{
    return UART_TxBuffer.num_data;
}

uint8_t UART_GetNumCharsRX()
{
    return UART_RxBuffer.num_data;
}