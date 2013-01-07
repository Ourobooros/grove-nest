/** \file sonic_ranger.c
 *  \brief Grove devices support library 
 */

/**
\addtogroup Grove devices
@{
*/

/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        sonic_ranger.c
 *  Module:          FlyPort WI-FI/EHT
 *  Compiler:        Microchip C30 v3.12 or higher
 *
 *  Author               Rev.    Date              Comment
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Davide Vicca    1.0     11/17/2012		   First release  (core team)
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
#include "input_capture.h"


extern unsigned char flag;
struct Interface *attachSensorToDigioBus(void *,int,int);

/**
 * struct Ranger - The structure for Ranger grove sensor 
 */
struct Ranger
{
	const void *Ranger;
	BYTE ic_module;
	struct Interface *inter;
};



/**
 * static float decode(unsigned long data1,unsigned long data2)- Perform the decoding of the object distance
 * \unsignend long data1- first timer value
 * \unsignend long data2- first timer value
 *\return - the distance to the object
*/
static float decode(unsigned long data1,unsigned long data2)
{
	unsigned int time= 0;
	float data = 0;
	/****sound speed round trip conversion*****/ 
	time = (data2 - data1)>>5;
	data = (float)time /29;
	return data;
}


/**
 * static void *Ranger_ctor (void * _self, va_list *app) -Ranger grove device Constructor  
 * \param *_self - pointer to the RHT03 grove device class.
 * \param *app - none
* \return - Pointer to the Ranger devices instantiated
*/
static void *Ranger_ctor (void * _self, va_list *app)
{
	struct Ranger *self = _self;
	self->inter = NULL;
	return self;
}	


/**
 * static void Ranger_dtor (void * _sensor)- Ranger grove device Destructor  
 * \param *_sensor - pointer to the RHT03 grove device class.
 * \return - None
*/
static void Ranger_dtor (void * _sensor)
{
	struct Ranger *sensor = _sensor;
	if(sensor->inter)
	{

		free(sensor->inter->port);
		free(sensor->inter);
	}
}	


/**
 * static void* Ranger_attach (void * _board,void *_sensor,int n) - attach a Ranger grove device to the GroveNest digital I/O port  
 * \param *_board - pointer to the GroveNest 
 * \param *_sensor - pointer to the Ranger grove device class.
 * \param n - port number which Ranger device is connected to
 * \return 
 <UL>
	<LI><Breturn = Pointer to the digital interface created:</B> the operation was successful.</LI> 
	<LI><B>return = NULL:</B> the operation was unsuccessful.</LI> 
 </UL>
 */
static void* Ranger_attach (void * _board,void *_sensor,int n)
{
	struct Ranger *sensor = _sensor;
	sensor->inter = attachSensorToDigioBus(_board,n,IN);	
	return sensor->inter;

}	


/**
 *  static int Ranger_configure (void * _self, va_list *app) -  Configure the Ranger grove device
 * \param *_self - pointer to the device 
 * \param *app - the number of IC module is being used by the Ranger device 
 * \return operation is always successful:
 	<LI><Breturn = 0:</B> </LI> 
 </UL>
 */
 static int Ranger_configure (void * _self, va_list *app)
{
	struct Ranger *self = _self;
	self->ic_module = va_arg(*app, const BYTE);
	IOInit_(self->inter->port->Pin1,(self->ic_module + 50));
	return 0;

}	


/**
 * static float Ranger_get(void * _self,va_list *app) -  read a float value from the ranger device.
 * \param *_self - pointer to the device 
 * \return the distance to the object:
 */
static float Ranger_get(void * _self,va_list *app)
{
	struct Ranger *self = _self;
	unsigned long data1,data2;
	float data = 0;
	int error = 0;
	/*triggering procedure*/
	IOInit(self->inter->port->Pin1,OUT);
	IOPut(self->inter->port->Pin1,OFF);
	Delay10us(2);
	IOPut(self->inter->port->Pin1,ON);
	Delay10us(2);
	IOPut(self->inter->port->Pin1,OFF);
	IOInit(self->inter->port->Pin1,IN);
	/**********************/
	
	ICInit(self->ic_module,2,1);
	
	error = ICOn(self->ic_module,50,&data1);
	if(!error)
		error = ICOn(self->ic_module,50,&data2);
	ICOff(self->ic_module);//swicht off the ic module
	if(error)
	{
		flag = -1; 
		return -1;
	}
	data = decode(data1,data2);
	IOInit(self->inter->port->Pin1,OUT);
	IOPut(self->inter->port->Pin1,OFF);
	return data;
}	

static const struct SensorClass _Ranger =
{	
	sizeof(struct Ranger),
	Ranger_ctor,
	Ranger_dtor,
	Ranger_attach,
	Ranger_configure,
	0,
	Ranger_get,
};

const void * Ranger = &_Ranger;





