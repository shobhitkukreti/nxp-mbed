/*
  @file LCDTFT.h
  @version: 1.0
  
  @web www.micros-designs.com.ar
  @date 30/01/11
  
*- Version Log --------------------------------------------------------------*
*   Fecha       Autor                Comentarios                             *
*----------------------------------------------------------------------------*
* 30/01/11      Suky        Original                                         *
*----------------------------------------------------------------------------*/ 
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                                                                   ////
////        (C) Copyright 2011 www.micros-designs.com.ar               ////
//// Este código puede ser usado, modificado y distribuido libremente  ////
//// sin eliminar esta cabecera y  sin garantía de ningún tipo.        ////
////                                                                   ////
////                                                                   ////
/////////////////////////////////////////////////////////////////////////// 

/** Libreria para LCD TFT chip Himax HX8347-A
 *
 *
 * @code
 * #include "mbed.h"
 * #include "LCDTFT.h"
 * 
 *BusOut     MyBus(p13,p14,p15,p16,p17,p18,p19,p20,p30,p29,p28,p27,p26,p25,p24,p23);
 *LCDTFT     MyLCD(p5,p6,p7,p22,p21,&MyBus);
 *
 *int main(){
 *    MyLCD.vLCDTFTInit();
 *    MyLCD.vLCDTFTFillScreen(ColorWhite);
 *    MyLCD.printf("Hola mbed!!!");
 *
 *    MyLCD.vDrawImageBMP24Bits("IMG0.BMP");
 *
 *    while(1){  
 *    }
 *}
 * @endcode
 */

#include "mbed.h"
#include "Fuentes.h"

#define    ColorRed                0xf800
#define    ColorGreen            0x400
#define    ColorBlue            0x001f
#define    ColorMarron            0x8208
#define    ColorBlack            0x0
#define    ColorWhite            0xffff
#define    ColorMaroon            0x8000
#define    ColorFuchsia            0xf81f
#define    ColorVioleta            0x801f
#define    ColorAzulMarino        0x14
#define    ColorGray            0x8410
#define    ColorOlive            0x8400
#define    ColorNaranja            0xfc08
#define    ColorYellow            0xffe0
#define    ColorCian            0x87ff
#define    ColorRosado            0xf810
#define    ColorNavy            0x10
#define    ColorPurple            0x8010
#define    ColorTeal            0x410
#define    ColorLime            0x7e0
#define    ColorAqua            0x7ff

#define       LCD_X_MAX            240
#define       LCD_Y_MAX            320

class LCDTFT: public Stream {
    public:
         /** Crea LCDTFT interface
         *
         * @param RD    
         * @param WR     
         * @param RS
         * @param CS
         * @param RESET
         * @param BusLCD (16-bits) 
         */
        LCDTFT(PinName PIN_RD,PinName PIN_WR,PinName PIN_RS,PinName PIN_CS,PinName PIN_RESET, BusInOut *BUSLCD);
        /** Inicializa LCD
        *
        */
        void vLCDTFTInit(void);
        /** Fija parametros para escritura mediante printf
        *
        * @param Xo X inicial
        * @param Yo Y inicial
        * @param Xmin X minimo
        * @param Xmax X maximo
        * @param Alto Alto de letra (1,2,3..)
        * @param Color Color de letra 16-bits
        */
        void vLCDTFTSetParametersPrintf(unsigned short Xo,unsigned short Yo,unsigned short Xmin,unsigned short Xmax,unsigned char Alto, unsigned short Color);
        /** Pinta pantalla completa de color
        * @param Color Color de 16-bits
        */
        void vLCDTFTFillScreen(unsigned short Color);
        /** Dibuja punto en pantalla
        * @param x Posicion x
        * @param y Posicion y
        * @param Color Color 16-bits
        */
        void vLCDTFTPoint(unsigned short x,unsigned short y,unsigned short Color);
        /** Escribe string en LCD
        * @param x X inicial
        * @param y Y inicial
        * @param *PtrText Texto a escribir
        * @param *Fuente Fuente de letra a escribir (ARIAL)
        * @param Alto Alto de letra (1,2,3)
        * @param Color Color de 16-bits
        */
        void vLCDTFTText(unsigned short x,unsigned short y,const char *PtrText,const char (*Fuente)[5],unsigned char Alto,unsigned short Color);
        
        void vLCDTFTLine(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2,unsigned short Color);
        void vLCDTFTRectangle(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2,bool Filled,unsigned short Color);
        void vLCDTFTCircle(unsigned short x,unsigned short y,unsigned short Radius,bool Filled,unsigned short Color);
        /** Dibuja imagen ubicada en memoria de mbed (2Mbytes) centrada en patalla
        * @param NameImagen nombre de imagen (ejm: IMAGEN.BMP)
        */        
        void vDrawImageBMP24Bits(const char *NameImagen);
        void vLCDTFTDrawImage(unsigned short x,unsigned short y, unsigned short Width, unsigned short Heigh, unsigned int Lenght, const unsigned short *Imagen);
    #if DOXYGEN_ONLY
        /** Escribe un caracter en LCD
         *
         * @param c El caracter a escribir en LCD
         */
        int putc(int c);
    
        /** Escribe un string formateado en LCD
         *
         * @param format A printf-style format string, followed by the
         *               variables to use in formating the string.
         */
        int printf(const char* format, ...);
    #endif                

    private:
        DigitalOut LCD_PIN_RD,LCD_PIN_WR,LCD_PIN_RS,LCD_PIN_CS,LCD_PIN_RESET;    
        BusInOut *LCD_PORT;    
        unsigned short X,Y,X_min,X_max,_Alto,_Color;
        // Stream implementation functions
        virtual int _putc(int value);
        virtual int _getc();

        virtual void vLCDTFTWriteCommand(unsigned short Data);
        virtual void vLCDTFTWriteData(unsigned short Data);
        virtual void vLCDTFTWriteCommandData(unsigned short CMD,unsigned short Data);
        virtual void vLCDTFTAddressSet(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2);
        virtual void LCD_Read(unsigned short index);
		virtual void vLCDTFTAddressSetPoint(unsigned short x,unsigned short y);    
};
// **************************************************************************************************************************************
