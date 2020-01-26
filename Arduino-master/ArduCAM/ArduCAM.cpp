/*
  ArduCAM.cpp - Arduino library support for CMOS Image Sensor
  Copyright (C)2011-2015 ArduCAM.com. All right reserved
  
  Basic functionality of this library are based on the demo-code provided by
  ArduCAM.com. You can find the latest version of the library at
  http://www.ArduCAM.com

  Now supported controllers:
		-	OV7670
		-	MT9D111
		-	OV7675
		-	OV2640
		-	OV3640
		-	OV5642
		-	OV7660
		-	OV7725			
			
	We will add support for many other sensors in next release.
        
  Supported MCU platform
 		-	Theoretically support all Arduino families
  		-	Arduino UNO R3			(Tested)
  		-	Arduino MEGA2560 R3		(Tested)
  		-	Arduino Leonardo R3		(Tested)
  		-	Arduino Nano			(Tested)
  		-	Arduino DUE				(Tested)
  		- Arduino Yun				(Tested)
  		-	Raspberry Pi			(Tested)
  		      
  If you make any modifications or improvements to the code, I would appreciate
  that you share the code with me so that I might include it in the next release.
  I can be contacted through http://www.ArduCAM.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*------------------------------------
	Revision History:
	2012/09/20 	V1.0.0	by Lee	first release 	
	2012/10/23  V1.0.1  by Lee  Resolved some timing issue for the Read/Write Register
	2012/11/29	V1.1.0	by Lee  Add support for MT9D111 sensor
	2012/12/13	V1.2.0	by Lee	Add support for OV7675 sensor
	2012/12/28  V1.3.0	by Lee	Add support for OV2640,OV3640,OV5642 sensors
	2013/02/26	V2.0.0	by Lee	New Rev.B shield hardware, add support for FIFO control 
															and support Mega1280/2560 boards 
	2013/05/28	V2.1.0	by Lee	Add support all drawing functions derived from UTFT library 		
	2013/08/24	V3.0.0	by Lee	Support ArudCAM shield Rev.C hardware, features SPI interface and low power mode.
								Support almost all series of Arduino boards including DUE.
	2014/02/06  V3.0.1  by Lee  Minor change to the library, fixed some bugs, add self test code to the sketches for easy debugging.
	2014/03/09  V3.1.0  by Lee  Add the more impressive example sketches. 
								Optimise the OV5642 settings, improve image quality.
								Add live preview before JPEG capture.
								Add play back photos one by one	after BMP capture.	
	2014/05/01  V3.1.1  by Lee  Minor changes to add support Arduino IDE for linux distributions.				
	2014/09/30  V3.2.0  by Lee  Improvement on OV5642 camera dirver.			
	2014/10/06  V3.3.0  by Lee  Add OV7660,OV7725 camera support.	
	2015/02/27  V3.4.0  by Lee  Add the support for Arduino Yun board, update the latest UTFT library for ArduCAM.		
	2015/06/09  V3.4.1  by Lee	Minor changes and add some comments															
--------------------------------------*/
#include "Arduino.h"
#include "ArduCAM.h"
#include "memorysaver.h"
#include <Wire.h>
#include <SPI.h>

#if defined(__arm__)
#define Wire Wire1
#endif 

//Assert CS signal
void ArduCAM::CS_LOW(void)
{
	cbi(P_CS, B_CS);
}

//Disable CS signal
void ArduCAM::CS_HIGH(void)
{
	sbi(P_CS, B_CS);
}

//Set corresponding bit  
void ArduCAM::set_bit(uint8_t addr, uint8_t bit)
{
	uint8_t temp;
	temp = read_reg(addr);
	write_reg(addr, temp | bit);

}

//Clear corresponding bit 
void ArduCAM::clear_bit(uint8_t addr, uint8_t bit)
{
	uint8_t temp;
	temp = read_reg(addr);
	write_reg(addr, temp & (~bit));
}

//Get corresponding bit status
uint8_t ArduCAM::get_bit(uint8_t addr, uint8_t bit)
{
	uint8_t temp;
	temp = read_reg(addr);
	temp = temp & bit;
	return temp;
}

//Set ArduCAM working mode
//MCU2LCD_MODE: MCU writes the LCD screen GRAM
//CAM2LCD_MODE: Camera takes control of the LCD screen
//LCD2MCU_MODE: MCU read the LCD screen GRAM
void ArduCAM::set_mode(uint8_t mode)
{
	switch(mode)
	{
		case MCU2LCD_MODE:
			write_reg(ARDUCHIP_MODE, MCU2LCD_MODE);
			break;
		case CAM2LCD_MODE:
			write_reg(ARDUCHIP_MODE, CAM2LCD_MODE);
			break;
		case LCD2MCU_MODE:
			write_reg(ARDUCHIP_MODE, LCD2MCU_MODE);
			break;
		default:
			write_reg(ARDUCHIP_MODE, MCU2LCD_MODE);
			break;
	}
}

//Low level SPI write operation
int ArduCAM::bus_write(int address, int value) {
  // take the SS pin low to select the chip:
  cbi(P_CS, B_CS);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  sbi(P_CS, B_CS);
}

//Low level SPI read operation
uint8_t ArduCAM::bus_read(int address) {
  uint8_t value = 0;
  // take the SS pin low to select the chip:
  cbi(P_CS, B_CS);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  value = SPI.transfer(0x00);
  // take the SS pin high to de-select the chip:
  sbi(P_CS, B_CS);
  return value;
}

//Write ArduChip internal registers
void ArduCAM::write_reg(uint8_t addr, uint8_t data)
{
	bus_write(addr | 0x80, data);
}

//Read ArduChip internal registers
uint8_t ArduCAM::read_reg(uint8_t addr)
{
	uint8_t data;
	data = bus_read(addr & 0x7F);
	return data;
}

ArduCAM::ArduCAM()
{
	sensor_model = OV7670;
	sensor_addr = 0x42;
}

ArduCAM::ArduCAM(byte model,int CS)
{ 
	P_CS	= portOutputRegister(digitalPinToPort(CS));
	B_CS	= digitalPinToBitMask(CS);
	
	pinMode(CS,OUTPUT);
	
	//Must initialize the Bus default status
	sbi(P_CS, B_CS);
	
	//flush_fifo();
	
	sensor_model=model;
	switch(sensor_model)
	{
		case OV7660:
		case OV7670:
		case OV7675:
		case OV7725:
			sensor_addr = 0x42;
			break;
		case MT9D111_A: //Standard MT9D111 module
			sensor_addr = 0xba;
			break;			
		case MT9D111_B:	//Flex MT9D111 AF module
			sensor_addr = 0x90;
			break;
		case MT9M112:
		case MT9M001:
			sensor_addr = 0xba;	
			break;		
		case OV3640:
		case OV5640:
		case OV5642:
			sensor_addr = 0x78;
			break;
		case OV2640:
		case OV9650:
		case OV9655:
			sensor_addr = 0x60;
			break;
	
		default:
			sensor_addr = 0x42;
			break;
	}
}

//Reset the FIFO pointer to ZERO		
void ArduCAM::flush_fifo(void)
{
	write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

//Send capture command
void ArduCAM::start_capture(void)
{
	write_reg(ARDUCHIP_FIFO, FIFO_START_MASK);
}

//Clear FIFO Complete flag
void ArduCAM::clear_fifo_flag(void)
{
	write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

//Read FIFO single		
uint8_t ArduCAM::read_fifo(void)
{
	uint8_t data;
	data = bus_read(SINGLE_FIFO_READ);
	return data;
}

//Read Write FIFO length
//Support ArduCAM Mini only
uint32_t ArduCAM::read_fifo_length(void)
{
	uint32_t len1,len2,len3,length=0;
	len1 = read_reg(FIFO_SIZE1);
  len2 = read_reg(FIFO_SIZE2);
  len3 = read_reg(FIFO_SIZE3) & 0x07;
  length = ((len3 << 16) | (len2 << 8) | len1) & 0x07ffff;
	return length;
}

//Send read fifo burst command
//Support ArduCAM Mini only
void ArduCAM::set_fifo_burst()
{
	SPI.transfer(BURST_FIFO_READ);
}

//I2C Write 8bit address, 8bit data
byte ArduCAM::wrSensorReg8_8(int regID, int regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID & 0x00FF); 	
	Wire.write(regDat & 0x00FF); 	

	if(Wire.endTransmission())
	{
		return 0; 
	}

	delay(1);
  return(1);
}

//I2C Read 8bit address, 8bit data
byte ArduCAM::rdSensorReg8_8(uint8_t regID, uint8_t* regDat)
{

	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID & 0x00FF); 	
	Wire.endTransmission();
	
	Wire.requestFrom((sensor_addr >> 1),1);
	if(Wire.available())
		*regDat = Wire.read(); 	

	delay(1);
  return(1);
}

//I2C Write 8bit address, 16bit data
byte ArduCAM::wrSensorReg8_16(int regID, int regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID & 0x00FF); 	

	Wire.write(regDat >> 8);            // sends data byte, MSB first
  	Wire.write(regDat & 0x00FF);  	
	
	if(Wire.endTransmission())
	{
		return 0; 
	}

	delay(1);
  	return(1);
}

//I2C Read 8bit address, 16bit data
byte ArduCAM::rdSensorReg8_16(uint8_t regID, uint16_t* regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID); 	
	Wire.endTransmission();
	
	Wire.requestFrom((sensor_addr >> 1),2);
	if(Wire.available())
	{
		*regDat = Wire.read(); 	
		*regDat++ = Wire.read();
	}

	delay(1);
  	return(1);
}

//I2C Write 16bit address, 8bit data
byte ArduCAM::wrSensorReg16_8(int regID, int regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID >> 8);            // sends instruction byte, MSB first
  	Wire.write(regID & 0x00FF); 	
  	Wire.write(regDat & 0x00FF);  	

	if(Wire.endTransmission())
	{
		return 0; 
	}

	delay(1);
  return(1);
}

//I2C Read 16bit address, 8bit data
byte ArduCAM::rdSensorReg16_8(uint16_t regID, uint8_t* regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID >> 8);
	Wire.write(regID & 0x00FF); 	
	Wire.endTransmission();
	
	Wire.requestFrom((sensor_addr >> 1),1);
	if(Wire.available())
	{
		*regDat = Wire.read(); 	
	}
	delay(1);
  return(1);
}

//I2C Write 16bit address, 16bit data
byte ArduCAM::wrSensorReg16_16(int regID, int regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	
	Wire.write(regID >> 8);            // sends instruction byte, MSB first
  	Wire.write(regID & 0x00FF); 	

  	Wire.write(regDat >> 8);            // sends data byte, MSB first
  	Wire.write(regDat & 0x00FF);  	

	if(Wire.endTransmission())
	{
		return 0; 
	}

	delay(1);
  return(1);
}

//I2C Read 16bit address, 16bit data
byte ArduCAM::rdSensorReg16_16(uint16_t regID, uint16_t* regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID >> 8);
	Wire.write(regID & 0x00FF); 	
	Wire.endTransmission();
	
	Wire.requestFrom((sensor_addr >> 1),2);
	if(Wire.available())
	{
		*regDat = Wire.read(); 	
		*regDat++ = Wire.read(); 	
	}

	delay(1);
  return(1);
}

//I2C Array Write 8bit address, 8bit data
int ArduCAM::wrSensorRegs8_8(const struct sensor_reg reglist[])
{
	int err = 0;
	uint16_t reg_addr = 0;
	uint16_t reg_val = 0;
	const struct sensor_reg *next = reglist;
	
	while ((reg_addr != 0xff) | (reg_val != 0xff))
	{		
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
		err = wrSensorReg8_8(reg_addr, reg_val);
   	next++;
	   	
	} 
	
	return 1;
}

//I2C Array Write 8bit address, 16bit data
int ArduCAM::wrSensorRegs8_16(const struct sensor_reg reglist[])
{
	int err = 0;
	
	unsigned int reg_addr,reg_val;
	const struct sensor_reg *next = reglist;
	
	while ((reg_addr != 0xff) | (reg_val != 0xffff))
	{		
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
		err = wrSensorReg8_16(reg_addr, reg_val);
		//	if (!err)
	   	//return err;
	   	next++;
	}  
	
	return 1;
}

//I2C Array Write 16bit address, 8bit data
int ArduCAM::wrSensorRegs16_8(const struct sensor_reg reglist[])
{
	int err = 0;
	
	unsigned int reg_addr;
	unsigned char reg_val;
	const struct sensor_reg *next = reglist;
	
	while ((reg_addr != 0xffff) | (reg_val != 0xff))
	{		
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
		err = wrSensorReg16_8(reg_addr, reg_val);
		//if (!err)
	   	//return err;
	   next++;
	} 
	
	return 1;
}

//I2C Array Write 16bit address, 16bit data
int ArduCAM::wrSensorRegs16_16(const struct sensor_reg reglist[])
{
	int err = 0;
	
	unsigned int reg_addr,reg_val;
	const struct sensor_reg *next = reglist;
	
	while ((reg_addr != 0xffff) | (reg_val != 0xffff))
	{		
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
		err = wrSensorReg16_16(reg_addr, reg_val);
		//if (!err)
	    //   return err;
	   next++;
	} 
	
	return 1;
}

void ArduCAM::OV2640_set_JPEG_size(uint8_t size)
{
	#if defined OV2640_CAM
	switch(size)
	{
		case OV2640_160x120:	
			wrSensorRegs8_8(OV2640_160x120_JPEG);
			break;
		case OV2640_176x144:	
			wrSensorRegs8_8(OV2640_176x144_JPEG);
			break;
		case OV2640_320x240:
			wrSensorRegs8_8(OV2640_320x240_JPEG);
			break;
		case OV2640_352x288:
			wrSensorRegs8_8(OV2640_352x288_JPEG);
			break;
		case OV2640_640x480:
			wrSensorRegs8_8(OV2640_640x480_JPEG);
			break;
		case OV2640_800x600:
			wrSensorRegs8_8(OV2640_800x600_JPEG);
			break;
		case OV2640_1024x768:
			wrSensorRegs8_8(OV2640_1024x768_JPEG);
			break;
		case OV2640_1280x1024:
			wrSensorRegs8_8(OV2640_1280x1024_JPEG);
			break;
		case OV2640_1600x1200:
			wrSensorRegs8_8(OV2640_1600x1200_JPEG);
			break;
		default:
			wrSensorRegs8_8(OV2640_320x240_JPEG);
			break;
	}
	#endif
}

void ArduCAM::OV5642_set_JPEG_size(uint8_t size)
{
	#if defined OV5642_CAM
	uint8_t reg_val;
	wrSensorRegs16_8(ov5642_dvp_fmt_global_init); 
	//wrSensorRegs16_8(OV5642_VGA_preview_setting); 
	delay(100); 
	switch(size)
	{
		case OV5642_320x240:	
			wrSensorRegs16_8(ov5642_dvp_fmt_jpeg_qvga);
			wrSensorReg16_8(0x4407,0x04);
			break;
		case OV5642_640x480:	
			wrSensorRegs16_8(ov5642_dvp_fmt_jpeg_vga);
			break;
		case OV5642_1280x720:
			wrSensorRegs16_8(ov5642_dvp_fmt_jpeg_qvga);
			wrSensorRegs16_8(ov5642_res_720P);
			break;
		case OV5642_1920x1080:
			wrSensorRegs16_8(ov5642_dvp_fmt_jpeg_qvga);
			wrSensorRegs16_8(ov5642_res_1080P);
			break;
		case OV5642_2048x1563:
			wrSensorRegs16_8(ov5642_dvp_fmt_jpeg_qxga);
			break;
		case OV5642_2592x1944:
			//wrSensorRegs16_8(ov5642_dvp_fmt_jpeg_5M);
			wrSensorRegs16_8(OV5642_VGA_preview_setting); 
			delay(100);
			wrSensorRegs16_8(OV5642_JPEG_Capture_QSXGA);
			wrSensorReg16_8(0x4407,0x0C); 

			break;
		default:
			wrSensorRegs16_8(ov5642_dvp_fmt_jpeg_qvga);
			break;
	}

	#endif
}

void ArduCAM::set_format(byte fmt)
{
	if(fmt == BMP)
		m_fmt = BMP;
	else
		m_fmt = JPEG;
}
			
void ArduCAM::InitCAM()
{
	byte rtn = 0;
	byte reg_val;
	switch(sensor_model)
	{
		case OV7660:
		{
			#if defined OV7660_CAM
			wrSensorReg8_8(0x12, 0x80);
			delay(100);
			rtn = wrSensorRegs8_8(OV7660_QVGA);
			#endif
			break;
		}
		case OV7725:
		{
			#if defined OV7725_CAM
			wrSensorReg8_8(0x12, 0x80);
			delay(100);
			rtn = wrSensorRegs8_8(OV7725_QVGA);
			rdSensorReg8_8(0x15,&reg_val);
			wrSensorReg8_8(0x15, (reg_val | 0x02));
			#endif
			break;
		}
		case OV7670:
		{
			#if defined OV7670_CAM
			wrSensorReg8_8(0x12, 0x80);
			delay(100);
			rtn = wrSensorRegs8_8(OV7670_QVGA);
			#endif
			break;
		}
		case OV7675:
		{
			#if defined OV7675_CAM
			wrSensorReg8_8(0x12, 0x80);
			delay(100);
			rtn = wrSensorRegs8_8(OV7675_QVGA);
			
			#endif
			break;
		}
		case MT9D111_A:
		case MT9D111_B:
		{
			#if defined MT9D111_CAM
			wrSensorRegs8_16(MT9D111_QVGA_30fps);
			delay(1000);
			wrSensorReg8_16(0x97, 0x0020);
			wrSensorReg8_16(0xf0, 0x00);
			wrSensorReg8_16(0x21, 0x8403); //Mirror Column
			wrSensorReg8_16(0xC6, 0xA103);//SEQ_CMD
      wrSensorReg8_16(0xC8, 0x0005); //SEQ_CMD
      #endif
			break;

		}  
		case OV5642:
		{
			#if defined OV5642_CAM
			wrSensorReg16_8(0x3008, 0x80);

			delay(100);
			if(m_fmt == JPEG)
			{
				// wrSensorRegs16_8(OV5642_1080P_Video_setting);
				// rdSensorReg16_8(0x3818,&reg_val);
				// wrSensorReg16_8(0x3818, (reg_val | 0x20) & 0xBf);
				// rdSensorReg16_8(0x3621,&reg_val);
				// wrSensorReg16_8(0x3621, reg_val | 0x20);
				wrSensorRegs16_8(ov5642_dvp_fmt_global_init); 
				delay(100); 
				wrSensorRegs16_8(ov5642_dvp_fmt_jpeg_qvga); 
				// wrSensorReg16_8(0x4407,0x0C);
				//******************Test************************//
				//wrSensorRegs16_8(OV5642_VGA_preview_setting); 
				//delay(100); 
				//wrSensorRegs16_8(OV5642_JPEG_Capture_QSXGA); 
				wrSensorReg16_8(0x4407,0x10);
			}
			else
			{
				wrSensorRegs16_8(OV5642_RGB_QVGA);
				rdSensorReg16_8(0x3818,&reg_val);
				//Serial.println(reg_val,HEX);
				wrSensorReg16_8(0x3818, (reg_val | 0x60) & 0xff);
				rdSensorReg16_8(0x3621,&reg_val);
				wrSensorReg16_8(0x3621, reg_val & 0xdf);
			}
			
			#endif
			break;
		}
		case OV3640:
		{
			#if defined OV3640_CAM
			rtn = wrSensorRegs16_8(OV3640_QVGA);
			#endif
			break;
		}
		case OV2640:
		{
			#if defined OV2640_CAM
			wrSensorReg8_8(0xff, 0x01);
			wrSensorReg8_8(0x12, 0x80);
			delay(100);
			if(m_fmt == JPEG)
			{
				wrSensorRegs8_8(OV2640_JPEG_INIT);
				wrSensorRegs8_8(OV2640_YUV422);
				wrSensorRegs8_8(OV2640_JPEG);
				wrSensorReg8_8(0xff, 0x01);
				wrSensorReg8_8(0x15, 0x00);
				wrSensorRegs8_8(OV2640_320x240_JPEG);
				//wrSensorReg8_8(0xff, 0x00);
				//wrSensorReg8_8(0x44, 0x32);
			}
			else
			{
				wrSensorRegs8_8(OV2640_QVGA);			
			}
			#endif
			break;
		}
		case OV9655:
		{
			
			break;
		}
		case MT9M112:
		{
			
			break;
		}

		default:
			
			break;	
	}
}
