/*
  @file LCDTFT.cpp
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


#include "LCDTFT.h"

LCDTFT::LCDTFT(PinName PIN_RD,PinName PIN_WR,PinName PIN_RS,PinName PIN_CS,PinName PIN_RESET, BusInOut *BUSLCD)
    : LCD_PIN_RD(PIN_RD),LCD_PIN_WR(PIN_WR),LCD_PIN_RS(PIN_RS),LCD_PIN_CS(PIN_CS),LCD_PIN_RESET(PIN_RESET){
    LCD_PORT=BUSLCD;
    X=0;
    Y=0;
    X_min=0;
    X_max=LCD_X_MAX;
    _Alto=1;
    _Color=0x0000;
}

void LCDTFT::vLCDTFTSetParametersPrintf(unsigned short Xo,unsigned short Yo,unsigned short Xmin,unsigned short Xmax,unsigned char Alto, unsigned short Color){

    X=Xo;
    Y=Yo;
    X_min=Xmin;
    X_max=Xmax;
    _Alto=Alto;
    _Color=Color;
}

int LCDTFT::_putc(int value){
    char Fmt[2]={value,0};

    if(value=='\n'){
        X=X_min;
        Y+=7*_Alto + 1;
    }else{
        vLCDTFTText(X,Y,(const char *)&Fmt[0],&ARIAL[0],_Alto,_Color);
        X+=5*_Alto+1;
        if(X >= X_max){
            X = X_min;                           
            Y += 7*_Alto + 1;                
        }
    }
    return(value);
}

int LCDTFT::_getc(){
    return(-1);
}

void LCDTFT::vLCDTFTWriteCommand(unsigned short Data){   
    
    LCD_PIN_RS=0;
    LCD_PIN_CS=0;   
    LCD_PORT->output();
	LCD_PORT->write(Data);   
    LCD_PIN_WR=0;
    LCD_PIN_WR=1;
    LCD_PIN_CS=1;   
}

void LCDTFT::vLCDTFTWriteData(unsigned short Data){
    LCD_PIN_RS=1;
    LCD_PIN_CS=0;   
    LCD_PORT->output();
	LCD_PORT->write(Data);   
    
	LCD_PIN_WR=0;
    LCD_PIN_WR=1;
    LCD_PIN_CS =1;   
}


void LCDTFT::LCD_Read(unsigned short index)
{
int d1,d2;
vLCDTFTWriteCommand(index);
LCD_PIN_RS=1;
LCD_PIN_CS=0;
LCD_PIN_RD=0;
LCD_PORT->input();																				   
LCD_PIN_RD=1;
d1 = LCD_PORT->read();	  // 

wait_ms(10);
LCD_PIN_RD=0;
LCD_PORT->input();
LCD_PIN_RD=1;
d2= LCD_PORT->read();
LCD_PIN_CS=1;

}





void LCDTFT::vLCDTFTWriteCommandData(unsigned short CMD,unsigned short Data){
    vLCDTFTWriteCommand(CMD);
    vLCDTFTWriteData(Data);
}

void LCDTFT::vLCDTFTAddressSet(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2){ 

   
 vLCDTFTWriteCommandData(0x0002,x1>>8);       // Column address start2
 vLCDTFTWriteCommandData(0x0003,x1);       // Column address start1
 vLCDTFTWriteCommandData(0x0004,x2>>8);       // Column address end2
 vLCDTFTWriteCommandData(0x0005,x2);       // Column address end1
 vLCDTFTWriteCommandData(0x0006,y1>>8);       // Row address start2
 vLCDTFTWriteCommandData(0x0007,y1);       // Row address start1
 vLCDTFTWriteCommandData(0x0008,y2>>8);       // Row address end2
 vLCDTFTWriteCommandData(0x0009,y2);       // Row address end1
 vLCDTFTWriteCommand (0x0022);
   
   /*  vLCDTFTWriteCommandData(0x0044,(x2<<8)+x1);
    vLCDTFTWriteCommandData(0x0045,y1);
    vLCDTFTWriteCommandData(0x0046,y2);
    vLCDTFTWriteCommandData(0x004e,x1);
    vLCDTFTWriteCommandData(0x004f,y1);  */
   // vLCDTFTWriteCommand(0x0022);       
}  

void LCDTFT::vLCDTFTAddressSetPoint(unsigned short x,unsigned short y){ 

    //vLCDTFTWriteCommandData(0x004e,x);
    //vLCDTFTWriteCommandData(0x004f,y);
    
 vLCDTFTWriteCommandData(0x0002,x>>8);       // Column address start2
 vLCDTFTWriteCommandData(0x0003,x);       // Column address start1
 vLCDTFTWriteCommandData(0x0004,x>>8);       // Column address end2
 vLCDTFTWriteCommandData(0x0005,x);       // Column address end1
 vLCDTFTWriteCommandData(0x0006,y>>8);       // Row address start2
 vLCDTFTWriteCommandData(0x0007,y);       // Row address start1
 vLCDTFTWriteCommandData(0x0008,y>>8);       // Row address end2
 vLCDTFTWriteCommandData(0x0009,y);       // Row address end1
 
	
	vLCDTFTWriteCommand(0x0022);       
}  

void LCDTFT::vLCDTFTInit(void){

    LCD_PIN_RESET=1;
    wait_ms(5);   
    LCD_PIN_RESET=0;
    wait_ms(10);
    LCD_PIN_RESET=1;
    LCD_PIN_CS=1;
    LCD_PIN_RD=1;
    LCD_PIN_WR=1;
    wait_ms(20);

    vLCDTFTWriteCommandData(0x0046,0x00A4);    wait_ms(1);
    LCD_Read(0x0046);
	vLCDTFTWriteCommandData(0x0047, 0x0053);
	vLCDTFTWriteCommandData(0x0048,0x0000);
    vLCDTFTWriteCommandData(0x0049,0x0044);
    vLCDTFTWriteCommandData(0x004a,0x0004);
    vLCDTFTWriteCommandData(0x004b,0x0067);
    vLCDTFTWriteCommandData(0x004c,0x0033);
    vLCDTFTWriteCommandData(0x004d,0x0077);
    vLCDTFTWriteCommandData(0x004e,0x0012);
    vLCDTFTWriteCommandData(0x004f,0x004C);

vLCDTFTWriteCommandData(0x0002,0x0000); // Column address start2
    vLCDTFTWriteCommandData(0x0003,0x0000); // Column address start1
    vLCDTFTWriteCommandData(0x0004,0x0001); // Column address end2
    vLCDTFTWriteCommandData(0x0005,0x00ef); // Column address end1
    vLCDTFTWriteCommandData(0x0006,0x0000); // Row address start2
    vLCDTFTWriteCommandData(0x0007,0x0000); // Row address start1
    vLCDTFTWriteCommandData(0x0008,0x0001); // Row address end2
    vLCDTFTWriteCommandData(0x0009,0x003f); // Row address end1
 
  // Display Setting
    vLCDTFTWriteCommandData(0x0001,0x0006); // IDMON=0, INVON=1, NORON=1, PTLON=0
    vLCDTFTWriteCommandData(0x0016,0x00C8); // MY=0, MX=0, MV=0, ML=1, BGR=0, TEON=0   0048
    vLCDTFTWriteCommandData(0x0023,0x0095); // N_DC=1001 0101
    vLCDTFTWriteCommandData(0x0024,0x0095); // PI_DC=1001 0101
    vLCDTFTWriteCommandData(0x0025,0x00FF); // I_DC=1111 1111
   
    vLCDTFTWriteCommandData(0x0027,0x0002); // N_BP=0000 0010
    vLCDTFTWriteCommandData(0x0028,0x0002); // N_FP=0000 0010
    vLCDTFTWriteCommandData(0x0029,0x0002); // PI_BP=0000 0010
    vLCDTFTWriteCommandData(0x002a,0x0002); // PI_FP=0000 0010
    vLCDTFTWriteCommandData(0x002C,0x0002); // I_BP=0000 0010
    vLCDTFTWriteCommandData(0x002d,0x0002); // I_FP=0000 0010
   
    vLCDTFTWriteCommandData(0x003a,0x0001); // N_RTN=0000, N_NW=001    0001
    vLCDTFTWriteCommandData(0x003b,0x0000); // P_RTN=0000, P_NW=001
    vLCDTFTWriteCommandData(0x003c,0x00f0); // I_RTN=1111, I_NW=000
    vLCDTFTWriteCommandData(0x003d,0x0000); // DIV=00
    wait_ms(1);
    vLCDTFTWriteCommandData(0x0035,0x0038); // EQS=38h
    vLCDTFTWriteCommandData(0x0036,0x0078); // EQP=78h
    vLCDTFTWriteCommandData(0x003E,0x0038); // SON=38h
    vLCDTFTWriteCommandData(0x0040,0x000F); // GDON=0Fh
    vLCDTFTWriteCommandData(0x0041,0x00F0); // GDOFF
   
    // Power Supply Setting
    vLCDTFTWriteCommandData(0x0019,0x0049); // CADJ=0100, CUADJ=100, OSD_EN=1 ,60Hz
    vLCDTFTWriteCommandData(0x0093,0x000F); // RADJ=1111, 100%
    wait_ms(1);
    vLCDTFTWriteCommandData(0x0020,0x0040); // BT=0100
    vLCDTFTWriteCommandData(0x001D,0x0007); // VC1=111   0007
    vLCDTFTWriteCommandData(0x001E,0x0000); // VC3=000
    vLCDTFTWriteCommandData(0x001F,0x0004); // VRH=0011
   
    //VCOM SETTING
    vLCDTFTWriteCommandData(0x0044,0x004D); // VCM=101 0000  4D
    vLCDTFTWriteCommandData(0x0045,0x000E); // VDV=1 0001   0011
    wait_ms(1);
    vLCDTFTWriteCommandData(0x001C,0x0004); // AP=100
    wait_ms(2);
   
    vLCDTFTWriteCommandData(0x001B,0x0018); // GASENB=0, PON=0, DK=1, XDK=0, VLCD_TRI=0, STB=0
    wait_ms(1);
    vLCDTFTWriteCommandData(0x001B,0x0010); // GASENB=0, PON=1, DK=0, XDK=0, VLCD_TRI=0, STB=0
    wait_ms(1);
    vLCDTFTWriteCommandData(0x0043,0x0080); //set VCOMG=1
    wait_ms(2);
   
    // Display ON Setting
    vLCDTFTWriteCommandData(0x0090,0x007F); // SAP=0111 1111
    vLCDTFTWriteCommandData(0x0026,0x0004); //GON=0, DTE=0, D=01
    wait_ms(1);
    vLCDTFTWriteCommandData(0x0026,0x0024); //GON=1, DTE=0, D=01
    vLCDTFTWriteCommandData(0x0026,0x002C); //GON=1, DTE=0, D=11
    wait_ms(1);
    vLCDTFTWriteCommandData(0x0026,0x003C); //GON=1, DTE=1, D=11
   
    // INTERNAL REGISTER SETTING
    vLCDTFTWriteCommandData(0x0057,0x0002); // TEST_Mode=1: into TEST mode
    vLCDTFTWriteCommandData(0x0095,0x0001); // SET DISPLAY CLOCK AND PUMPING CLOCK TO SYNCHRONIZE
    vLCDTFTWriteCommandData(0x0057,0x0000); // TEST_Mode=0: exit TEST mode
    //LCD_WriteReg(0x0021,0x0000);    
	



	/*  vLCDTFTWriteCommandData(0x0050,0x0046);
    vLCDTFTWriteCommandData(0x0051,0x0044); 
	vLCDTFTWriteCommandData(0x0000,0x0001);    wait_ms(1);  
    vLCDTFTWriteCommandData(0x0003,0xA8A4);    wait_ms(1);  
    vLCDTFTWriteCommandData(0x000C,0x0000);    wait_ms(1);   
    vLCDTFTWriteCommandData(0x000D,0x080C);    wait_ms(1);   
    vLCDTFTWriteCommandData(0x000E,0x2B00);    wait_ms(1);   
    vLCDTFTWriteCommandData(0x001E,0x00B0);    wait_ms(1);   
    vLCDTFTWriteCommandData(0x0001,0x2B3F);    wait_ms(1);   
    vLCDTFTWriteCommandData(0x0002,0x0600);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0010,0x0000);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0011,0x6070);    wait_ms(1);        
    vLCDTFTWriteCommandData(0x0005,0x0000);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0006,0x0000);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0016,0xEF1C);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0017,0x0003);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0007,0x0233);    wait_ms(1);          
    vLCDTFTWriteCommandData(0x000B,0x0000);    wait_ms(1);
    vLCDTFTWriteCommandData(0x000F,0x0000);    wait_ms(1);        
    vLCDTFTWriteCommandData(0x0041,0x0000);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0042,0x0000);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0048,0x0000);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0049,0x013F);    wait_ms(1);
    vLCDTFTWriteCommandData(0x004A,0x0000);    wait_ms(1);
    vLCDTFTWriteCommandData(0x004B,0x0000);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0044,0xEF00);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0045,0x0000);    wait_ms(1);
       vLCDTFTWriteCommandData(0x0030,0x0707);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0031,0x0204);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0032,0x0204);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0033,0x0502);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0034,0x0507);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0035,0x0204);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0036,0x0204);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0037,0x0502);    wait_ms(1);
    vLCDTFTWriteCommandData(0x003A,0x0302);    wait_ms(1);
    vLCDTFTWriteCommandData(0x003B,0x0302);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0023,0x0000);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0024,0x0000);    wait_ms(1);
    vLCDTFTWriteCommandData(0x0025,0x8000);    wait_ms(1);
   // vLCDTFTWriteCommandData(0x004f,0);        
  //  vLCDTFTWriteCommandData(0x004e,0);        
    vLCDTFTWriteCommand(0x0022);			   */
}

void LCDTFT::vLCDTFTFillScreen(unsigned short Color){
    unsigned short i,j;
    vLCDTFTAddressSet(0,0,239,319);

    for(i=0;i<320;i++){
        for (j=0;j<240;j++){
            vLCDTFTWriteData(Color);
        }
    }      
}

void LCDTFT::vLCDTFTPoint(unsigned short x,unsigned short y,unsigned short Color){

    vLCDTFTAddressSetPoint(x,y);
    vLCDTFTWriteData(Color);
}
 
void LCDTFT::vLCDTFTText(unsigned short x,unsigned short y,const char *PtrText,const char (*Fuente)[5],unsigned char Alto,unsigned short Color){
   unsigned short i, j, k, l, m, temp;           
   char DataPunto[5]; 
   const char *Ptr;
    
   while(*PtrText!='\0'){
      Ptr=(Fuente+*PtrText-' ')[0];
      for(i=0;i<5;i++){DataPunto[i]=*Ptr++;}
      switch(*PtrText){
         case '\n':
            y += 7*Alto + 1;
         break;
         case '\r':
            x = 0;
         break;
         default:
            if(x+5*Alto >= LCD_X_MAX){
                 x = 0;                           
                 y += 7*Alto + 1;                
              }
              for(j=0; j<5; ++j, x+=Alto){
                 for(k=0; k < 7; k++){
                    temp=(0x01<<k);
                    if((DataPunto[j]&temp)==temp){
                       for(l=0; l < Alto; ++l){                         
                          for(m=0; m < Alto; ++m){
                             vLCDTFTPoint(x+m,y+k*Alto+l,Color);
                          }
                       }
                    }
                 }
              }
              x++;
          break;
      } 
      *PtrText++;     
   }
}

void LCDTFT::vLCDTFTLine(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2,unsigned short Color){

   unsigned short dy, dx;
   short addx=1, addy=1;
   short P, diff;
   unsigned short i=0;

   diff=((short)x2-x1);
   if(diff<0) diff*=-1;
   dx=diff;
   diff=((short)y2-y1);
   if(diff<0) diff*=-1;
   dy=diff;


   if(x1 > x2)addx = -1;
   if(y1 > y2)addy = -1;
   if(dx >= dy){
      dy *= 2;
      P = dy - dx;
      diff = P - dx;
      for(;i<=dx;++i){
         vLCDTFTPoint(x1, y1, Color);
         if(P < 0){
            P  += dy;
            x1 += addx;
         }else{
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }else{
      dx *= 2;
      P = dx - dy;
      diff = P - dy;
      for(; i<=dy; ++i){
         vLCDTFTPoint(x1, y1, Color);
         if(P < 0){
            P  += dx;
            y1 += addy;
         }else{
            P  += diff;
            x1 += addx;
            y1 += addy;
         }
      }
   }
}


void LCDTFT::vLCDTFTRectangle(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2,bool Filled,unsigned short Color){
   
    if(Filled){
        int Lenght=((int)(x2-x1+1)*(y2-y1+1));  
       
        vLCDTFTAddressSet(x1,y1,x2,y2);      
        for(int i=0;i<Lenght;i++){
            vLCDTFTWriteData(Color);                         
        }   
    }else{
        vLCDTFTLine(x1, y1, x2, y1, Color);     
        vLCDTFTLine(x1, y2, x2, y2, Color);
        vLCDTFTLine(x1, y1, x1, y2, Color);
        vLCDTFTLine(x2, y1, x2, y2, Color);
    }
}


void LCDTFT::vLCDTFTCircle(unsigned short x,unsigned short y,unsigned short Radius,bool Filled,unsigned short Color){
    short  a, b, P;

    a = 0;
    b = Radius;
    P = 1 - Radius;

    do{
        if(Filled){
            vLCDTFTLine(x-a, y+b, x+a, y+b, Color);
            vLCDTFTLine(x-a, y-b, x+a, y-b, Color);
            vLCDTFTLine(x-b, y+a, x+b, y+a, Color);
            vLCDTFTLine(x-b, y-a, x+b, y-a, Color);
        }else{
            vLCDTFTPoint(a+x, b+y, Color);
            vLCDTFTPoint(b+x, a+y, Color);
            vLCDTFTPoint(x-a, b+y, Color);
            vLCDTFTPoint(x-b, a+y, Color);
            vLCDTFTPoint(b+x, y-a, Color);
            vLCDTFTPoint(a+x, y-b, Color);
            vLCDTFTPoint(x-a, y-b, Color);
            vLCDTFTPoint(x-b, y-a, Color);
        }
        if(P < 0) P += 3 + 2 * a++;
        else P += 5 + 2 * (a++ - b--);
    }while(a <= b);
}

void LCDTFT::vDrawImageBMP24Bits(const char *NameImagen){

    #define OffsetWidthPixel    18
    #define OffsetHeighPixel    22
    #define OffsetSizeFile      34
    #define OffsetInitData      10
    #define OffserTipeFile      28
    char Nombre[80],k;
    unsigned short PosXImagen,PosYImagen;
    unsigned char Temp,BufferHeader[54],BufferTemp[3],Adicional;
    unsigned int WidthPixel,HeighPixel;
    unsigned short TipeFile,InitData,Temp16bits;
    union{
        unsigned short Val;
        struct{
            unsigned Blue:5;
            unsigned Green:6;
            unsigned Red:5;
        };
    }Color; 

    LocalFileSystem local("mbedMemory");
    sprintf(&Nombre[0],"/mbedMemory/");
    k=12;
    while(*NameImagen!='\0'){
        Nombre[k++]=*NameImagen++;
    }
    
    FILE *Imagen = fopen((const char *)&Nombre[0], "r"); // Abrimos archivo para lectura.
    // Si no se pudo encontrar archivo:
    if(!Imagen) {
        vLCDTFTFillScreen(ColorBlack);
        return;
    }
    // Leemos cabecera de archivo
    fread(&BufferHeader[0],1,54,Imagen);
    
    WidthPixel = ((unsigned int)BufferHeader[OffsetWidthPixel + 3]*16777216+BufferHeader[OffsetWidthPixel + 2]*65536+BufferHeader[OffsetWidthPixel + 1]*256+BufferHeader[OffsetWidthPixel]);
    HeighPixel = ((unsigned int)BufferHeader[OffsetHeighPixel + 3]*16777216+BufferHeader[OffsetHeighPixel + 2]*65536+BufferHeader[OffsetHeighPixel + 1]*256+BufferHeader[OffsetHeighPixel]);
    InitData = ((unsigned short)BufferHeader[OffsetInitData]);
    TipeFile = ((unsigned short)BufferHeader[OffserTipeFile + 1]*256 + BufferHeader[OffserTipeFile]);
    
    if((WidthPixel>LCD_X_MAX) || (HeighPixel>LCD_Y_MAX) || (TipeFile!=24)){
        fclose(Imagen);
        return;
    }
    
    if(InitData!=54){
        for(int k=54;k<InitData;k++){
            fread(&Temp,1,1,Imagen);
        }
    }
    
    PosXImagen=(LCD_X_MAX/2)-(WidthPixel/2);
    PosYImagen=(LCD_Y_MAX/2)+(HeighPixel/2);
    
    Temp16bits=WidthPixel*3;
    Adicional=0;
    while(((Temp16bits)%4)!=0){
        Adicional++;
        Temp16bits++;
    }
    
    for(int k=0,y=PosYImagen;k<HeighPixel;k++,y--){
        vLCDTFTAddressSet(PosXImagen,y,PosXImagen+WidthPixel-1,y);
        for(int i=0;i<WidthPixel;i++){
            fread(&BufferTemp[0],1,3,Imagen); // Leemos 3 bytes (R,G,B)
            Color.Blue=BufferTemp[0]>>3;
			Color.Green=BufferTemp[1]>>2;
			Color.Red=BufferTemp[2]>>3; // Conversion de 24-bits a 16-bits.-
            vLCDTFTWriteData(Color.Val);
        }
        // Bytes adicionales para que linea sea multiplo de 4.-
        for(int p=0;p<Adicional;p++){
            fread(&Temp,1,1,Imagen);
        }
    }
    fclose(Imagen);
}

void LCDTFT::vLCDTFTDrawImage(unsigned short x,unsigned short y, unsigned short Width, unsigned short Heigh, unsigned int Lenght, const unsigned short *Imagen){

    vLCDTFTAddressSet(x,y,x+Width-1,y+Heigh-1);      
    for(int i=0;i<Lenght;i++){
        vLCDTFTWriteData(*Imagen++);                         
    }
}