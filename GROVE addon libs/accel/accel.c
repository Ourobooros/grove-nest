/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        accel.c
 *  Module:          FlyPort WI-FI - FlyPort ETH
 *  Compiler:        Microchip C30 v3.12 or higher
 *
 *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Vicca Davide	     1.0     2/09/2012		   First release  
 *  
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  Software License Agreement
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  This is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License (version 2) as published by 
 *  the Free Software Foundation AND MODIFIED BY OpenPicus team.
 *  
 *  ***NOTE*** The exception to the GPL is included to allow you to distribute
 *  a combined work that includes OpenPicus code without being obliged to 
 *  provide the source code for proprietary components outside of the OpenPicus
 *  code. 
 *  OpenPicus software is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details. 
 * 
 * 
 * Warranty
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * WE ARE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 **************************************************************************/

#include "taskFlyport.h"
#include "grovelib.h"
#include "accel.h"


struct Interface *attachSensorToI2CBus(void *,int,int);
int set_register(BYTE,BYTE, BYTE);
BYTE read_register(BYTE,BYTE);



struct Accel
{
	const void *class;
	struct Interface *inter;
	BYTE devaddress; 
};


static int Accel_read(void * _dev,BYTE reg)
{
	struct Accel *dev = _dev;
	BYTE data = 0;
	Delay10us(1);
	data =	read_register(dev->devaddress, reg);
	return data;	
}	


static int init(struct Accel *self)
{
	if(set_register(self->devaddress,MMA7660_MODE,0x00))//Reset Mode register
		return -1;
	if(set_register(self->devaddress,MMA7660_SR,0x07))
		return -1;
	return set_register(self->devaddress,MMA7660_MODE,0x01);
}




static void *Accel_ctor (void * _self, va_list *app)
{
	struct Accel *self = _self;
	self->devaddress =  va_arg(*app, const BYTE);
	self->inter = NULL;
	return self;
}

static void Accel_dtor (void * _sensor)
{
}


static void *Accel_attach (void * _board,void *_sensor,int ic2bus)
{
	struct Accel *sensor = _sensor;
	int add = sensor->devaddress;
	sensor->inter = attachSensorToI2CBus(_board,ic2bus,add);
	return sensor->inter;
}


static int Accel_config (void * _self, va_list *app)
{
	struct Accel *self = _self;
	return init(self);	
}


static float Accel_get (void * _self, va_list *app)
{
	struct Accel *self = _self;
	char data = 64;
	BYTE register_ =  va_arg(*app, const BYTE);
	unsigned int y = TIMEOUT;
	while(((data>>6) == 1) && y)
	{
		y--;
		data = Accel_read(self,register_);
	}
	if(!y)
	{//Time out occurred
		flag = 1;
		return -1;
	}		
	if (data & 0x20)
		data = data | 0xC0;
	else
		data = data & 0x3F;

	return (int)data;
}

static const struct SensorClass _Accel =
{	
	sizeof(struct Accel),
	Accel_ctor,
	Accel_dtor,
	Accel_attach,
	Accel_config,
	0,
	Accel_get,
};

const void *Accel = &_Accel;




