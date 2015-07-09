/*
 * env.c
 *
 *  Created on: Jul 6, 2015
 *      Author: USAF_RJohnson
 */

#include <esp8266.h>
#include "env.h"
#include "user_config.h"


int ICACHE_FLASH_ATTR cgiEnv(HttpdConnData *connData) {
	char buff[2048];
	int tempF = 0;
	int hudP = 0;
	int len=0;

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "application/json");
	httpdHeader(connData, "Access-Control-Allow-Origin", "*");
	httpdEndHeaders(connData);

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	//os_strcpy(buff, "Unknown");
	//os_strcpy(temp, "N/A");
	//os_strcpy(humi, "N/A");

	len=httpdFindArg(connData->getArgs, "param", buff, sizeof(buff));
	if (len>0) {
		if(os_strcmp(buff,"temp")==0) {
			len=os_sprintf(buff, "%d",tempF);
			httpdSend(connData, buff, -1);
			tempF = tempF + 5;
		}
		if(os_strcmp(buff,"hud")==0) {
			len=os_sprintf(buff, "%d",hudP);
			httpdSend(connData, buff, -1);
			hudP = hudP +10;
		}
	}
	return HTTPD_CGI_DONE;
}



