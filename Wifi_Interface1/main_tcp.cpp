#include "mbed.h"
#include "WiflyInterface.h"
//#include "SHT1x.h"
#include "SHTx/sht15.hpp"
#include "SHTx/i2c.hpp"

DigitalIn in(p6);
DigitalOut led(LED1);

SHTx::SHT15 sensor(p6, p7);


DigitalOut myled(LED1);

Serial pc(USBTX, USBRX);
 
/* wifly object where:
*     - p9 and p10 are for the serial communication
*     - p25 is for the reset pin
*     - p26 is for the connection status
*     - "mbed" is the ssid of the network
*     - "password" is the password
*     - WPA is the security
*/


const char* ECHO_SERVER_ADDRESS = "192.168.0.105";			//.18
//const char* ECHO_SERVER_ADDRESS = "10.0.0.12";			//.18

const int ECHO_SERVER_PORT = 8090;
char *out_buffer = "0 5001";
char temp[10];
char *in_buffer = NULL;
//WiflyInterface wifly(p9, p10, p25, p26, "HOME-2012","2A7FC553A4492C02", WPA);
WiflyInterface wifly(p9, p10, p25, p26, "TP-LINK_50BE0D","0250BE0D", WPA);
int main() {
   
    sensor.setOTPReload(false);
    sensor.setResolution(true);
	sensor.setScale(true); 
	sensor.update();
	sprintf(temp,"%4.2f",sensor.getTemperature()); // get temperature value, format it
	pc.printf("temp [%s]\n",temp);
   
   int error=1;
  //  while(pc.scanf("%d",&mj)==0);
  
     pc.printf("Mbed Wireless Temperature Sensor\n\n\n");
	 error = wifly.init(); // use DHCP
    if(error<0) 
    { pc.printf("Error in Initialization");
    }
    while (!wifly.connect()); // join the network
    printf("IP Address Obtained from Wireless Router %s\n\r", wifly.getIPAddress());
   

    TCPSocketServer server;
    server.bind(5001);	// Listen on this port no.
    server.listen();

   	   /* Mbed as a client to send the hello packet */
      
        TCPSocketConnection socket;
        while (socket.connect(ECHO_SERVER_ADDRESS, ECHO_SERVER_PORT) < 0) {
        printf("Unable to connect to (%s) on port (%d)\r\n", ECHO_SERVER_ADDRESS, ECHO_SERVER_PORT);
        wait(1);
    }
     // socket.set_blocking(false, 1500);
	
	// Sending Hello World Packet

	 int len = strlen(out_buffer);
     int j=  socket.send_all(out_buffer, len);
	 pc.printf("no. of bytes sent [%d]\n",j);
	 socket.close();
	 
	 /* Work over as client, now it is a sever */
	  

    
    float h,t;
	   
  int n=0;
  char buffer[255]="";
  sensor.setScale(true);  // Setting up Temperature Sensor
  
  
    wait_ms(20);
    
	TCPSocketConnection client;
   // client.set_blocking(false, 1500); // Timeout after (1.5)s	  
	 
	 
 // Infinite loop. Updates the awc whenever it connects with the mbed server.
	
	  while (true) 
	 {
     sensor.update();
	 sprintf(temp,"%4.2f",sensor.getTemperature()); // get temperature value, format it
	 printf("\nWait for new connection...\n");
     server.accept(client);
	 wait_ms(200);	   	  
	 pc.printf("temp [%s]\n",temp);	// debug data to PC over USB
		    
      //  printf("Connection from: %s\n", client.get_address());
             	
			while(true)	
				// Another while loop for testing. We will break 
				//from it after reading it once and close the connection.
				   {
			n = client.receive(buffer, sizeof(buffer));
           	if(n>0)	// If some data is received.
		 	{
			 buffer[n+1]='\0';
			 pc.printf("Byte received[%d] %s\n",n,buffer);
             len= strlen(temp);
			 j = client.send_all(temp, len);
	    	 printf("bytes send [%d] \n",j);
			 wait_ms(100);
		     client.close();
	  	     printf("breaking \n");
			 break;
	        }
			else // It will never come to else
			{ pc.printf("No data\n");	break; }
				}
 	 
    } 
	  }
	
	 /*// For motion sensor

char* out1= "ON";
char* out2 ="OFF";
while(true)
{
pc.printf("Waiting for a new connection\n");
server.accept(client);
n = client.receive(buffer, sizeof(buffer));
if(	in==0)
{ client.send_all(out1, sizeof(out1));
 client.close();
 }
else
{
client.send_all(out2,sizeof(out2));
client.close();
}
}	}   */



	
	





