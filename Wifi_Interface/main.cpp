#include "WiflyInterface.h"
//#include "UDPsocket.h"
#include "SHTx/sht15.hpp"
#include "SHTx/i2c.hpp"
#include "mbed.h"

Serial pc(USBTX, USBRX);


DigitalOut green(p12);
DigitalOut red(p13);
DigitalOut blue(LED1);
DigitalOut blue1(LED4);
DigitalIn sw3(p5);
DigitalIn sw2(p6);
DigitalIn sw1(p7);
//DigitalOut P8(p8);
//DigitalOut P9 (p9);
DigitalOut P15(p15);
//DigitalOut P14(p14);




char out_msg[32];
char decode[127];
char strcallid[20];

int id=1,callid=230;
void message_code(int id,char *msg, int callid)
{
sprintf(out_msg,"ID:%d;MSG:%s;CALLID:%d;",id,msg,callid);
}


int message_decode(char buffer[255])
{
char *mdata;
int i=0;
int pos=0;
int ret1,ret2;


if((mdata=strstr(buffer,"MSG"))!=NULL)
	{
	for(i=4;i<strlen(mdata);i++)
	{ 
	if(mdata[i]==';')
    {
	pos=i;
	break;
	}
	}
for(i=4;i<pos;i++)
{
decode[i-4] = mdata[i];
}
decode[pos-4]='\0';
ret1=1;
}

if((mdata=strstr(buffer,"CALLID"))!=NULL)
	{
	for(i=7;i<strlen(mdata);i++)
	{ 
	if(mdata[i]==';')
    {
	pos=i;
	break;
	}
	}
for(i=7;i<pos;i++)
strcallid[i-7]=mdata[i];

strcallid[pos-7]='\0';
callid=atoi(strcallid);
ret2=1;
}
pc.printf("Decoded Messagr %s\n",decode);

return (ret1&ret2);

}





int main()
{

//const char* ECHO_SERVER_ADDRESS = "10.0.0.12";			//.18
//const char* ECHO_SERVER_ADDRESS = "192.168.0.116";			//.18
const char* ECHO_SERVER_ADDRESS = "128.2.131.11";			//.18
const int ECHO_SERVER_PORT = 18732;

	int loop1=0,loop2=0,loop3=0,loop4=0,error,n=0,ret1=0;
	char out_buffer[255] = "INIT";
	char in_buffer[255]="False";
	
 WiflyInterface wifly(p9, p10, p25, p26, "CMU"," ", NONE);
//WiflyInterface wifly(p9, p10, p25, p26, "TP-LINK_50BE0D","0250BE0D", WPA);
	blue=0;
	red=0;
	green=0;
	pc.printf("\tMbed Patient Band Emulator\n\n\n");
//P14=0;;
P15=0;
	
	
	 while(1)
	{													
	if(sw1==1)
	blue=1;
	if(sw1==0)
	green=1;
	if(sw2==0)
	red=1;
	if(sw2==1)
	blue1=1;
	} 
	
	


	
	error = wifly.init(); // use DHCP
    if(error<0) 
    { 
	pc.printf("Error in Initialization");
    }
    while (!wifly.connect()); // join the network
    printf("IP Address Obtained from Wireless Router %s\n\r", wifly.getIPAddress());
    
    UDPSocket sock, server;
    
	//sock.bind(8086);
	server.bind(8086);
	//Endpoint client;
	Endpoint echo_server;
    echo_server.set_address(ECHO_SERVER_ADDRESS, ECHO_SERVER_PORT);


//out_buffer = (char*)realloc(out_buffer,255*sizeof(char));
//in_buffer = (char*)realloc(in_buffer,255*sizeof(char));
//decode = (char*)realloc(decode,127*sizeof(char));

/*Starts here */
//server.bind(18732);
//sock.bind(8085);

if(-1==(sock.init()))
	printf("Sock Init Failed \n");

loop1=1;

int ret=0;

while(1)
{
loop1=1;
loop2=0;
loop3=0;
loop4=0;

	pc.printf("Inside LOOP 5\n");
	if(loop1)
	{
   	pc.printf("Inside LOOP 1\n");
	//out_buffer="INIT";
	strcpy(out_buffer,"INIT");
	message_code(id,out_buffer,callid);
	pc.printf(out_msg);
	if((ret=sock.sendTo(echo_server, out_msg, sizeof(out_msg)))==-1)
	     pc.printf("Failed to Send a Message\n");
    	 else
		 printf("Bytes Sent[%d]\n",ret);
	
	sock.set_blocking(false, 3500);
    n = sock.receiveFrom(echo_server, in_buffer, sizeof(in_buffer));
    //in_buffer[strlen(in_buffer)-1]='\0';
	if(n>0)
    {
     pc.printf("bytes read[%d],%s\n",n,in_buffer);
     if((ret1=message_decode(in_buffer))==1)
   		 if((strcmp(decode,"PINITACK")==0))
		     {
			 printf("INIT EQUAL\n"); loop1=0;
	loop2=1; 
	}
	
	}   

/* this loop takes care of call query by patient */
int loop21=1;
while(loop2)
{

pc.printf("Inside LOOP 2\n");


if(sw1==0)
{ 
while(loop21)
{
 blue=1;
strcpy(out_buffer,"CALL");
message_code(id,out_buffer,callid);
pc.printf(out_msg);
sock.sendTo(echo_server,out_msg,sizeof(out_msg));
sock.set_blocking(false, 1500);
    n = sock.receiveFrom(echo_server, in_buffer, sizeof(in_buffer));
    if(n>0)
    {
     pc.printf("bytes read[%d],%s\n",n,in_buffer);
     if((ret1=message_decode(in_buffer))==1)
   		 if((strcmp(decode,"PCALLACK")==0))
		 { printf("CALL EQUAL\n"); loop2=0;
	loop3=1;					  
	loop21=0;
	 }
	}   
   }
}

}

/* this loop takes care of the contact by nurse */

int loop31=1;
while(loop3)
{

if(sw2==0)
{
pc.printf("Inside LOOP 3\n");
//blue=0;
while(loop31)
{
strcpy(out_buffer,"CONTACT");
message_code(id,out_buffer,callid);
pc.printf(out_msg);
sock.sendTo(echo_server,out_msg,sizeof(out_msg));
sock.set_blocking(false, 1500);
    n = server.receiveFrom(echo_server, in_buffer, sizeof(in_buffer));
    if(n>0)
    {
     pc.printf("bytes read[%d],%s\n",n,in_buffer);
     if((ret1=message_decode(in_buffer))==1)
   		 if((strcmp(decode,"CALLSRVD")==0))
		 { loop2=0;
		 printf("CONTACT EQUAL\n");
		 	loop3=0; 
	loop4=1; loop31=0;		 }
	}   
   }
blue=0;
green=1;
}
}
/* This Loop is for feedback*/

int loop41=0;
while(loop4)
{
if(sw1==0)
{ strcpy(out_buffer,"posFB"); loop41=1; }
if(sw2==0)
{ strcpy(out_buffer,"negFB"); loop41=1; }
pc.printf("Inside LOOP 4\n");
while(loop41)
{ message_code(id,out_buffer,callid);
pc.printf(out_msg); sock.sendTo(echo_server,out_msg,sizeof(out_msg));
sock.set_blocking(false, 1500);
     n = sock.receiveFrom(echo_server, in_buffer, sizeof(in_buffer));
    if(n>0)
    { pc.printf("bytes read[%d],%s\n",n,in_buffer);
     if((ret1=message_decode(in_buffer))==1)
   		 if((strcmp(decode,"FBACK")==0))
		 { printf("FB EQUAL\n");//loop2=0;
	  loop3=0; loop4=0; loop41=0;	green=0; }
	}   
  } 
   } }
}
return 0;
}
