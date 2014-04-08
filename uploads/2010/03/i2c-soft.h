/*
 * i2c-soft.h
 *
 *  Created on: 19.12.2009
 *      Author: Pavel V. Gololobov
 */

#ifndef I2C_H_
#define I2C_H_

#include <io.h>
#include <stdint.h>

#define NO_I2C_ACK 0
#define OK_I2C_ACK 1

#ifndef SDA
#define	I2COUT		P3OUT
#define	I2CIN		P3IN
#define	I2CDIR		P3DIR
#define	I2CSEL		P3SEL

#define SDA       	0x02  // Serial Data Line
#define SCL       	0x08  // Serial Clock Line
#endif

// Init Bus
void i2c_Init();
// Start Transfer
void i2c_Start();
// Stop Transfer
void i2c_Stop();
// Write Transfer
int16_t i2c_Write(uint8_t);
// Read Transfer
uint8_t i2c_Read(int16_t);

// Read Byte
uint8_t i2c_ReadByte(uint8_t, uint8_t);
// Write Byte
void i2c_WriteByte(uint8_t, uint8_t, uint8_t);

#endif
