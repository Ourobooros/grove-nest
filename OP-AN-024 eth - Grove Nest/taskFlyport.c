#include "taskFlyport.h"
#include "lib_grovesensors.h"

void FlyportTask()
{	
	// GROVE board
	void *board = new (Board);

	// GROVE devices
	// Digital Input
	void *waterSensor = new (Dig_io, IN);
	
	// Attach Devices
	attachSensorToBoard(board, waterSensor, DIG4);
	
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
		if(get(waterSensor))
		{
			IOPut(p21, ON);
		}
		else
		{
			IOPut(p21, OFF);
		}
	}
}