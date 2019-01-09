# include <stdio.h>
#include "rpi.h"

int main(int argc, char **argv)
{
	struct bcm2835_peripheral gpio = {GPIO_BASE};
	if(map_peripheral(&gpio) == -1)
	  {
	    printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
	    return -1;
	  }

	  // Define pin 7 as output
	  INP_GPIO(18);
	  OUT_GPIO(18);

	  for (int i=0; i < 10; i++)
	  {
	    // Toggle pin 7 (blink a led!)
		printf("On\n");
		GPIO_SET = 1 << 18;
	    sleep(1);
	    printf("Off\n");
	    GPIO_CLR = 1 << 18;
	    sleep(1);
	  }
}
