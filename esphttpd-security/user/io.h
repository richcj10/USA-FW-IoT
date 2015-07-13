#ifndef IO_H
#define IO_H

#define PWM_CHANNEL	3  //  5:5channel ; 3:3channel

#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_MTDI_U
#define PWM_0_OUT_IO_NUM 12
#define PWM_0_OUT_IO_FUNC  FUNC_GPIO12

#define PWM_1_OUT_IO_MUX PERIPHS_IO_MUX_MTDO_U
#define PWM_1_OUT_IO_NUM 15
#define PWM_1_OUT_IO_FUNC  FUNC_GPIO15

#define PWM_2_OUT_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define PWM_2_OUT_IO_NUM 13
#define PWM_2_OUT_IO_FUNC  FUNC_GPIO13

//#define PWM_3_OUT_IO_MUX PERIPHS_IO_MUX_MTMS_U
//#define PWM_3_OUT_IO_NUM 14
//#define PWM_3_OUT_IO_FUNC  FUNC_GPIO14
//
//#define PWM_4_OUT_IO_MUX PERIPHS_IO_MUX_GPIO5_U
//#define PWM_4_OUT_IO_NUM 5
//#define PWM_4_OUT_IO_FUNC  FUNC_GPIO5

struct light_saved_param {
    uint32  pwm_period;
    uint32  pwm_duty[PWM_CHANNEL];
};

void ICACHE_FLASH_ATTR ioLed(int ena);
void ICACHE_FLASH_ATTR ioLed2(int ena);
void ICACHE_FLASH_ATTR ioLed3(int ena);
void ICACHE_FLASH_ATTR ioLed4(int ena);
void ioInit(void);

#endif
