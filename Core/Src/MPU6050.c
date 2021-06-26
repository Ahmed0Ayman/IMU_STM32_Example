/*
 * MPU6050.c
 *
 *  Created on: Jun 26, 2021
 *      Author: Ahmed_Ayman
 */



/* static variables hold the callibration values
 *
 */
#include "MPU6050.h"


PRIVATRE int32_t  GYRO_OFFSET_X,GYRO_OFFSET_Y,GYRO_OFFSET_Z;
PRIVATRE int32_t  ACCEL_OFFSET_X,ACCEL_OFFSET_Y,ACCEL_OFFSET_Z ;

PRIVATRE _Bool  GYRO_CALIBRATED,IsGYROInitialized ;

PRIVATRE MPU6050_DATA_t MPU_REGS_DATA_STATIC ;

float ANGLE_PITCH,ANGLE_ROLL;

/* brief : this function used to read the MPU temp register this function PRIVATEM used only by this file
 * param : ALL_Axis_Var is a data structure that in general hold all MPU readed data register
 * return : void
 */
PRIVATRE void MPU6050_Read_temp_reg(MPU6050_DATA_t *ALL_Axis_Var);


/* brief : this function used to Calibrate the MPU gyro this function PRIVATEM used only by this file
 * param : flag  used to mark that the calibration is performed succefully
 * return : void
 */
PRIVATRE void MPU6050_GYRO_CALIBRATION_FUN(_Bool * flag);





/* brief : this function used to Calibrate the MPU gyro this function PRIVATEM used only by this file
 * param : flag  used to mark that the calibration is performed succefully
 * return : void
 */
PRIVATRE void MPU6050_GYRO_CALIBRATION_FUN(_Bool * flag)
{
	int32_t READED_DATA[3]={0};
	uint8_t data[6] = { 0 };
	for(int i=0;i<2000;i++)
	{
		HAL_I2C_Mem_Read(&hi2c2, MPU6050_DEVICE_ADDR, MPU6050_GYRO_XOUT_H_R, 1,
				data, 6,20);
		MPU_REGS_DATA_STATIC.XG_Value = (uint16_t) (data[0] << 8
				| data[1]);
		MPU_REGS_DATA_STATIC.YG_Value  = (uint16_t) (data[2] << 8
				| data[3]);
		MPU_REGS_DATA_STATIC.ZG_Value  = (uint16_t) (data[4] << 8
				| data[5]);

	READED_DATA[0] += MPU_REGS_DATA_STATIC.XG_Value ;
	READED_DATA[1] += MPU_REGS_DATA_STATIC.YG_Value ;
	READED_DATA[2] += MPU_REGS_DATA_STATIC.ZG_Value ;
	HAL_Delay(10);
	}
	// now we can calculate the offset
	GYRO_OFFSET_X = READED_DATA[0]/2000 ;
	GYRO_OFFSET_Y = READED_DATA[1]/2000 ;
	GYRO_OFFSET_Z = READED_DATA[2]/2000 ;


	// set the flag to true to indicate that the sensor is calibrated
	*flag = true ;

}





/* brief : this function used to get the GYRO angles
 * param : ANGELS_VAR_STRUCT is a data structure that in general hold all MPU Angles
 * return : void
*/
void MPU6050_ALL_GYRO_ANGELS(MPU6050_ANGELS_t *ANGELS_VAR_STRUCT) {


/* first before initialize gyro we need to read the accel data to make it as start value of gyro */
	if(IsGYROInitialized == false)
	{
	ANGELS_VAR_STRUCT->GYRO_ANGLE_PITCH = ANGELS_VAR_STRUCT->ACCEL_ANGLE_PITCH ;
	ANGELS_VAR_STRUCT->GYRO_ANGLE_ROLL  = ANGELS_VAR_STRUCT->ACCEL_ANGLE_ROLL  ;
	}

/* now  we read gyro data */
	MPU6050_ALL_GYRO_Read(&MPU_REGS_DATA_STATIC);

	ANGELS_VAR_STRUCT->GYRO_ANGLE_PITCH += (float) MPU_REGS_DATA_STATIC.XG_Value
			* (1/(MPU_SCALL * ((1000 / MPU_REFRUSH_READING_RATE))));

	ANGELS_VAR_STRUCT->GYRO_ANGLE_ROLL += (float) MPU_REGS_DATA_STATIC.YG_Value
			* (1/(MPU_SCALL * ((1000 / MPU_REFRUSH_READING_RATE))));


	ANGELS_VAR_STRUCT->GYRO_ANGLE_ROLL  -= ANGELS_VAR_STRUCT->GYRO_ANGLE_PITCH
			* sin((float) MPU_REGS_DATA_STATIC.ZG_Value
							* ((1/(MPU_SCALL * ((1000 / MPU_REFRUSH_READING_RATE))))
									* DegreeToRadian));
	ANGELS_VAR_STRUCT->GYRO_ANGLE_PITCH += ANGELS_VAR_STRUCT->GYRO_ANGLE_ROLL
			* sin((float) MPU_REGS_DATA_STATIC.ZG_Value
							* ((1/(MPU_SCALL * ((1000 / MPU_REFRUSH_READING_RATE))))
									* DegreeToRadian));


}/* END_FUN MPU6050_ALL_GYRO_ANGELS() */






/* brief : this function used to get the accel angles
 * param : ANGELS_VAR_STRUCT is a data structure that in general hold all MPU Angles
 * return : void
*/
void MPU6050_ALL_ACCEL_ANGELS(MPU6050_ANGELS_t *ANGELS_VAR_STRUCT) {
	float accel_x,accel_y,accel_z,accel_total=0;
	MPU6050_ALL_ACCEL_Read(&MPU_REGS_DATA_STATIC);


accel_x = ((float)MPU_REGS_DATA_STATIC.XA_Value)/ MPU6050_ACCEL_SCALL;
accel_y = ((float)MPU_REGS_DATA_STATIC.YA_Value)/ MPU6050_ACCEL_SCALL;
accel_z = (((float)MPU_REGS_DATA_STATIC.ZA_Value)/ MPU6050_ACCEL_SCALL);
accel_total = sqrt((accel_x*accel_x)+(accel_y*accel_y)+(accel_z*accel_z));
ANGELS_VAR_STRUCT->ACCEL_ANGLE_PITCH = asin(accel_y/accel_total)*57.296;
ANGELS_VAR_STRUCT->ACCEL_ANGLE_ROLL = asin(accel_x/accel_total)*57.296;
IsGYROInitialized = true ;


}/* END_FUN MPU6050_ALL_ACCEL_ANGELS() */









/* brief : this function used to read the MPU GYRO register
 * param : ALL_Axis_Var is a data structure that in general hold all MPU readed data register
 * return : void
*/
void MPU6050_ALL_GYRO_Read(MPU6050_DATA_t *ALL_Axis_Var) {


	if (GYRO_CALIBRATED == false) {
		MPU6050_GYRO_CALIBRATION_FUN(&GYRO_CALIBRATED);
	}
	uint8_t data[6] = { 0 };
	HAL_I2C_Mem_Read(&hi2c2, MPU6050_DEVICE_ADDR, MPU6050_GYRO_XOUT_H_R, 1,
			data, 6, 20);
	ALL_Axis_Var->XG_Value = (uint16_t)((data[0] << 8 | data[1]) - GYRO_OFFSET_X);
	ALL_Axis_Var->YG_Value = (uint16_t)((data[2] << 8 | data[3]) - GYRO_OFFSET_Y);
	ALL_Axis_Var->ZG_Value = (uint16_t)((data[4] << 8 | data[5]) - GYRO_OFFSET_Z);




}/* END_FUN MPU6050_ALL_GYRO_Read() */









/* brief : this function used to read the MPU accel register
 * param : ALL_Axis_Var is a data structure that in general hold all MPU readed data register
 * return : void
*/
void MPU6050_ALL_ACCEL_Read(MPU6050_DATA_t *ALL_Axis_Var) {


	uint8_t data[6] = { 0 };
	HAL_I2C_Mem_Read(&hi2c2, MPU6050_DEVICE_ADDR, MPU6050_ACCEL_XOUT_H_R, 1,
			data, 6, 20);
	ALL_Axis_Var->XA_Value = (uint16_t)(((data[0] << 8)  | data[1])-ACCEL_OFFSET_X);
	ALL_Axis_Var->YA_Value = (uint16_t) (((data[2] << 8) | data[3])-ACCEL_OFFSET_Y);
	ALL_Axis_Var->ZA_Value = (uint16_t) (((data[4] << 8) | data[5])-ACCEL_OFFSET_Z);



}/* END_FUN MPU6050_ALL_ACCEL_Read() */





/****************************************************************************************
 * set all configurations
 *  parameter : Config_arr 2D array first byte represent addr and the second byte represent value
 *  return MPU6050_OK if this function successfully set registers
 *  you can add and remove any registers that you need to config
 *  the configuration will be in form of 2D array first byte hold the address register and the second byte hold
 *  the value that you need to assign to that register
 ****************************************************************************************/
void MPU6050_SET_Config(uint8_t *Config_arr, uint8_t array_size) {

	for (uint8_t i = 0; i < (array_size / 2); i++) {
		HAL_I2C_Mem_Write(&hi2c2, MPU6050_DEVICE_ADDR, *(Config_arr + (i * 2)), 1,
				(Config_arr + (i * 2) + 1), 1, 20);
	}

}/* END_FUN MPU6050_SET_Config() */





/* brief : this function used to read the MPU temp register this function PRIVATEM used only by this file
 * param : ALL_Axis_Var is a data structure that in general hold all MPU readed data register
 * return : void
 */
 static void MPU6050_Read_temp_reg(MPU6050_DATA_t *ALL_Axis_Var) {

	 int data[2] = {0} ;
	HAL_I2C_Mem_Read(&hi2c2, MPU6050_DEVICE_ADDR, MPU6050_TEMP_OUT_H_R, 1,
			data , 2, 20);
	ALL_Axis_Var->TEMP_READED = (uint16_t)(((data[0] << 8)  | data[1]));

 }/* END_FUN MPU6050_Read_temp_reg() */


/* brief : this function used to get the temp value in C degree
 * param : ANGLES_VAR_STRUCT this data structure type in general used to hold the all required data that needed from MPU
 * return : void
 */
void MPU6050_Read_Temp_In_C(MPU6050_ANGELS_t *ANGLES_VAR_STRUCT)
{
	MPU6050_Read_temp_reg(&MPU_REGS_DATA_STATIC);
	ANGLES_VAR_STRUCT->TEMP_IN_CEL = (MPU_REGS_DATA_STATIC.TEMP_READED)/340 + 36.53;

}/* END_FUN MPU6050_Read_Temp_In_C() */




/* brief : if you call this function you will get aall data from MPU *(GYRO - ACCAL - And tamp )
 * here also if you need to change the complemantary filter gain you caan do it in the MPU_config file
 * param : ANGELS_STRUCT_VAR return  all angles and temp
 * return : void
 */
void Read_Accurate_Angles_ComplemantrayFilter(MPU6050_ANGELS_t *ANGELS_STRUCT_VAR)
{

	MPU6050_ALL_GYRO_ANGELS(ANGELS_STRUCT_VAR);
	MPU6050_ALL_ACCEL_ANGELS(ANGELS_STRUCT_VAR);
	MPU6050_Read_Temp_In_C(ANGELS_STRUCT_VAR);
	// now we have accel & gyro angles so we will apply filter to our measurements angle to get a very stable angle without offset
	ANGELS_STRUCT_VAR->GYRO_ANGLE_ROLL = ANGELS_STRUCT_VAR->GYRO_ANGLE_ROLL * ComplemantaryFilterGain1 \
			+ ANGELS_STRUCT_VAR->ACCEL_ANGLE_ROLL * ComplemantaryFilterGain2;
	ANGELS_STRUCT_VAR->GYRO_ANGLE_PITCH = ANGELS_STRUCT_VAR->GYRO_ANGLE_PITCH * ComplemantaryFilterGain1 \
			+ ANGELS_STRUCT_VAR->ACCEL_ANGLE_PITCH *ComplemantaryFilterGain2;




}/* END_FUN Read_Accurate_Angles_ComplemantrayFilter() */


