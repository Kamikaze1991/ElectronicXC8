#pragma config PLLDIV = 2, CPUDIV = OSC1_PLL2, USBDIV = 2
#pragma config FOSC = HSPLL_HS, FCMEN = OFF, IESO = OFF
#pragma config PWRT = OFF, BOR = OFF, BORV = 3, VREGEN = OFF 
#pragma config WDT = OFF    
#pragma config WDTPS = 32768
#pragma config CCP2MX = ON, PBADEN = OFF, LPT1OSC = OFF, MCLRE = ON
#pragma config STVREN = ON, LVP = OFF, ICPRT = OFF, XINST = OFF
#pragma config CP0 = OFF,CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF, CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF
#pragma config WRTC = OFF, WRTB = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF
#pragma config EBTRB = OFF     

#include <xc.h>

#define _XTAL_FREQ 48000000



#define CLK_DIR LATCbits.LC0
#define LATCH_DIR LATCbits.LC1
#define DATA_DIR LATCbits.LC2

#define TRIG_DIR TRISAbits.RA0
#define ECHO_DIR TRISAbits.RA1
#define TRIG_PIN LATAbits.LA0
#define ECHO_PIN PORTAbits.RA1

uint16_t fila_flow;
uint16_t columna_flow;
void inicializarSensor(){
    ADCON1bits.PCFG=0x0f;
    TRIG_DIR = 0;
    ECHO_DIR = 1;
    T0CON=0x88;
    TMR0=0;
}

void inicializarPantalla()
{

}



float obtenerDistancia(void){
    
    float distancia;
    uint16_t tiempo;
    TRIG_PIN =1;
    __delay_ms(10);
    TRIG_PIN =0;
    TMR0 =0;
    while(ECHO_PIN==0);
    T0CONbits.TMR0ON=1;
    while(ECHO_PIN==1);
    T0CONbits.TMR0ON=0;
    tiempo=TMR0;
    distancia=tiempo*0.017/2.0;
    return distancia;
}




int acumulador=7;

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

void drawCoord(int fila_in, int columna_in){
        int col_in = columna_in;
        int fil_in= fila_in;
        fil_in=(fil_in-7)*-1;
        fila_flow=0x80;
        columna_flow=0x80;
        columna_flow=columna_flow>>col_in;
        fila_flow=fila_flow>>fil_in;
        Send_Byte_Data(~columna_flow);
        LATD=fila_flow;
}

void main() {
    //unsigned char bicubic[8]={0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    
    //uint8_t bicubic[8]={0x10,0x18,0x1c,0xfe,0xfe,0x1c,0x18,0x10};
    
    //const uint8_t peaton_stop[8] = {0x01, 0xe3, 0xf7, 0xe3, 0xd5, 0xf7, 0xeb, 0xeb}; 
   // uint8_t contador_binario=0x80;
    //int CONTADOR =0;
    //inicializarSensor();
    //__delay_ms(1000);
    //float dis=obtenerDistancia();
    //__delay_ms(1000);
    //if(dis>0)
    //    return;
    //ADCON1bits.PCFG=0x0f;
    
    inicializarSensor();
    
   
   
  
   
      
    TRISD = 0x00;
    TRISC = 0x00;
    LATD = 0x00;
    LATCbits.LC6=1;
    int contador=0;
	while(1)
    {
                    
        
        
        float distancia= obtenerDistancia();
        
        if(distancia>0&&distancia<50)drawCoord(0,1);
        else if(distancia>50&&distancia<100)drawCoord(0,0);
        else if(distancia>100&&distancia<150)drawCoord(0,1);
        else if(distancia>150&&distancia<200)drawCoord(0,2);
        else if(distancia>200&&distancia<250)drawCoord(0,3);
        else if(distancia>250&&distancia<300)drawCoord(0,4);
        else if(distancia>300&&distancia<350)drawCoord(0,5);
        else if(distancia>350&&distancia<400)drawCoord(0,6);
        else if(distancia>400&&distancia<450)drawCoord(0,7);
        else if(distancia>450&&distancia<500)drawCoord(1,0);
        else drawCoord(1,1);
         
        
        //Send_Byte_Data(0x7f);
        
    }
}



