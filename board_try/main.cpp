#include "mbed.h"

DigitalIn out(p6);
DigitalOut led(LED1);
int main() {
/*   
	while(1) {
      
      wait_ms(1000);
      led=1;
      wait_ms(1000);
      led=0;
			wait_ms(1000);
    }
*/
asm("ldr r0,=0x10");
	}
