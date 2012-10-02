/* **************************************************************************																					
 *                                OpenPicus                 www.openpicus.com
 *                                                            italian concept
 * 
 *            openSource wireless Platform for sensors and Internet of Things	
 * **************************************************************************
 *  FileName:        sensors.h
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
 


#ifndef SENSORS_H
#define SENSORS_H


extern const void *I2C;
extern const void *Sensor;
extern const void *Touch;
extern const void *Dig_io;
extern const void *An_i;

extern const void *An_Temp;
extern const void *Gps;
extern const void *Gyro;
extern const void *Accel;
extern const void *Rgb;
extern const void *Rht03;
extern const void *Oled;
extern const void * Uart_sensor;

extern unsigned char flag;

//Ports
#define DIG1	  	0
#define DIG2	  	1
#define DIG3	  	2
#define DIG4	  	3
#define AN1		  	0
#define AN2		  	1
#define AN3		  	2
#define I2C		  	0
#define SPI		  	0


//PWM sensor type
#define PWM		  	4
#define FREQUENCY	2
#define DUTY		3


#define TIMEOUT		65500


//Gyro sensor addresses and constants
#define GYROADD0 	0xD0
#define GYRO_TEMP	0
#define GYRO_X		1
#define GYRO_Y		2
#define GYRO_Z		3


//Accelerometer sensor addresses and constants
#define ACCADD0   0x98
#define ACC_X		0
#define ACC_Y		1
#define ACC_Z		2



//Oled sensor addresses and constants
#define OLEDADD0   	0x78
#define WHITE		0xA7
#define BLACK		0xA6
#define H			0x00
#define V			0x00
#define P			0x02
#define STRING		0x00
#define IMAGE		0x01



//Touch sensor address
#define TOUCHADD0 	0xB4
#define PAD1		1
#define PAD2		2
#define PAD3		4
#define PAD4		8
#define PAD5		16
#define PAD6		32
#define PAD7		64
#define PAD8		128



#endif


