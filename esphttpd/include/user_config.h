#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define USE_WIFI_MODE		STATIONAP_MODE
#define WIFI_CLIENTSSID		"Johnson"
#define WIFI_CLIENTPASSWORD	"suntemplesquare"
#define WIFI_AP_NAME		"ESP8266"
#define WIFI_AP_PASSWORD	"00000000"
#define PLATFORM_DEBUG		true

typedef struct{
char relay_1_state;
char relay_2_state;
char relay_3_state;
char relay_4_state;
}sysCfg;


#endif
