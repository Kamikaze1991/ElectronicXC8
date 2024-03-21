
// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 2, CPUDIV = OSC1_PLL2, USBDIV = 2

// CONFIG1H
#pragma config FOSC = HSPLL_HS, FCMEN = OFF, IESO = OFF

// CONFIG2L
#pragma config PWRT = OFF, BOR = OFF, BORV = 3, VREGEN = OFF 

// CONFIG2H
#pragma config WDT = OFF    
#pragma config WDTPS = 32768

// CONFIG3H
#pragma config CCP2MX = ON, PBADEN = OFF, LPT1OSC = OFF, MCLRE = ON

// CONFIG4L
#pragma config STVREN = ON, LVP = OFF, ICPRT = OFF, XINST = OFF

// CONFIG5L
#pragma config CP0 = OFF,CP1 = OFF, CP2 = OFF, CP3 = OFF

// CONFIG5H
#pragma config CPB = OFF, CPD = OFF

// CONFIG6L
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF

// CONFIG6H
#pragma config WRTC = OFF, WRTB = OFF, WRTD = OFF

// CONFIG7L
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>

#define _XTAL_FREQ 48000000

#define CLK_DIR LATCbits.LC0
#define LATCH_DIR LATCbits.LC1
#define DATA_DIR LATCbits.LC2
#define TRIG_DIR TRISBbits.RB2
#define ECHO_DIR TRISBbits.RB3
#define TRIG_PIN LATBbits.LB2
#define ECHO_PIN PORTBbits.RB3

void inicializarSensor(void){
    TRIG_DIR = 0;
    ECHO_DIR = 0;
    T1CON = 0xf8;
    T1CONbits.TMR1ON = 1;
    
}

float obtenerDistancia(void){
    unsigned int timer_high;
    unsigned int timer_low;
    
    TRIG_PIN =1;
    __delay_ms(10);
    TRIG_PIN =0;
    while(ECHO_PIN==0);
    TMR1H=0x00;
    TMR1L=0x00;
    while(ECHO_PIN==1);
    timer_low=TMR1L;
    timer_high=TMR1H;
    float dis_hc = (((timer_high<<8)+timer_low)*0.000666*34.0)/2.0;
    return dis_hc;
}




int acumulador=7;
void setOne(){
    DATA_DIR=1;
    CLK_DIR=1;
    CLK_DIR=0;
    DATA_DIR=0;
    LATCH_DIR=1;
    LATCH_DIR=0;
}

void setZero(){
    DATA_DIR=0;
    CLK_DIR=1;
    CLK_DIR=0;
    DATA_DIR=1;
    LATCH_DIR=1;
    LATCH_DIR=0;
}

void desplazamiento(){
    for(int i=0;i<8;i++){
        if(i==acumulador){
            setZero();
        }
        else{
            setOne();
        }
            
    }
    
    if(acumulador<0){
        acumulador=7;
    }
    acumulador--;
}



void Send_Byte_Data(uint8_t b_m1)
{
    for(uint8_t i=0x80; i>0; i=i>>1)
    {
        (b_m1 & i) ? (DATA_DIR = 1) : (DATA_DIR = 0);
        CLK_DIR = 1;
        CLK_DIR = 0;
    }
    LATCH_DIR = 1;
    LATCH_DIR = 0;
}

void main() {
    //unsigned char bicubic[8]={0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    const uint8_t bicubic[8]={0x10,0x18,0x1c,0xfe,0xfe,0x1c,0x18,0x10};
    
    const uint8_t peaton_stop[8] = {0x01, 0xe3, 0xf7, 0xe3, 0xd5, 0xf7, 0xeb, 0xeb}; 
    uint8_t contador_binario=0x80;
    int CONTADOR =0;
    ADCON1bits.PCFG=0x0f;
    TRISD = 0x00;
    TRISC = 0x00;
    LATD = 0x00;
    LATC = 0x00;
    LATCbits.LC6=1;
   
	while(1)
    {
        __delay_ms(1);
        
        
        //Send_Byte_Data(0x7f);
        Send_Byte_Data(~contador_binario);
        contador_binario=contador_binario>>1;
        if(contador_binario==0x00){
            contador_binario=0x80;
        }
        LATD=bicubic[CONTADOR];
        
        CONTADOR++;
        if(CONTADOR>7)
        {
            CONTADOR=0;
        }
        
        
        
    }
}

