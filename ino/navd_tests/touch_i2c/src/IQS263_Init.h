/*
* This file contains all the necessary settings for the IQS263 and this file can
* be changed from the GUI or edited here
* File:   IQS263_init.h
* Author: Azoteq
*/

#ifndef IQS263_INIT_H
#define IQS263_INIT_H

/* Used to switch Projected mode & set Global Filter Halt (0x01 Byte1) */
#define SYST_SET					0x00

/* Enable / Disable system events (0x01 Byte2)*/
#define SYSTEM_EVENTS_VAL					0x00

/*
* Change the Multipliers & Base values (0x07 in this order)
* Please note that these values are affected by the Auto ATI routine and should
* only be written in the case of a specific setup.  Alternatively Auto ATI
* should be manually called after writing these settings.
* This is applicable for both Multipliers and Compensation.
*/
#define CH0_MULT 					0x25
#define CH1_MULT						0x17
#define CH2_MULT					0x16
#define CH2_MULT					0x17
#define CH_BASE							0x12

/*
* Change the Compensation for each channel (0x08 in this order)
* Please note that these values are affected by the Auto ATI routine and should
* only be written in the case of a specific setup.  Alternatively Auto ATI
* should be manually called after writing these settings.
* This is applicable for both Multipliers and Compensation.
*/
#define COMPENSATION_CH0					0x99
#define COMPENSATION_CH1					0xA6
#define COMPENSATION_CH2					0xAC
#define COMPENSATION_CH3					0xA6

/* Change the Prox Settings or setup of the IQS263 (0x09 in this order) */
#define PROX_SET0					0x00
#define PROX_SET1					0x19
#define PROX_SET2					0x00
#define PROX_SET3					0x00
#define EVENT_MASK                                      0xFF

/* Change the Thresholds for each channel (0x0A in this order) */
#define PROX_THR					0x04
#define TOUCH_THR1					0x0A
#define TOUCH_THR2					0x0A
#define TOUCH_THR3					0x0A
#define MOV_THR					0x03
#define MOV_DEB						0x00
#define HALT_TIME							0x14
#define I2C_TIMEOUT							0x04

/* Change the Timing settings (0x0B in this order) */
#define LP_TIME							0x00
#define TARGET_T					0x40
#define TARGET_P						0x80
#define TAP_TIM							0x05
#define FLICK_TIM							0x00
#define FLICK_THR					0x32

/* Set Active Channels (0x0D) */
#define ACT_CH0						0x0F

#endif	/* IQS263_INIT_H */
