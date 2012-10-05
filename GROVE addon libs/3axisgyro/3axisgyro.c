/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        3axisgyro.c
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
#include <math.h>
#include "taskFlyport.h"
#include "grovelib.h"
#include "3axisgyro.h"


struct Interface *attachSensorToI2CBus(void *,int,int);
int set_register(BYTE,BYTE, BYTE);
BYTE read_register(BYTE,BYTE);


struct Gyro
{
	const void *class;
	struct Interface *inter;	
	BYTE devaddress; //already shifted;
};



static int init(struct Gyro *self)
{
	//Set the display Off
	if(set_register(self->devaddress,PWR_M,0x80))	
		return -1;
	if(set_register(self->devaddress,SMPL,0x00))// sample rate divider
		return -1;
	if(set_register(self->devaddress,DLPF,0x1E))// ?2000 degrees/s (default value) and low pass filter
		return -1;
	if(set_register(self->devaddress,INT_C,0xC5))// enable send raw values
		return -1;
	return set_register(self->devaddress,PWR_M,0x01);
	
}


static int Gyro_read(void * _dev,BYTE reg)
{
	struct Gyro *dev = _dev;
	BYTE data = 0;
	Delay10us(1);
	data =	read_register(dev->devaddress, reg);
	return data;	
}	


static void *Gyro_ctor (void * _self, va_list *app)
{
	struct Gyro *self = _self;
	self->devaddress =  va_arg(*app, const BYTE);
	self->inter = NULL;
	return self;
}	

static void Gyro_dtor (void * _sensor)
{

}	


static void *Gyro_attach (void * _board,void *_sensor,int ic2bus)
{
	struct Gyro *sensor = _sensor;
	int add = sensor->devaddress;
	sensor->inter = attachSensorToI2CBus(_board,ic2bus,add);
	return sensor->inter;
}	


static int Gyro_config (void * _self, va_list *app)
{
	struct Gyro *self = _self;
	return init(self);	
}


static float Gyro_get (void * _self, va_list *app)
{
	struct Gyro *self = _self;
	int data = 0;
	float output = 0;
	BYTE register_ =  va_arg(*app, const BYTE);
	unsigned int y = TIMEOUT;
	while(((Gyro_read(self,INT_S) &1) != 1) && y)
		y--;
	if(!y)
	{//Time out occurred
		flag = 1;
		return -1;
	}
	switch(register_)
	{
		case 0:
		{
			data = Gyro_read(self,TMP_L);
			data = data | (Gyro_read(self,TMP_H)<<8);	
			output = 35.0 + (((float) (data + 13200)) / 280);
			break;
		}	
		case 1:
		{
			while((Gyro_read(self,INT_S) &1) != 1);
			data = Gyro_read(self,GX_L);
			data = data | (Gyro_read(self,GX_H)<<8);	
			output = (float)data/16 ;
			break;
		}
		case 2:
		{
			while((Gyro_read(self,INT_S) &1) != 1);
			data = Gyro_read(self,GY_L);
			data = data | (Gyro_read(self,GY_H)<<8);	
			output = (float)data/16 ;
			break;
		}
		case 3:
		{
			while((Gyro_read(self,INT_S) &1) != 1);
			data = Gyro_read(self,GZ_L);
			data = data | (Gyro_read(self,GZ_H)<<8);	
			output = (float)data/16 ;
		}
	}
	return output;
}

static const struct SensorClass _Gyro =
{	
	sizeof(struct Gyro),
	Gyro_ctor,
	Gyro_dtor,
	Gyro_attach,
	Gyro_config,
	0,
	Gyro_get,
};

const void *Gyro = &_Gyro;





