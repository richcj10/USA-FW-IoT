/*
Some random cgi routines. Used in the LED example and the page that returns the entire
flash as a binary. Also handles the hit counter on the main page.
*/

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
#include "io.h"

char red_led = 0;
char green_led = 0;
char blue_led = 0;

//Cgi that turns the LED on or off according to the 'led' param in the POST data
int ICACHE_FLASH_ATTR cgiLedRGB(HttpdConnData *connData) {
	int len;
	char buff[128];
	int gotcmd=0;
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->getArgs, "red", buff, sizeof(buff));
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

		//httpdRedirect(connData, "relay.tpl");
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


