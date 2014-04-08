/*
 * i2c-soft.c
 *
 *  Created on: 19.12.2009
 *      Author: Pavel V. Gololobov
 */

#include <io.h>
#include <stdint.h>
#include "i2c-soft.h"

//------------------------------------------------------------------
// I2C Speed Down
//------------------------------------------------------------------
#define I2CWAIT i2c_Wait(100);

//------------------------------------------------------------------
// I2C Delay
//------------------------------------------------------------------
void i2c_Wait(int32_t n)
{
	do
	{
		nop();
		nop();
		nop();
		nop();
		nop();
	}
	while(--n);
}

//------------------------------------------------------------------
// I2C SDA SCL Control
//------------------------------------------------------------------
static void SetLowSDA()
{
	I2CDIR |= SDA;
	I2CWAIT
}
static void SetHighSDA()
{
	I2CDIR &= ~SDA;
	I2CWAIT
}

static void SetLowSCL()
{
	I2COUT &= ~SCL;
	I2CWAIT
}
static void SetHighSCL()
{
	I2COUT |= SCL;
	I2CWAIT
}

//------------------------------------------------------------------
// I2C Initialize Bus
//------------------------------------------------------------------
void i2c_Init()
{
	I2CSEL &= ~SDA;
	I2CSEL &= ~SCL;

	I2COUT &= ~SCL;
	I2COUT &= ~SDA;

	I2CDIR |= SCL;
	I2CDIR &= ~SDA;

	SetHighSCL();
	SetLowSDA();
	SetHighSDA();
}

//------------------------------------------------------------------
// I2C Start Data Transfer
//------------------------------------------------------------------
void i2c_Start()
{
	SetHighSCL();
	SetHighSDA();

	SetHighSCL();
	SetLowSDA();

	SetLowSCL();
	SetHighSDA();
}

//------------------------------------------------------------------
// I2C Stop  Transfer
//------------------------------------------------------------------
void i2c_Stop()
{
	SetLowSCL();
	SetLowSDA();

	SetHighSCL();
	SetLowSDA();

	SetHighSCL();
	SetHighSDA();
}

//------------------------------------------------------------------
// I2C Write  Transfer
//------------------------------------------------------------------
int16_t i2c_Write(uint8_t a)
{
	int16_t i;
	int16_t return_ack;

	for (i = 0; i < 8; i++)
    {
		SetLowSCL();
		if (a & 0x80)
			SetHighSDA();
		else
			SetLowSDA();

		SetHighSCL();
		a <<= 1;
	}
	SetLowSCL();

	/* ack Read */
	SetHighSDA();
	SetHighSCL();

	if (I2CIN & SDA)
		return_ack = NO_I2C_ACK;
	else
		return_ack = OK_I2C_ACK;
	
    SetLowSCL();

	return (return_ack);
}

//------------------------------------------------------------------
// I2C Read  Transfer
//------------------------------------------------------------------
uint8_t i2c_Read(int16_t ack)
{
	int16_t i;
	uint8_t caracter = 0x00;

	SetLowSCL();
	SetHighSDA();

	for (i = 0; i < 8; i++)
    {
		caracter = caracter << 1;
		SetHighSCL();
		if (I2CIN & SDA)
			caracter = caracter  + 1;
        
		SetLowSCL();
	}

	if (ack)
    {
		SetLowSDA();
	}
	SetHighSCL();
	SetLowSCL();

	return (caracter);
}

//------------------------------------------------------------------
// I2C Read Byte
//------------------------------------------------------------------
uint8_t i2c_ReadByte(uint8_t nAddress, uint8_t nRegister)
{
	i2c_Start();
	i2c_Write(nAddress);
	i2c_Write(nRegister);
	i2c_Start();
	i2c_Write(nAddress | 0x01);
	uint8_t ret = i2c_Read(0);
	i2c_Stop();

	return ret;
}

//------------------------------------------------------------------
// I2C Write Byte
//------------------------------------------------------------------
void i2c_WriteByte(uint8_t nAddress, uint8_t nRegister, uint8_t nValue)
{
	i2c_Start();
	i2c_Write(nAddress);
	i2c_Write(nRegister);
	i2c_Write(nValue);
	i2c_Stop();
}

