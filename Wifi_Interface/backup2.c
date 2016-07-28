#include "mbed.h"
#include "WiflyInterface.h"
#include "UDPsocket.h"
#include "SHTx/sht15.hpp"
#include "SHTx/i2c.hpp"

SHTx::SHT15 sensor(p6, p7);

//DigitalOut myled(LED1);

DigitalIn in(p6);
DigitalOut led(LED1);

Serial pc(USBTX, USBRX);
 
/* wifly object where:
*     - p9 and p10 are for the serial communication
*     - p25 is for the reset pin
*     - p26 is for the connection status
*     - "mbed" is the ssid of the network
*     - "password" is the password
*     - WPA is the security
*/


//const char* ECHO_SERVER_ADDRESS = "10.0.0.12";			//.18
const char* ECHO_SERVER_ADDRESS = "192.168.0.101";			//.18
//const char* ECHO_SERVER_ADDRESS = "128.237.238.36";			//.18

const int ECHO_SERVER_PORT = 8085;
int loop1,loop2;
char *out_buffer = "1 OK";
char in_buffer[255]="";
char temp[10];
char sendt[12]={"1 "};;
char* out1= "6 1";
char* out2 ="6 0";
char *out_motion = "6 OK";
//WiflyInterface wifly(p9, p10, p25, p26, "HOME-2012","2A7FC553A4492C02", WPA);
WiflyInterface wifly(p9, p10, p25, p26, "TP-LINK_50BE0D","0250BE0D", WPA);
//WiflyInterface wifly(p9, p10, p25, p26, "TP-LINK_50BE0D"," ", NONE);
//WiflyInterface wifly(p9, p10, p25, p26, "AndroidAP"," ", NONE);
//WiflyInterface wifly(p9, p10, p25, p26, "CMU"," ", NONE);
int main() {
   int error=1,mj=0;

  
  pc.printf("Mbed Wireless Temperature Sensor\n\n\n");

//   pc.printf("Mbed Wireless Motion Sensor\n\n\n");

    sensor.setOTPReload(false);
    sensor.setResolution(true);
	sensor.setScale(true); 
	sensor.update();
	sprintf(temp,"%4.2f",sensor.getTemperature()); // get temperature value, format it
	pc.printf("temp [%s]\n",temp);
  
 	error = wifly.init(); // use DHCP
    if(error<0) 
    { 
	pc.printf("Error in Initialization");
    }
    while (!wifly.connect()); // join the network
    printf("IP Address Obtained from Wireless Router %s\n\r", wifly.getIPAddress());
    
    UDPSocket sock, server;
    sock.init();
	server.init();
	Endpoint client;
	Endpoint echo_server;
    echo_server.set_address(ECHO_SERVER_ADDRESS, ECHO_SERVER_PORT);
    //char out_buffer[] = "Hello World from MBED\n";
   // sock.sendTo(echo_server, out_buffer, sizeof(out_buffer));
    loop1 = 1; loop2=1;

		while(loop1)
		{
 
  pc.printf("again\n");       
  sock.sendTo(echo_server, out_buffer, sizeof(out_buffer));
   server.set_blocking(false, 1500);
 // sock.sendTo(echo_server, out_motion, sizeof(out_motion));
  int n = server.receiveFrom(client, in_buffer, sizeof(in_buffer));
  if(n>0)
    {
     pc.printf("bytes read[%d],%s\n",n,in_buffer);
    if((strcmp("ACK",in_buffer)==0)||(strcmp("Ack",in_buffer)==0))
     
	 { pc.printf("string equal\n");	
	 loop1=0;
	 }
	 else
	 { printf("String do not match\n");
	  
	 }
  
 }   

 pc.printf("Still in first loops\n");  
 }


while(loop2)
{
     sensor.update();
	 sprintf(temp,"%s %4.2f","1",sensor.getTemperature()); // get temperature value, format it
		
    out_buffer="ACK";
    int n = server.receiveFrom(client, in_buffer, sizeof(in_buffer));
    if(n>0)
   { 
    pc.printf("data bytes read[%d],%s \n",n,in_buffer);
    if((strcmp("ACK",in_buffer)==0)||(strcmp("Ack",in_buffer)==0))
    { 
	pc.printf("bytes read[%d],%s \n",n,in_buffer); 
	}
	
	else if((strcmp("CMD",in_buffer)==0)||(strcmp("cmd",in_buffer)==0)) 
	{ 
	pc.printf("temp in loop 2[%s]\n",temp);
	sock.sendTo(echo_server,temp,sizeof(temp));
 /*	if(	in==0)
    {sock.sendTo(echo_server, out1, sizeof(out1));
    pc.printf("Motion %s\n",out1);
	}
     else
     {
     pc.printf("Motion %s\n",out2);
	 sock.sendTo(echo_server, out2, sizeof(out2));
    } 
*/	
	}
    
}	
else printf("No Incoming Data\n ");
}
}