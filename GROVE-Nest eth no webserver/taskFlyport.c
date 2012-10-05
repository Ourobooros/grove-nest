#include "taskFlyport.h"
#include "grovelib.h"

void FlyportTask()
{	
	vTaskDelay(100);
	UARTWrite(1,"Welcome to GROVE NEST example!\r\n");

	// GROVE board
	void *board = new(GroveNest);

	// GROVE devices	
	// Digital Input
	void *button = new(Dig_io, IN);
	attachToBoard(board, button, DIG1);
	
	// Connection to Network
	#if defined (FLYPORT)
	WFConnect(WF_DEFAULT);
	while (WFStatus != CONNECTED);
	#endif
	#if defined (FLYPORTETH)
	while(!MACLinked);
	#endif
	UARTWrite(1,"Flyport connected... hello world!\r\n");
	vTaskDelay(200);
	
	while(1)
	{
		// Check pressure of button
		if(get(button) != 0)
		{
			UARTWrite(1, "button pressed!\r\n");
			vTaskDelay(20);
		}
	}
}
