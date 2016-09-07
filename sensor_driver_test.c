/*! @mainpage
 *
 ****************************************************************************
 * Copyright (C) 2016 GlobalMEMS, Inc. <www.globalmems.com>
 * All rights reserved.
 *
 * File : sensor_driver_test.c
 *
 * Date : 2016/09/06
 *
 * Revision : 1.0.0
 *
 * Usage: GMC303 Sensor Driver User Reference Code
 *
 ****************************************************************************
 * @section License
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 **************************************************************************/
 
 /*! @file sensor_driver_test.c
 *  @brief  GMC303 Sensor Driver Test Main Program 
 *  @author Joseph FC Tseng
 */
 
#include <stdio.h>
#include "gmc303.h" 
 
#define DELAY_MS(ms)	//.....     /* Add your time delay function here */

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{

	s32 i;
	bus_support_t gmc303_bus;
	raw_data_xyzt_t rawData;
	float_xyzt_t calibData;
	float_xyzt_t adjustVal = { 1.0, 1.0, 1.0, 0.0 };

	/* Add your HW initialization code here
	...
	...
	...
	...
	*/
	
	/* GMC303 I2C bus setup */
	bus_init_I2C(&gmc303_bus, GMC303_7BIT_I2C_ADDR); //Initialize I2C bus
	gmc303_bus_init(&gmc303_bus); //Initailze GMC303 bus to I2C

	/* GMC303 soft reset */
	gmc303_soft_reset();
	
	/* Wait 10ms for reset complete */
	DELAY_MS(10);

	/* GMC303 get the sensitivity adjust values */
	gmc303_get_sensitivity_adjust_val(&adjustVal);
	
	printf("\rSadj=%f, %f, %f\r", 
	       adjustVal.u.x,
	       adjustVal.u.y,
	       adjustVal.u.z);
									
	//Set to CM 10Hz
	gmc303_set_operation_mode(GMC303_OP_MODE_CM_10HZ);
	
	for (;;){
		
		//Read XYZ raw
		gmc303_read_data_xyz(&rawData);
		
		//Sensitivity adjustment
		for(i = 0; i < 3; ++i)
			calibData.v[i] = rawData.v[i] * adjustVal.v[i];
		
		printf("\rXYZ=%f, %f, %f\r",
		       calibData.u.x,
		       calibData.u.y,
		       calibData.u.z);		
		
		/* Delay 1 sec */
		DELAY_MS(1000);
	}
}
