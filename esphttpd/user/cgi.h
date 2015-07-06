#ifndef CGI_H
#define CGI_H

#include "httpd.h"

int cgiLed(HttpdConnData *connData);
int tplLed(HttpdConnData *connData, char *token, void **arg);
int tplCounter(HttpdConnData *connData, char *token, void **arg);

typedef struct{
char relay_1_state;
char relay_2_state;
char relay_3_state;
char relay_4_state;
}sysCfg;

#endif
