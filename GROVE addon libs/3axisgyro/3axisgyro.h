// 3axisgyro.h
extern const void *Gyro;

#ifndef GROVE3AXIS_H
#define GROVE3AXIS_H

//Gyro sensor addresses and constants
#define GYRO_ADDR0 	0xD0
#define GYRO_TEMP	0
#define GYRO_X		1
#define GYRO_Y		2
#define GYRO_Z		3

// ITG3200 Register Defines
#define WHO				0x00
#define	SMPL			0x15
#define DLPF			0x16
#define INT_C			0x17
#define INT_S			0x1A
#define	TMP_H			0x1B
#define	TMP_L			0x1C
#define	GX_H			0x1D
#define	GX_L			0x1E
#define	GY_H			0x1F
#define	GY_L			0x20
#define GZ_H			0x21
#define GZ_L			0x22
#define PWR_M			0x3E
#define GYRO_ADDRESS 	0x68
#define NACK			1
#define Y_OFFSET		243
#define X_OFFSET		28


#endif


