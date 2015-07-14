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

int red_led = 0;
int green_led = 0;
int blue_led = 0;

//Cgi that turns the LED on or off according to the 'led' param in the POST data
int ICACHE_FLASH_ATTR cgiLedRGB(HttpdConnData *connData) {
	int len;
	char buff[128];
	int gotcmd=0;
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->getArgs, "status", buff, sizeof(buff));
	if (len>0) {
		os_printf(buff);
		red_led = ((buff[0]-48)*100+(buff[1]-48)*10+(buff[2]-48))-100;
		green_led = ((buff[3]-48)*100+(buff[4]-48)*10+(buff[5]-48))-100;
		blue_led = ((buff[6]-48)*100+(buff[7]-48)*10+(buff[8]-48))-70;
		os_printf("red = %d",red_led);
		os_printf("green = %d",green_led);
		os_printf("blue = %d",blue_led);
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


