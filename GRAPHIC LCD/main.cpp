#define    ColorRed                0xf800
#define    ColorGreen            0x400
#define    ColorBlue            0x001f
#define    ColorMaron            0x8208
#define    ColorBlack            0x0
#define    ColorWhite            0xffff
#define    ColorMaroon            0x8000
#define    ColorFuchsia            0xf81f
#define    ColorViolet            0x801f
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
#include "mbed.h"
//#include "LCDTFT.h"
#include "LCDTFT.cpp"
//include "Fuentes.h"
#include "Touch.h"

/*
 
Serial pc(USBTX, USBRX);
int d1,d2,i,Lenght;
DigitalOut Led(LED1);
//DigitalOut Reset(p30);
//BusOut     MyBus(p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23,p24,p25,p26,p27,p28);
//LCDTFT     MyLCD(p5,p6,p7,p29,p30, &MyBus); // rd,wr,rs,cs,reset,bus
DigitalOut LCD_RS(p7);
DigitalOut LCD_RD(p5);
DigitalOut LCD_REST(p30);
DigitalOut LCD_CS(p29);
DigitalOut LCD_WR(p6);
BusInOut  LCD_PORT(p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23,p24,p25,p26,p27,p28);




void LCDTFTWriteCommand(unsigned short Data){   
    
    LCD_RS=0;
    LCD_CS=0;   
    LCD_PORT.output();
    LCD_PORT.write(Data);   
   
    //LCD_PORT = Data;   
    LCD_WR=0;
    //wait_ms(20);
    LCD_WR=1;
    //wait_ms(20);
    LCD_CS=1;   
//pc.printf("%d \n",Data);
}


void LCDTFTWriteData(unsigned short Data){
    LCD_RS=1;
    LCD_CS=0;   
    LCD_PORT.output();
    LCD_PORT.write(Data);   
    //LCD_PORT = Data;   
    LCD_WR=0;
    //wait_ms(10);
    LCD_WR=1;
    //wait_ms(10);
    LCD_CS =1;   
}


void LCD_WriteReg(unsigned short CMD,unsigned short Data){
    LCDTFTWriteCommand(CMD);
    LCDTFTWriteData(Data);
}

void LCD_Read(unsigned short index)
{
LCDTFTWriteCommand(index);
LCD_RS=1;
LCD_CS=0;
LCD_RD=0;
LCD_PORT.input();
LCD_RD=1;
d1 = LCD_PORT;
wait_ms(10);
LCD_RD=0;
LCD_PORT.input();
LCD_RD=1;
d2 = LCD_PORT;
LCD_CS=1;
//pc.printf("%d,%d ",d1,d2);
//pc.printf(" Done Reading ");

}


void address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
 LCD_WriteReg(0x0002,x1>>8);       // Column address start2
 LCD_WriteReg(0x0003,x1);       // Column address start1
 LCD_WriteReg(0x0004,x2>>8);       // Column address end2
 LCD_WriteReg(0x0005,x2);       // Column address end1
 LCD_WriteReg(0x0006,y1>>8);       // Row address start2
 LCD_WriteReg(0x0007,y1);       // Row address start1
 LCD_WriteReg(0x0008,y2>>8);       // Row address end2
 LCD_WriteReg(0x0009,y2);       // Row address end1
 LCDTFTWriteCommand (0x0022);
 
}

void Fill_Color(unsigned int color)
{

    int i,j;
    address_set(0,0,239,319);

    for(i=0;i<320;i++)
     {
      for (j=0;j<240;j++)
           {
         LCDTFTWriteData(color);
        }

      }        
}


 
 void Draw_Rectangle(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2,unsigned short Color){
   
          Lenght=((int)(x2-x1+1)*(y2-y1+1));  
       
        address_set(x1,y1,x2,y2);      
        for(int i=0;i<Lenght;i++){
        LCDTFTWriteData(Color);    
        }   
}









int main()
{
LCD_REST=0;
wait_ms(20);
LCD_REST=1;
wait_ms(20);
Led=1;

    LCD_Read(0x0022);
    LCD_WriteReg(0x0046,0x00A4);
    LCD_WriteReg(0x0047,0x0053);
    
    LCD_WriteReg(0x0048,0x0000);
    LCD_WriteReg(0x0049,0x0044);
    LCD_WriteReg(0x004a,0x0004);
    LCD_WriteReg(0x004b,0x0067);
    LCD_WriteReg(0x004c,0x0033);
    LCD_WriteReg(0x004d,0x0077);
    LCD_WriteReg(0x004e,0x0012);
    LCD_WriteReg(0x004f,0x004C);
    LCD_WriteReg(0x0050,0x0046);
    LCD_WriteReg(0x0051,0x0044);
     
      //240x320 window setting
    LCD_WriteReg(0x0002,0x0000); // Column address start2
    LCD_WriteReg(0x0003,0x0000); // Column address start1
    LCD_WriteReg(0x0004,0x0001); // Column address end2
    LCD_WriteReg(0x0005,0x00ef); // Column address end1
    LCD_WriteReg(0x0006,0x0000); // Row address start2
    LCD_WriteReg(0x0007,0x0000); // Row address start1
    LCD_WriteReg(0x0008,0x0001); // Row address end2
    LCD_WriteReg(0x0009,0x003f); // Row address end1
 
  // Display Setting
    LCD_WriteReg(0x0001,0x0006); // IDMON=0, INVON=1, NORON=1, PTLON=0
    LCD_WriteReg(0x0016,0x00C8); // MY=0, MX=0, MV=0, ML=1, BGR=0, TEON=0   0048
    LCD_WriteReg(0x0023,0x0095); // N_DC=1001 0101
    LCD_WriteReg(0x0024,0x0095); // PI_DC=1001 0101
    LCD_WriteReg(0x0025,0x00FF); // I_DC=1111 1111
   
    LCD_WriteReg(0x0027,0x0002); // N_BP=0000 0010
    LCD_WriteReg(0x0028,0x0002); // N_FP=0000 0010
    LCD_WriteReg(0x0029,0x0002); // PI_BP=0000 0010
    LCD_WriteReg(0x002a,0x0002); // PI_FP=0000 0010
    LCD_WriteReg(0x002C,0x0002); // I_BP=0000 0010
    LCD_WriteReg(0x002d,0x0002); // I_FP=0000 0010
   
    LCD_WriteReg(0x003a,0x0001); // N_RTN=0000, N_NW=001    0001
    LCD_WriteReg(0x003b,0x0000); // P_RTN=0000, P_NW=001
    LCD_WriteReg(0x003c,0x00f0); // I_RTN=1111, I_NW=000
    LCD_WriteReg(0x003d,0x0000); // DIV=00
    wait_ms(1);
    LCD_WriteReg(0x0035,0x0038); // EQS=38h
    LCD_WriteReg(0x0036,0x0078); // EQP=78h
    LCD_WriteReg(0x003E,0x0038); // SON=38h
    LCD_WriteReg(0x0040,0x000F); // GDON=0Fh
    LCD_WriteReg(0x0041,0x00F0); // GDOFF
   
    // Power Supply Setting
    LCD_WriteReg(0x0019,0x0049); // CADJ=0100, CUADJ=100, OSD_EN=1 ,60Hz
    LCD_WriteReg(0x0093,0x000F); // RADJ=1111, 100%
    wait_ms(1);
    LCD_WriteReg(0x0020,0x0040); // BT=0100
    LCD_WriteReg(0x001D,0x0007); // VC1=111   0007
    LCD_WriteReg(0x001E,0x0000); // VC3=000
    LCD_WriteReg(0x001F,0x0004); // VRH=0011
   
    //VCOM SETTING
    LCD_WriteReg(0x0044,0x004D); // VCM=101 0000  4D
    LCD_WriteReg(0x0045,0x000E); // VDV=1 0001   0011
    wait_ms(1);
    LCD_WriteReg(0x001C,0x0004); // AP=100
    wait_ms(2);
   
    LCD_WriteReg(0x001B,0x0018); // GASENB=0, PON=0, DK=1, XDK=0, VLCD_TRI=0, STB=0
    wait_ms(1);
    LCD_WriteReg(0x001B,0x0010); // GASENB=0, PON=1, DK=0, XDK=0, VLCD_TRI=0, STB=0
    wait_ms(1);
    LCD_WriteReg(0x0043,0x0080); //set VCOMG=1
    wait_ms(2);
   
    // Display ON Setting
    LCD_WriteReg(0x0090,0x007F); // SAP=0111 1111
    LCD_WriteReg(0x0026,0x0004); //GON=0, DTE=0, D=01
    wait_ms(1);
    LCD_WriteReg(0x0026,0x0024); //GON=1, DTE=0, D=01
    LCD_WriteReg(0x0026,0x002C); //GON=1, DTE=0, D=11
    wait_ms(1);
    LCD_WriteReg(0x0026,0x003C); //GON=1, DTE=1, D=11
   
    // INTERNAL REGISTER SETTING
    LCD_WriteReg(0x0057,0x0002); // TEST_Mode=1: into TEST mode
    LCD_WriteReg(0x0095,0x0001); // SET DISPLAY CLOCK AND PUMPING CLOCK TO SYNCHRONIZE
    LCD_WriteReg(0x0057,0x0000); // TEST_Mode=0: exit TEST mode
    //LCD_WriteReg(0x0021,0x0000);
    Fill_Color(ColorBlack); 
    while(1)
    {
   
    Draw_Rectangle(0,0,120,160, ColorRed);
    Draw_Rectangle(121,161 ,239,319,ColorBlue);
    Draw_Rectangle(0,161,120,319,ColorGreen);
    Draw_Rectangle(121,0,239,161,ColorYellow);
   
   // Fill_Color(ColorGreen);
    //wait_ms(400);
    //Fill_Color(ColorBlue);
    //wait_ms(400);
    }
}


 */

int m,n;
 #define TOUCH_X_MIN 20
#define TOUCH_X_MAX 230
#define TOUCH_Y_MIN 15
#define TOUCH_Y_MAX 320
// **********************************************************************
DigitalOut Led1(LED1);
DigitalOut Led2(LED2);
DigitalOut Led3(LED3);
DigitalOut Led4(LED4);

// **********************************************************************
TOUCH_DATA Coordinate;
volatile bool kbhit_irq=0;
TouchScreenADS7843 MyTouch(p9,p11,p12,p10,p8); // ADS7843-> SDO,SDI,SCK,CS,IRQ
int m1;
void vISRTouchIRQ(void)
{
    UINT_COORDxy Temp;
   	m1 = MyTouch.bReadPinIRQ();
	Led3= m1;
	wait_ms(250);
	Led3=0;

    //MyTouch.vTestTouchPanel(&Coordinate);
	 
    
    //if((Coordinate.x>TOUCH_X_MIN && Coordinate.x<TOUCH_X_MAX) && (Coordinate.y>TOUCH_Y_MIN && Coordinate.y<TOUCH_Y_MAX))
	{
        
      //  Temp=Coordinate.y;
       // Coordinate.y=((float)((255-TOUCH_X_MIN)-Coordinate.x)*(320.0/(TOUCH_X_MAX-TOUCH_X_MIN)));
       // Coordinate.x=((float)(Temp-TOUCH_Y_MIN)*(240.0/(TOUCH_Y_MAX-TOUCH_Y_MIN)));
       // Led4 = ~Led4;
	}
	  //MyTouch.vDisableInterrupt();
 kbhit_irq=1;
}


 BusInOut     MyBus(p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23,p24,p25,p26,p27,p28);
 LCDTFT     MyLCD(p5,p6,p7,p29,p30, &MyBus); // rd,wr,rs,cs,reset,bus
  int main(){
   int color =0x2000;
   
   Led1=1;
    MyTouch.vInitADS7843(USE_8BITS,&vISRTouchIRQ);

    Led1=0;
    Led2=0;

    MyLCD.vLCDTFTInit();
    wait_ms(300);
    
   MyLCD.vLCDTFTSetParametersPrintf(0,0,239,319,2,0x0101);
    //MyLCD.printf("Graphic LCD DEMO");
	MyLCD.vLCDTFTFillScreen(0x0000);
//	wait_ms(1000);
//	 MyLCD.vLCDTFTRectangle(0,0,200,150,1,ColorWhite);
	while(1)
	 { 
	 
	    
//	MyLCD.vDrawImageBMP24Bits("img1.bmp");
//    wait_ms(1000);
//	MyLCD.vDrawImageBMP24Bits("img2.bmp");
//	wait_ms(1000);
  //  MyLCD.vDrawImageBMP24Bits("img3.bmp");   
//	wait_ms(1000);
    
	
    	if (color>0xffff)
		{
		color = 0x0000;
		}
		
		Led2=!Led2;
		   if(kbhit_irq==1)
		   {
            Led1= 1;
			 MyLCD.vLCDTFTText(0,25,"  PRESS ME ",ARIAL,1,color+=1000); 
			 			 MyLCD.vLCDTFTText(0,60,"SHOBHIT ",ARIAL,3,ColorRed); 
MyLCD.vLCDTFTCircle( 120,80, 10,1,color+=10);
MyLCD.vLCDTFTCircle( 120,120,10,1,color+=50);
MyLCD.vLCDTFTCircle( 120,150,10,1,color+=500);
MyLCD.vLCDTFTCircle( 120,190,10,1,color+=230);
MyLCD.vLCDTFTCircle( 120,230,10,1,color+=70);
MyLCD.vLCDTFTCircle( 120,265,10,1,color+=120);
MyLCD.vLCDTFTCircle( 120,295,10,1,color+=20);
//MyLCD.vLCDTFTCircle( 120,308,10,1,color+=1200);


		     //   if((Coordinate.x<235 && Coordinate.x>40) && (Coordinate.y<320 && Coordinate.y>40))
			//	{
           
           kbhit_irq=0;
		  
		  MyTouch.vEnableSetInterrupt(&vISRTouchIRQ);
		 // }
        }
       
	   	
		 
  //    MyLCD.vLCDTFTText(20,50,"SHOBHIT ",ARIAL,2,ColorGreen);
//	  MyLCD.vLCDTFTText(35,80,"SHOBHIT ",ARIAL,4,ColorBlue);
   //  int x1[10]= {10,30,50,65,120,135,170,200,220};
	   
	    //wait_ms(150);
    }//  while(1);
	
}


