/*
 * MPU6050.h
 *
 *  Created on: Jun 26, 2021
 *      Author: Ahmed_Ayman
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_


#include <stdint.h>
#include <stm32f429xx.h>
#include <stm32f4xx_hal.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "MPU6050_CONFIG.h"


/* here I use the I2C2 to manage my MPU communication */
extern I2C_HandleTypeDef hi2c2;


#define PRIVATRE static
#define PUPLIC

/*****************************************
 * MPU6050 ref@ REGISTER_MAP
 ***************************************/

#define MPU6050_SELF_TEST_X_R                                              0x0Du
#define MPU6050_SELF_TEST_Y_R                                              0x0Eu
#define MPU6050_SELF_TEST_Z_R                                              0x0Fu
#define MPU6050_SELF_TEST_A_R                                              0x10u
#define MPU6050_SMPLRT_DIV_R                                               0x19u
#define MPU6050_CONFIG_R                                                   0x1Au
#define MPU6050_GYRO_CONFIG_R                                              0x1Bu
#define MPU6050_ACCEL_CONFIG_R                                             0x1Cu
#define MPU6050_FIFO_EN_R                                                  0x23u
#define MPU6050_I2C_MST_CTRL_R                                             0x24u
#define MPU6050_I2C_SLV0_ADDR_R                                            0x25u
#define MPU6050_I2C_SLV0_REG_R                                             0x26u
#define MPU6050_I2C_SLV0_CTRL_R                                            0x27u
#define MPU6050_I2C_SLV1_ADDR_R                                            0x28u
#define MPU6050_I2C_SLV1_REG_R                                             0x29u
#define MPU6050_I2C_SLV1_CTRL_R                                            0x2Au
#define MPU6050_I2C_SLV2_ADDR_R                                            0x2Bu
#define MPU6050_I2C_SLV2_REG_R                                             0x2Cu
#define MPU6050_I2C_SLV2_CTRL_R                                            0x2Du
#define MPU6050_I2C_SLV3_ADDR_R                                            0x2Eu
#define MPU6050_I2C_SLV3_REG_R                                             0x2Fu
#define MPU6050_I2C_SLV3_CTRL_R                                            0x30u
#define MPU6050_I2C_SLV4_ADDR_R                                            0x31u
#define MPU6050_I2C_SLV4_REG_R                                             0x32u
#define MPU6050_I2C_SLV4_DO_R                                              0x33u
#define MPU6050_I2C_SLV4_CTRL_R                                            0x34u
#define MPU6050_I2C_SLV4_DI_R                                              0x35u
#define MPU6050_I2C_MST_STATUS_R                                           0x36u
#define MPU6050_INT_PIN_CFG_R                                              0x37u
#define MPU6050_INT_ENABLE_R                                               0x38u
#define MPU6050_INT_STATUS_R                                               0x3Au
#define MPU6050_ACCEL_XOUT_H_R                                             0x3Bu
#define MPU6050_ACCEL_XOUT_L_R                                             0x3Cu
#define MPU6050_ACCEL_YOUT_H_R                                             0x3Du
#define MPU6050_ACCEL_YOUT_L_R                                             0x3Eu
#define MPU6050_ACCEL_ZOUT_H_R                                             0x3Fu
#define MPU6050_ACCEL_ZOUT_L_R                                             0x40u
#define MPU6050_TEMP_OUT_H_R                                               0x41u
#define MPU6050_TEMP_OUT_L_R                                               0x42u
#define MPU6050_GYRO_XOUT_H_R                                              0x43u
#define MPU6050_GYRO_XOUT_L_R                                              0x44u
#define MPU6050_GYRO_YOUT_H_R                                              0x45u
#define MPU6050_GYRO_YOUT_L_R                                              0x46u
#define MPU6050_GYRO_ZOUT_H_R                                              0x47u
#define MPU6050_GYRO_ZOUT_L_R                                              0x48u
#define MPU6050_EXT_SENS_DATA_00_R                                         0x49u
#define MPU6050_EXT_SENS_DATA_01_R                                         0x4Au
#define MPU6050_EXT_SENS_DATA_02_R                                         0x4Bu
#define MPU6050_EXT_SENS_DATA_03_R                                         0x4Cu
#define MPU6050_EXT_SENS_DATA_04_R                                         0x4Du
#define MPU6050_EXT_SENS_DATA_05_R                                         0x4Eu
#define MPU6050_EXT_SENS_DATA_06_R                                         0x4Fu
#define MPU6050_EXT_SENS_DATA_07_R                                         0x50u
#define MPU6050_EXT_SENS_DATA_08_R                                         0x51u
#define MPU6050_EXT_SENS_DATA_09_R                                         0x52u
#define MPU6050_EXT_SENS_DATA_10_R                                         0x53u
#define MPU6050_EXT_SENS_DATA_11_R                                         0x54u
#define MPU6050_EXT_SENS_DATA_12_R                                         0x55u
#define MPU6050_EXT_SENS_DATA_13_R                                         0x56u
#define MPU6050_EXT_SENS_DATA_14_R                                         0x57u
#define MPU6050_EXT_SENS_DATA_15_R                                         0x58u
#define MPU6050_EXT_SENS_DATA_16_R                                         0x59u
#define MPU6050_EXT_SENS_DATA_17_R                                         0x5Au
#define MPU6050_EXT_SENS_DATA_18_R                                         0x5Bu
#define MPU6050_EXT_SENS_DATA_19_R                                         0x5Cu
#define MPU6050_EXT_SENS_DATA_20_R                                         0x5Du
#define MPU6050_EXT_SENS_DATA_21_R                                         0x5Eu
#define MPU6050_EXT_SENS_DATA_22_R                                         0x5Fu
#define MPU6050_EXT_SENS_DATA_23_R                                         0x60u
#define MPU6050_I2C_SLV0_DO_R                                              0x63u
#define MPU6050_I2C_SLV1_DO_R                                              0x64u
#define MPU6050_I2C_SLV2_DO_R                                              0x65u
#define MPU6050_I2C_SLV3_DO_R                                              0x66u
#define MPU6050_2C_MST_DELAY_CTRL_R                                        0x67u
#define MPU6050_SIGNAL_PATH_RESET_R                                        0x68u
#define MPU6050_USER_CTRL_R                                                0x6Au
#define MPU6050_PWR_MGMT_1_R                                               0x6Bu
#define MPU6050_PWR_MGMT_2_R                                               0x6Cu
#define MPU6050_FIFO_COUNTH_R                                              0x72u
#define MPU6050_FIFO_COUNTL_R                                              0x73u
#define MPU6050_FIFO_R_W_R                                                 0x74u
#define MPU6050_WHO_AM_I_R                                                 0x75u

/*****************************************
 * MPU6050 REGISTER ref@ BITS_MAPS
 ***************************************/

#define MPU6050_SELF_TEST_X_XG_MASK                                              0X0Fu
#define MPU6050_SELF_TEST_X_XA_MASK                                              0XF0u

#define MPU6050_SELF_TEST_Y_YG_MASK                                              0X0Fu
#define MPU6050_SELF_TEST_Y_YA_MASK                                              0XF0u

#define MPU6050_SELF_TEST_Z_ZG_MASK                                              0X0Fu
#define MPU6050_SELF_TEST_Z_ZA_MASK                                              0XF0u

#define MPU6050_SELF_TEST_A_XA_MASK                                              0X03u
#define MPU6050_SELF_TEST_A_YA_MASK                                              0X0Cu
#define MPU6050_SELF_TEST_A_ZA_MASK                                              0X30u

#define MPU6050_CONFIG_DLPF_DIS                                                  0x00u
#define MPU6050_CONFIG_DLPF_TEMP_L                                               0x01u
#define MPU6050_CONFIG_DLPF_GYRO_X_L                                             0x02u
#define MPU6050_CONFIG_DLPF_GYRO_Y_L                                             0x03u
#define MPU6050_CONFIG_DLPF_GYRO_Z_L                                             0x04u
#define MPU6050_CONFIG_DLPF_ACCEL_X_L                                            0x05u
#define MPU6050_CONFIG_DLPF_ACCEL_Y_L                                            0x06u
#define MPU6050_CONFIG_DLPF_ACCEL_Z_L                                            0x07u


#define MPU6050_CONFIG_EXT_SYNC_MASK                                             0x38u


#define MPU6050_FIFO_EN_SLV0                                                     0x01u
#define MPU6050_FIFO_EN_SLV1                                                     0x02u
#define MPU6050_FIFO_EN_SLV2                                                     0x04u
#define MPU6050_FIFO_EN_ACCEL_EN                                                 0x08u
#define MPU6050_FIFO_EN_GYRO_XG_EN                                               0x10u
#define MPU6050_FIFO_EN_GYRO_YG_EN                                               0x20u
#define MPU6050_FIFO_EN_GYRO_ZG_EN                                               0x40u
#define MPU6050_FIFO_EN_TEMP_EN                                                  0x80u

#define MPU6050_INT_ENABLE_DATA_RED                                              0x01u
#define MPU6050_INT_ENABLE_I2C_MASTER_INT_EN                                     0x80u
#define MPU6050_INT_ENABLE_FIFO_PVERFLOW                                         0x10u

#define MPU6050_INT_STATUS_DATA_RED                                              0x01u
#define MPU6050_INT_STATUS_I2C_MASTER_INT                                        0x80u
#define MPU6050_INT_STATUS_FIFO_PVERFLOW                                         0x10u


#define MPU6050_GYRO_CONFIG_FS_SEL_2000                                          0x18u
#define MPU6050_GYRO_CONFIG_FS_SEL_1000                                          0x10u
#define MPU6050_GYRO_CONFIG_FS_SEL_500                                           0x08u
#define MPU6050_GYRO_CONFIG_FS_SEL_250                                           0x00u

#define MPU6050_GYRO_CONFIG_ZA_ST                                                0x20u
#define MPU6050_GYRO_CONFIG_YA_ST                                                0x40u
#define MPU6050_GYRO_CONFIG_XA_ZT                                                0x80u

#define MPU6050_SIGNAL_PATH_TEMP_RESET                                           0x01u
#define MPU6050_SIGNAL_PATH_ACCEL_RESET                                          0x02u
#define MPU6050_SIGNAL_PATH_GYRO_RESET                                           0x04u


#define MPU6050_ACCEL_CONFIG_AFS_2                                               0x00u
#define MPU6050_ACCEL_CONFIG_AFS_4                                               0x08u
#define MPU6050_ACCEL_CONFIG_AFS_8                                               0x10u
#define MPU6050_ACCEL_CONFIG_AFS_16                                              0x18u


#define MPU6050_ACCEL_CONFIG_ZA_ST                                               0x20u
#define MPU6050_ACCEL_CONFIG_YA_ST                                               0x40u
#define MPU6050_ACCEL_CONFIG_XA_ZT                                               0x80u

#define MPU6050_USER_CTRL_SIG_COND_RESET				        	    		  0x01u
#define MPU6050_USER_CTRL_I2C_MST_RESET                                           0x02u
#define MPU6050_USER_CTRL_FIFO_RESET                                              0x04u
#define MPU6050_USER_CTRL_I2C_IF_DIS                                              0x10u
#define MPU6050_USER_CTRL_I2C_MST_EN                                              0x20u
#define MPU6050_USER_CTRL_FIFO_EN                                                 0x40u


#define MPU6050_DEVICE_ADDR                                                       0Xd0u

/*
 * WE DONOT NED O DO THIS STEP THE HAL LIBRARY DOES IT AUTOMATICALLY
#define MPU6050_ADDR_READ_SEQ                                                        ((0X68<<1) | 1)
#define MPU6050_ADDR_WRITE_SEQ                                                       ((0X68<<1) | 0)
*/




#define MPU6050_PWR_MGMT_1_CLKSEL_MASK                                            0X07u
#define MPU6050_PWR_MGMT_1_TEMP_DIS                                               0x40u
#define MPU6050_PWR_MGMT_1_CYCLE                                                  0x20u
#define MPU6050_PWR_MGMT_1_SLEEP                                                  0x40u
#define MPU6050_PWR_MGMT_1_DEVICE_REST                                            0x80u

#define MPU6050_USER_CTRL_SIG_RESET                                               0x01u
#define MPU6050_USER_CTRL_I2C_MASTER_RESET                                        0x02u
#define MPU6050_USER_CTRL_FIFO_RESET                                              0x04u
#define MPU6050_USER_CTRL_I2C_IF_DIS                                              0x10u
#define MPU6050_USER_CTRL_I2C_MASTER_EN                                           0x20u
#define MPU6050_USER_CTRL_FIFO_EN                                                 0x40u

#define MPU6050_sIGNAL_PATH_RESET_TEMP                                            0x01u
#define MPU6050_sIGNAL_PATH_RESET_ACCEL                                           0x02u
#define MPU6050_sIGNAL_PATH_RESET_GYRO                                            0x04u

#define MPU6050_FIFO_EN_SLV0_FIFO_EN                                              0x01u
#define MPU6050_FIFO_EN_SLV1_FIFO_EN                                              0x02u
#define MPU6050_FIFO_EN_SLV2_FIFO_EN                                              0x04u
#define MPU6050_FIFO_EN_ACCEL_FIFO_EN                                             0x08u
#define MPU6050_FIFO_EN_ZG_FIFO_EN                                                0x10u
#define MPU6050_FIFO_EN_YG_FIFO_EN                                                0x20u
#define MPU6050_FIFO_EN_XG_FIFO_EN                                                0x40u
#define MPU6050_FIFO_EN_TEMP_FIFO_EN                                              0x80u

#define MPU6050_CONFIG_FS_SEL_MAS                                                 0x18u

#define MPU6050_ACCEL_CONFIG_DLPF_MASK                                            0X07u
#define MPU6050_ACCEL_CONFIG_EXT_SYNC_MASK                                        0x38u



typedef enum {
	MPU6050_OK,
	MPU6050_ERROR
}MPU6050_status_t;


typedef struct {

	int16_t  XA_Value ;
	int16_t  YA_Value ;
	int16_t  ZA_Value  ;

	int16_t  XG_Value ;
	int16_t  YG_Value ;
	int16_t  ZG_Value ;

	int16_t  TEMP_READED ;
}MPU6050_DATA_t;


typedef struct {

	float  GYRO_ANGLE_PITCH ;
	float  GYRO_ANGLE_ROLL ;
	float  GYRO_ANGLE_YAW ;


	float  ACCEL_ANGLE_PITCH ;
	float  ACCEL_ANGLE_ROLL ;
	float  ACCEL_ANGLE_YAW ;

	float  TEMP_IN_CEL ;
}MPU6050_ANGELS_t;








void MPU6050_ALL_ACCEL_Read(MPU6050_DATA_t * ALL_Axis_Var);



MPU6050_status_t MPU6050_IsDataReady(void);




/****************************************************************************************
 * set all configurations
 *  parameter : Config_arr 2D array first byte represent addr and the second byte represent value
 *  return MPU6050_OK if this function successfully set registers
 *  you can add and remove any registers that you need to config
 *  the configuration will be in form of 2D array first byte hold the address register and the second byte hold
 *  the value that you need to assign to that register
 ****************************************************************************************/
void MPU6050_SET_Config(uint8_t * Config_arr,uint8_t array_size);






/* brief : this function used to get the GYRO angles
 * param : ANGELS_VAR_STRUCT is a data structure that in general hold all MPU Angles
 * return : void
*/
void MPU6050_ALL_GYRO_ANGELS(MPU6050_ANGELS_t *ANGELS_VAR_STRUCT);



/* brief : this function used to get the accel angles
 * param : ANGELS_VAR_STRUCT is a data structure that in general hold all MPU Angles
 * return : void
*/
void MPU6050_ALL_ACCEL_ANGELS(MPU6050_ANGELS_t *ANGELS_VAR_STRUCT);





/* brief : this function used to read the MPU GYRO register
 * param : ALL_Axis_Var is a data structure that in general hold all MPU readed data register
 * return : void
*/
void MPU6050_ALL_GYRO_Read(MPU6050_DATA_t * ALL_Axis_Var);



/* brief : this function used to read the MPU accel register
 * param : ALL_Axis_Var is a data structure that in general hold all MPU readed data register
 * return : void
*/
void MPU6050_ALL_ACCEL_Read(MPU6050_DATA_t *ALL_Axis_Var);





/* brief : this function used to get the temp value in C degree
 * param : ANGLES_VAR_STRUCT this data structure type in general used to hold the all required data that needed from MPU
 * return : void
 */
void MPU6050_Read_Temp_In_C(MPU6050_ANGELS_t *ANGLES_VAR_STRUCT);




/* brief : if you call this function you will get aall data from MPU *(GYRO - ACCAL - And tamp )
 * here also if you need to change the complemantary filter gain you caan do it in the MPU_config file
 * param : ANGELS_STRUCT_VAR return  all angles and temp
 * return : void
 */
void Read_Accurate_Angles_ComplemantrayFilter(MPU6050_ANGELS_t *ANGELS_STRUCT_VAR);



#endif /* INC_MPU6050_H_ */
