
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include <esp8266.h>
#include "cgi.h"

#define LEDGPIO 13
#define BTNGPIO 2

//static ETSTimer resetBtntimer;

void ICACHE_FLASH_ATTR ioLed(int ena) {
	//gpio_output_set is overkill. ToDo: use better mactos
	if (ena) {

		gpio_output_set((1<<LEDGPIO), 0, (1<<LEDGPIO), 0);
	} else {

		gpio_output_set(0, (1<<LEDGPIO), (1<<LEDGPIO), 0);
	}
}

void ICACHE_FLASH_ATTR ioLed2(int ena) {
	//gpio_output_set is overkill. ToDo: use better mactos
	if (ena) {
		gpio_output_set((1<<4), 0, (1<<4), 0);
	} else {
		gpio_output_set(0, (1<<4), (1<<4), 0);
	}
}

void ICACHE_FLASH_ATTR ioLed3(int ena) {
	//gpio_output_set is overkill. ToDo: use better mactos
	if (ena) {
		gpio_output_set((1<<5), 0, (1<<5), 0);
	} else {
		gpio_output_set(0, (1<<5), (1<<5), 0);
	}
}

void ICACHE_FLASH_ATTR ioLed4(int ena) {
	//gpio_output_set is overkill. ToDo: use better mactos
	if (ena) {
		os_printf("\nTurn On Relay\n");
		gpio_output_set((1<<12), 0, (1<<12), 0);
	} else {
		os_printf("\nTurn OFF Relay\n");
		gpio_output_set(0, (1<<12), (1<<12), 0);
	}
}

void ICACHE_FLASH_ATTR BtnTimer(void *arg) {
	static int resetCnt=0;
	if (!GPIO_INPUT_GET(BTNGPIO)) {
		resetCnt++;
	}
	if (resetCnt>=3) { //3 sec pressed
			os_printf("\nAlarm Went off");
			ioLed4(1);
			ioLed4(0);
			ioLed4(1);
			ioLed4(0);
			ioLed4(1);
			ioLed4(0);
			ioLed4(1);
			ioLed4(0);
			alarmstate = 0;
			resetCnt=0;
	}
}

void ioInit() {
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);
	//gpio_output_set(0, 0, (1<<LEDGPIO), (1<<BTNGPIO));
	//os_timer_disarm(&resetBtntimer);
	//os_timer_setfn(&resetBtntimer, resetBtnTimerCb, NULL);
	//os_timer_arm(&resetBtntimer, 500, 1);
}

