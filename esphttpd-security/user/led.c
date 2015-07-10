/*
 * env.c
 *
 *  Created on: Jul 6, 2015
 *      Author: USAF_RJohnson
 */

#include "led.h"

#include <esp8266.h>
#include "io.h"

int  red_led;
int  green_led;
int  blue_led;

int ICACHE_FLASH_ATTR cgiRGB(HttpdConnData *connData) {
	int len;
	char buff[128];
	int gotcmd=0;
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->getArgs, "green", buff, sizeof(buff));
	if (len>0) {
		os_printf("\nRd!\n");
		red_led=atoi(buff);
		gotcmd=1;
	}

	len=httpdFindArg(connData->getArgs, "green", buff, sizeof(buff));
	if (len>0) {
		os_printf("\nGN!\n");
		green_led=atoi(buff);
		gotcmd=1;
	}

	len=httpdFindArg(connData->getArgs, "blue", buff, sizeof(buff));
	if (len>0) {
		os_printf("\nBl!\n");
		blue_led=atoi(buff);
		gotcmd=1;
	}

	if(gotcmd==1) {

		httpdRedirect(connData, "relay.tpl");
		return HTTPD_CGI_DONE;
	} else { //with no parameters returns JSON with relay state

		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", "text/json");
		httpdHeader(connData, "Access-Control-Allow-Origin", "*");
		httpdEndHeaders(connData);

		len=os_sprintf(buff, "{\"relay1\": %d\n,\n\"relay2\": %d\n,\n\"relay3\": %d\n}\n", red_led,green_led,blue_led);
		httpdSend(connData, buff, -1);
		return HTTPD_CGI_DONE;
	}
}



