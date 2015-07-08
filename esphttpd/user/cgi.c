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

char currGPIO0State = 0;
char currGPIO4State = 0;
char currGPIO5State = 0;
char currGPIO12State = 0;

//cause I can't be bothered to write an ioGetLed()
static char currLedState=0;

//Cgi that turns the LED on or off according to the 'led' param in the POST data
int ICACHE_FLASH_ATTR cgiLed(HttpdConnData *connData) {
	int len;
	char buff[128];
	int gotcmd=0;
	os_printf("\nFCN Called!\n");
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->getArgs, "relay1", buff, sizeof(buff));
	if (len>0) {
		os_printf("\nR1!\n");
		currGPIO0State=atoi(buff);
		os_printf("\nRelay val = %d\n",currGPIO0State);
		ioLed(currGPIO0State);
		gotcmd=1;
	}

	len=httpdFindArg(connData->getArgs, "relay2", buff, sizeof(buff));
	if (len>0) {
		os_printf("\nR2!\n");
		currGPIO4State=atoi(buff);
		ioLed2(currGPIO4State);
		gotcmd=1;
	}

	len=httpdFindArg(connData->getArgs, "relay3", buff, sizeof(buff));
	if (len>0) {
		os_printf("\nR3!\n");
		currGPIO5State=atoi(buff);
		ioLed3(currGPIO5State);
		gotcmd=1;
	}

	len=httpdFindArg(connData->getArgs, "relay4", buff, sizeof(buff));
	if (len>0) {
		currGPIO12State=atoi(buff);
		ioLed(currGPIO12State);
		gotcmd=1;
	}

	if(gotcmd==1) {
			//sysCfg.relay_1_state=currGPIO0State;
			//sysCfg.relay_2_state=currGPIO4State;
			//sysCfg.relay_3_state=currGPIO5State;
			//sysCfg.relay_4_state=currGPIO12State;

		httpdRedirect(connData, "relay.tpl");
		return HTTPD_CGI_DONE;
	} else { //with no parameters returns JSON with relay state

		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", "text/json");
		httpdHeader(connData, "Access-Control-Allow-Origin", "*");
		httpdEndHeaders(connData);

		len=os_sprintf(buff, "{\"relay1\": %d\n,\"relay1name\":\"%s\",\n\"relay2\": %d\n,\"relay2name\":\"%s\",\n\"relay3\": %d\n,\"relay3name\":\"%s\",\n\"relay4\": %d\n,\"relay4name\":\"%s\"}\n",  currGPIO0State,"Lamp",currGPIO4State,"Fan",currGPIO5State,"Keven's Demo",currGPIO12State,"woot" );
		httpdSend(connData, buff, -1);
		return HTTPD_CGI_DONE;
	}
}



//Template code for the led page.
int ICACHE_FLASH_ATTR tplLed(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return HTTPD_CGI_DONE;

	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "ledstate")==0) {
		if (currLedState) {
			os_strcpy(buff, "on");
		} else {
			os_strcpy(buff, "off");
		}
	}
	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}

static long hitCounter=0;

//Template code for the counter on the index page.
int ICACHE_FLASH_ATTR tplCounter(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return HTTPD_CGI_DONE;

	if (os_strcmp(token, "counter")==0) {
		hitCounter++;
		os_sprintf(buff, "%ld", hitCounter);
	}
	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}
