#include "taskFlyport.h"
#include "lib_grovesensors.h"

void FlyportTask()
{	
	// GROVE devices
	void *board = new (Board);
	
	// Digital Output
	void *led1 = new (Dig_io, OUT);
	void *pwmLed1 = new (Dig_io, PWM, 1);
	
	// Digital Input
	void *button1 = new (Dig_io, IN);
	
	// Analog Input
	void *analogIn1 = new (An_i);
	
	// Attach Devices
	attachSensorToBoard(board, led1, DIG1);
	attachSensorToBoard(board, pwmLed1, DIG2);
	attachSensorToBoard(board, button1, DIG3);
	attachSensorToBoard(board, analogIn1, AN1);
	
	// Configure PWM
	set(pwmLed1, FREQUENCY, 220.50);
	set(pwmLed1, DUTY, 35.20);
	set(pwmLed1, ON);
	
	// Connection to Network
	WFConnect(WF_DEFAULT);
	while (WFStatus != CONNECTED);
	UARTWrite(1,"Flyport connected... hello world!\r\n");
	vTaskDelay(200);
	
	float anVal = 0.0;
	
	while(1)
	{
		anVal = get(analogIn1);
		
		set(pwmLed1, DUTY, ((anVal * 100)/1023));
		
		if( get(button1) == 0)
		{
			set(led1, OFF);
		}
		else
		{
			set(led1, ON);
		}
	}
}
