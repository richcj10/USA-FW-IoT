/*
 * env.c
 *
 *  Created on: Jul 6, 2015
 *      Author: USAF_RJohnson
 */

#include <esp8266.h>
#include "env.h"
#include "user_config.h"

int  red_led;
int  green_led;
int  blue_led;

int ICACHE_FLASH_ATTR cgiEnv(HttpdConnData *connData) {
	char buff[2048];
	char temp[128];
	char humi[32];
	int len=0;

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "application/json");
	httpdHeader(connData, "Access-Control-Allow-Origin", "*");
	httpdEndHeaders(connData);

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	os_strcpy(buff, "Unknown");
	os_strcpy(temp, "N/A");
	os_strcpy(humi, "N/A");

	len=httpdFindArg(connData->getArgs, "param", buff, sizeof(buff));
	if (len>0) {
		if(os_strcmp(buff,"state")==0) {

			if(os_strcmp(buff,"red_manualsetpoint")==0) {
				if(connData->post->len>0)
				{
					red_led = (int)atoi(connData->post->buff);
					os_printf("Handle red setpoint save (%d)\n",(int)red_led);
				}
				else
				{
					//os_sprintf(buff, "%d", (int)sysCfg.thermostat1manualsetpoint);
				}
			}
			if(os_strcmp(buff,"green_manualsetpoint")==0)
			{
				if(connData->post->len>0)
				{
					green_led=(int)atoi(connData->post->buff);
					os_printf("Handle green setpoint save (%d)\n",(int)green_led);
				}
				else
				{
					//os_sprintf(buff, "%d", (int)sysCfg.thermostat1manualsetpoint);
				}
			}
			if(os_strcmp(buff,"blue_manualsetpoint")==0)
			{
				if(connData->post->len>0)
				{
					blue_led=(int)atoi(connData->post->buff);
					os_printf("Handle blue setpoint save (%d)\n",(int)blue_led);
				}
				else
				{
					//os_sprintf(buff, "%d", (int)sysCfg.thermostat1manualsetpoint);
				}
			}

		}

	}
	return HTTPD_CGI_DONE;
}



