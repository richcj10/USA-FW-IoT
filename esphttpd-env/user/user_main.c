/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

/*
This is example code for the esphttpd library. It's a small-ish demo showing off 
the server, including WiFi connection management capabilities, some IO and
some pictures of cats.
*/

#include <esp8266.h>
#include "httpd.h"
#include "io.h"
#include "httpdespfs.h"
#include "cgi.h"
#include "cgiwifi.h"
#include "cgiflash.h"
#include "stdout.h"
#include "auth.h"
#include "espfs.h"
#include "captdns.h"
#include "webpages-espfs.h"
#include "env.h"
#include "dht22.h"

os_timer_t mytimer;

LOCAL void ICACHE_FLASH_ATTR setup_wifi_st_mode(void)
{
	wifi_set_opmode(STATION_MODE);
	struct station_config stconfig;
	wifi_station_disconnect();
	wifi_station_dhcpc_stop();
	if(wifi_station_get_config(&stconfig))
	{
		os_memset(stconfig.ssid, 0, sizeof(stconfig.ssid));
		os_memset(stconfig.password, 0, sizeof(stconfig.password));
		//os_sprintf(stconfig.ssid, "%s", WIFI_CLIENTSSID);
		//os_sprintf(stconfig.password, "%s", WIFI_CLIENTPASSWORD);
		if(!wifi_station_set_config(&stconfig))
		{
			os_printf("ESP8266 not set station config!\r\n");
		}
	}
	wifi_station_connect();
	wifi_station_dhcpc_start();
	wifi_station_set_auto_connect(1);
	os_printf("ESP8266 in STA mode configured.\r\n");
}

//The example can print out the heap use every 3 seconds. You can use this to catch memory leaks.
//#define SHOW_HEAP_USE

//Function that tells the authentication system what users/passwords live on the system.
//This is disabled in the default build; if you want to try it, enable the authBasic line in
//the builtInUrls below.
int myPassFn(HttpdConnData *connData, int no, char *user, int userLen, char *pass, int passLen) {
	if (no==0) {
		os_strcpy(user, "admin");
		os_strcpy(pass, "s3cr3t");
		return 1;
//Add more users this way. Check against incrementing no for each user added.
//	} else if (no==1) {
//		os_strcpy(user, "user1");
//		os_strcpy(pass, "something");
//		return 1;
	}
	return 0;
}

#ifdef ESPFS_POS
CgiUploadEspfsParams espfsParams={
	.espFsPos=ESPFS_POS,
	.espFsSize=ESPFS_SIZE
};
#endif

/*
This is the main url->function dispatching data struct.
In short, it's a struct with various URLs plus their handlers. The handlers can
be 'standard' CGI functions you wrote, or 'special' CGIs requiring an argument.
They can also be auth-functions. An asterisk will match any url starting with
everything before the asterisks; "*" matches everything. The list will be
handled top-down, so make sure to put more specific rules above the more
general ones. Authorization things (like authBasic) act as a 'barrier' and
should be placed above the URLs they protect.
*/
HttpdBuiltInUrl builtInUrls[]={
	{"*", cgiRedirectApClientToHostname, "esp8266.local"},
	{"/", cgiRedirect, "/index.html"},
	{"/enviroment.html ", cgiRedirect, NULL},
	//{"/flash.bin", cgiReadFlash, NULL},
	//{"/led.tpl", cgiEspFsTemplate, tplLed},
	//{"/index.tpl", cgiEspFsTemplate, tplCounter},
	{"/chartdata.cgi", cgiEnv, NULL},
#ifdef ESPFS_POS
	{"/updateweb.cgi", cgiUploadEspfs, &espfsParams},
#endif
	//Routines to make the /wifi URL and everything beneath it work.

//Enable the line below to protect the WiFi configuration with an username/password combo.
//	{"/wifi/*", authBasic, myPassFn},

	{"/wifi", cgiRedirect, "/wifi/wifi.tpl"},
	{"/wifi/", cgiRedirect, "/wifi/wifi.tpl"},
	{"/wifi/wifiscan.cgi", cgiWiFiScan, NULL},
	{"/wifi/wifi.tpl", cgiEspFsTemplate, tplWlan},
	{"/wifi/connect.cgi", cgiWiFiConnect, NULL},
	{"/wifi/connstatus.cgi", cgiWiFiConnStatus, NULL},
	{"/wifi/setmode.cgi", cgiWiFiSetMode, NULL},

	{"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
	{NULL, NULL, NULL}
};


#ifdef SHOW_HEAP_USE
static ETSTimer prHeapTimer;

static void ICACHE_FLASH_ATTR prHeapTimerCb(void *arg) {
	os_printf("Heap: %ld\n", (unsigned long)system_get_free_heap_size());
}
#endif

void readData(){
	float lastTemp, lastHum;
	struct dht_sensor_data* r;
	r = DHTRead();
	lastTemp = r->temperature;
	lastHum = r->humidity;
	os_printf("Temp = %f Hud = %f\n",lastTemp,lastHum);
}

//Main routine. Initialize stdout, the I/O, filesystem and the webserver and we're done.
void user_init(void) {

	if(wifi_get_opmode() != STATION_MODE)
	{
		//os_printf("ESP8266 is %s mode, restarting in %s mode...\r\n", WiFiMode[wifi_get_opmode()], WiFiMode[STATION_MODE]);
		//setup_wifi_st_mode();
	}
	if(wifi_get_phy_mode() != PHY_MODE_11N)
		wifi_set_phy_mode(PHY_MODE_11N);
	if(wifi_station_get_auto_connect() == 0)
		wifi_station_set_auto_connect(1);

	stdoutInit();
	ioInit();
	captdnsInit();
	DHTInit(SENSOR_DHT11, 30000);

	// 0x40200000 is the base address for spi flash memory mapping, ESPFS_POS is the position
	// where image is written in flash that is defined in Makefile.
#ifdef ESPFS_POS
	espFsInit((void*)(0x40200000 + ESPFS_POS));
#else
	espFsInit((void*)(webpages_espfs_start));
#endif
	httpdInit(builtInUrls, 80);

	os_timer_disarm(&mytimer);
	os_timer_setfn(&mytimer, readData, NULL);
	os_timer_arm(&mytimer, 1500, 1);
	os_printf("\nReady\n");
}


void user_rf_pre_init() {
	//Not needed, but some SDK versions want this defined.
}
