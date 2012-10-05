/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        touch.c
 *  Dependencies:    OpenPicus libraries
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


#include <stdlib.h>
#include <stdarg.h>
#include "taskFlyport.h"
#include "grovelib.h"
#include "touch.h"


struct Interface *attachSensorToI2CBus(void *,int,int);
int set_register(BYTE,BYTE, BYTE);
BYTE read_register(BYTE,BYTE);


struct Touch
{
	const void *class;
	struct Interface *inter;
	BYTE pads;//pads number installed
	BYTE implement;
	BYTE devaddress; //already shifted;
	BYTE datapre;
};


static int Touch_read_ (void * _dev)
{

	struct Touch *dev = _dev;
	BYTE data = 0;
	data =	read_register(dev->devaddress, STATUS1);
	data = data & 0x0F;
	if (dev->pads > 4)
		data =data | (read_register(dev->devaddress, STATUS2)  << 4);
	if(dev->datapre == data)
	{
		flag = 1;
		return 0;//same previuos value
	}
	else
		dev->datapre = data;	
	flag = 0;
	return data;
	
}	


static void groveTouchSensorEnable(BYTE address, BYTE padsnumb)
{
	if (padsnumb > 4)
		padsnumb+=4;
	set_register(address, ELE_CFG, padsnumb);
}	


static int init(struct Touch *self)
{

	if(set_register(self->devaddress,ELE_CFG,0))
		return -1;
	// Section A - Controls filtering when data is > baseline.
    if(set_register(self->devaddress, MHD_R, 0x01))
		return -1;
    if(set_register(self->devaddress, NHD_R, 0x01))
		return -1;
    if(set_register(self->devaddress, NCL_R, 0x00))
		return -1;
    if(set_register(self->devaddress, FDL_R, 0x00))
		return -1;
    
    // Section B - Controls filtering when data is < baseline.
    set_register(self->devaddress, MHD_F, 0x01);
    set_register(self->devaddress, NHD_F, 0x01);
    set_register(self->devaddress, NCL_F, 0xFF);
    set_register(self->devaddress, FDL_F, 0x02);
    
    // Section C - Sets touch and release thresholds for each electrode
    set_register(self->devaddress, ELE0_T, TOU_THRESH);
    set_register(self->devaddress, ELE0_R, REL_THRESH);
    set_register(self->devaddress, ELE1_T, TOU_THRESH);
    set_register(self->devaddress, ELE1_R, REL_THRESH);

    set_register(self->devaddress, ELE2_T, TOU_THRESH);
    set_register(self->devaddress, ELE2_R, REL_THRESH);
    
    set_register(self->devaddress, ELE3_T, TOU_THRESH);
    set_register(self->devaddress, ELE3_R, REL_THRESH);
    
    set_register(self->devaddress, ELE4_T, TOU_THRESH);
    set_register(self->devaddress, ELE4_R, REL_THRESH);
    
    set_register(self->devaddress, ELE5_T, TOU_THRESH);
    set_register(self->devaddress, ELE5_R, REL_THRESH);
    
    set_register(self->devaddress, ELE6_T, TOU_THRESH);
    set_register(self->devaddress, ELE6_R, REL_THRESH);
    
    set_register(self->devaddress, ELE7_T, TOU_THRESH);
    set_register(self->devaddress, ELE7_R, REL_THRESH);
    
    set_register(self->devaddress, ELE8_T, TOU_THRESH);
    set_register(self->devaddress, ELE8_R, REL_THRESH);
    
    set_register(self->devaddress, ELE9_T, TOU_THRESH);
    set_register(self->devaddress, ELE9_R, REL_THRESH);
    
    set_register(self->devaddress, ELE10_T, TOU_THRESH);
    set_register(self->devaddress, ELE10_R, REL_THRESH);
    
    set_register(self->devaddress, ELE11_T, TOU_THRESH);
    set_register(self->devaddress, ELE11_R, REL_THRESH);


	set_register(self->devaddress, 0x77, 0x0F);

	groveTouchSensorEnable(self->devaddress,self->pads);
	return 0;
}




static void *Touch_ctor (void * _self, va_list *app)
{
	struct Touch *self = _self;
	self->devaddress =  va_arg(*app, const BYTE);
	self->pads = va_arg(*app, const BYTE);
	self->datapre = 0;
	self->inter = NULL;
	return self;
}

static void Touch_dtor (void * _sensor)
{

}


static void *Touch_attach (void * _board,void *_sensor,int ic2bus)
{
	struct Touch *sensor = _sensor;
	int add = sensor->devaddress;
	sensor->inter = attachSensorToI2CBus(_board,ic2bus,add);
	return sensor->inter;
}


static int Touch_config (void * _self, va_list *app)
{
	struct Touch *self = _self;
	return init(self);	
}


static float Touch_get (void * _self,va_list *app)
{
	struct Touch *self = _self;
	int ret =  Touch_read_(self);
	return ret;
}



static const struct SensorClass _Touch =
{	
	sizeof(struct Touch),
	Touch_ctor,
	Touch_dtor,
	Touch_attach,
	Touch_config,
	0,
	Touch_get,

};

const void *Touch = &_Touch;



//end
