#include "mbed.h"
#include "WiflyInterface.h"
#include "UDPsocket.h"
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


 const char* ECHO_SERVER_ADDRESS = "10.0.0.2";			//.18
//const char* ECHO_SERVER_ADDRESS = "";
const int ECHO_SERVER_PORT = 8080;


WiflyInterface wifly(p9, p10, p25, p26, "HOME-2012","2A7FC553A4492C02", WPA);
//WiflyInterface wifly(p9, p10, p25, p26, "RPCS","12345678", WPA);

int main() {
   int error=1,mj=0;
    while(pc.scanf("%d",&mj)==0);
  // 	pc.printf("%d\n",mj);
 pc.printf("Mbed Wireless Temperature Sensor\n\n\n");
	error = wifly.init(); // use DHCP
    if(error<0) 
    { pc.printf("Error in Initialization");
    }
    while (!wifly.connect()); // join the network
   //pc.printf("it does work");
    printf("IP Address Obtained from Wireless Router %s\n\r", wifly.getIPAddress());
    
    UDPSocket sock;
    sock.init();
	Endpoint echo_server;
    echo_server.set_address(ECHO_SERVER_ADDRESS, ECHO_SERVER_PORT);
    //char out_buffer[] = "Hello World from MBED\n";
    //sock.sendTo(echo_server, out_buffer, sizeof(out_buffer));

															  while(1) {
    char out_buffer[] = "1 OK\n";
         
    sock.sendTo(echo_server, out_buffer, sizeof(out_buffer));
    wait(3);
     
	 }
}