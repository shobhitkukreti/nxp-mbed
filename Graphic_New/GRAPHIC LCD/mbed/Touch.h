 /*
   \file Touch.h
   \version: 1.0
   
   \brief Este fichero contiene class para control de TouchScreen ADS7843 
   
   \web www.micros-designs.com.ar
   \date 16/01/11
   
 *- Version Log --------------------------------------------------------------*
 *   Fecha       Autor                Comentarios                             *
 *----------------------------------------------------------------------------*
 * 16/01/11      Suky        Original                                         *
 *----------------------------------------------------------------------------* 
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                                                                   ////
////        (C) Copyright 2011 www.micros-designs.com.ar               ////
//// Este código puede ser usado, modificado y distribuido libremente  ////
//// sin eliminar esta cabecera y  sin garantía de ningún tipo.        ////
////                                                                   ////
////                                                                   ////
/////////////////////////////////////////////////////////////////////////// 
// **********************************************************************
// ************************** EXAMPLE ***********************************
// **********************************************************************
#include "mbed.h"
#include "Touch.h"
// **********************************************************************
#define TOUCH_X_MIN 30
#define TOUCH_X_MAX 230
#define TOUCH_Y_MIN 15
#define TOUCH_Y_MAX 240
// **********************************************************************
DigitalOut Led1(LED1);
DigitalOut Led2(LED2);
// **********************************************************************
TOUCH_DATA Coordenadas;
volatile bool kbhit_irq=0;

TouchScreenADS7843 MyTouch(p9,p10,p11,p12,p8); // ADS7843-> SDO,SDI,SCK,CS,IRQ
// **********************************************************************
void vISRTouchIRQ(void){
    UINT_COORDxy Temp;

    MyTouch.vTestTouchPanel(&Coordenadas);
    // Coordenadas dentro del rango de pantalla LCD:
    if((Coordenadas.x>TOUCH_X_MIN && Coordenadas.x<TOUCH_X_MAX) && (Coordenadas.y>TOUCH_Y_MIN && Coordenadas.y<TOUCH_Y_MAX)){
        // Cambio de coordenada y escala:
        Temp=Coordenadas.y;
        Coordenadas.y=((float)((255-TOUCH_X_MIN)-Coordenadas.x)*(320.0/(TOUCH_X_MAX-TOUCH_X_MIN)));
        Coordenadas.x=((float)(Temp-TOUCH_Y_MIN)*(240.0/(TOUCH_Y_MAX-TOUCH_Y_MIN)));
        kbhit_irq=1;
    }
}
// **********************************************************************
int main(){

    //MyTouch.vInitADS7843(USE_12BITS,NULL);
    MyTouch.vInitADS7843(USE_8BITS,&vISRTouchIRQ);

    Led1=0;
    Led2=0;

    while(1){
        Led2=!Led2;
        if(kbhit_irq==1){
            kbhit_irq=0;
            if((Coordenadas.x<235 && Coordenadas.x>135) && (Coordenadas.y<320 && Coordenadas.y>250)){
                Led1=!Led1;
            }
        }
        wait_ms(300);
    }
}
// **********************************************************************
*/
#include "mbed.h"
// **********************************************************************
#define USE_12BITS            12
#define USE_8BITS            8
#define ENABLE_INTERRUPT    1
#define DISABLE_INTERRUPT    0
// **********************************************************************
typedef unsigned short             UINT_COORDxy;
typedef struct {
    UINT_COORDxy    x;
    UINT_COORDxy    y;
}TOUCH_DATA;
// **********************************************************************
class TouchScreenADS7843 {
public:
    TouchScreenADS7843(PinName PIN_SDO,PinName PIN_SDI,PinName PIN_CLK,PinName PIN_CS,PinName PIN_IRQ);
    void vInitADS7843(unsigned char Bits,void (*fptr)(void));
    void vSetResolution(unsigned char Bits);
    void vTestTouchPanel(TOUCH_DATA *Data);
    void vEnableSetInterrupt(void (*fptr)(void));
    void vDisableInterrupt(void);
    bool bReadPinIRQ(void);
protected:
#define     __delay_CLK_ADS7843()     wait_us(1)
#define     ADS7843_READ_X_8        0x98 // IRQ Enable     
#define     ADS7843_READ_Y_8        0xD8
#define     ADS7843_READ_X_12       0x90 // IRQ Enable
#define     ADS7843_READ_Y_12       0xD0
    DigitalOut     _PIN_SDI,_PIN_CS,_PIN_CLK;
    DigitalIn     _PIN_SDO,_PIN_IRQ;;
    InterruptIn ADS7843_Event_Touch;
	
    unsigned char Resolution;
    virtual UINT_COORDxy ADS7843WriteAndReadData(unsigned char Data);
};
// **********************************************************************
TouchScreenADS7843::TouchScreenADS7843(PinName PIN_SDO,PinName PIN_SDI,PinName PIN_CLK,PinName PIN_CS,PinName PIN_IRQ)
        : _PIN_SDO(PIN_SDO),_PIN_SDI(PIN_SDI),_PIN_CS(PIN_CS),_PIN_CLK(PIN_CLK),_PIN_IRQ(PIN_IRQ),ADS7843_Event_Touch(PIN_IRQ) {}
// **********************************************************************
void TouchScreenADS7843::vInitADS7843(unsigned char Bits,void (*fptr)(void)) {

    _PIN_CS=1;
    _PIN_SDI=0;
    _PIN_CLK=0;
    Resolution=Bits;
    if (fptr!=NULL) {
        ADS7843_Event_Touch.fall(fptr);
    }
}
// **********************************************************************
void TouchScreenADS7843::vSetResolution(unsigned char Bits) {
    Resolution=Bits;
}
// **********************************************************************
void TouchScreenADS7843::vEnableSetInterrupt(void (*fptr)(void)) {
    ADS7843_Event_Touch.fall(fptr);
}
// **********************************************************************
void TouchScreenADS7843::vDisableInterrupt(void) {
    ADS7843_Event_Touch.fall(NULL);
}
// **********************************************************************
bool TouchScreenADS7843::bReadPinIRQ(void) {
    return(_PIN_IRQ);
	
}
// **********************************************************************
UINT_COORDxy TouchScreenADS7843::ADS7843WriteAndReadData(unsigned char Data) {
    unsigned char k;
    UINT_COORDxy Temp;

    _PIN_CS=0;
    __delay_CLK_ADS7843();
    // En cada flanco ascendente se toma el dato:
    for (k=0;k<8;k++) {
        _PIN_SDI=!!(Data&0x80);
        __delay_CLK_ADS7843();
        _PIN_CLK=1;
        __delay_CLK_ADS7843();
        Data<<=1;
        _PIN_CLK=0;

    }
    __delay_CLK_ADS7843(); // Esperamos busy:
    _PIN_CLK=1;
    __delay_CLK_ADS7843();
    _PIN_CLK=0;
    __delay_CLK_ADS7843();
    __delay_CLK_ADS7843(); // Esperamos busy:

    // En flanco descendente leemos data desde el ADS7843:
    _PIN_CLK=1;
    Temp=0;

    if (Resolution==8) {
        for (k=0;k<8;k++) {
            Temp<<=1;
            _PIN_CLK=0;
            Temp+=_PIN_SDO;
            __delay_CLK_ADS7843();
            _PIN_CLK=1;
        }
    } else {
        for (k=0;k<12;k++) {
            Temp<<=1;
            _PIN_CLK=0;
            Temp+=_PIN_SDO;
            __delay_CLK_ADS7843();
            _PIN_CLK=1;
        }
    }
    __delay_CLK_ADS7843();
    _PIN_CLK=0;
    _PIN_CS=1;

    return(Temp);
}
// **********************************************************************
void TouchScreenADS7843::vTestTouchPanel(TOUCH_DATA *Data) {
    unsigned short TempX,TempY;
    unsigned char k;

    wait_ms(1);

    TempX=0;
    TempY=0;
    for (k=0;k<8;k++) {
        if (Resolution==8) {
            TempX+=ADS7843WriteAndReadData(ADS7843_READ_X_8);
            TempY+=ADS7843WriteAndReadData(ADS7843_READ_Y_8);
        } else {
            TempX+=ADS7843WriteAndReadData(ADS7843_READ_X_12);
            TempY+=ADS7843WriteAndReadData(ADS7843_READ_Y_12);
        }
    }


    Data->x=(TempX>>3);
    Data->y=(TempY>>3);

    return;
}
// **********************************************************************
