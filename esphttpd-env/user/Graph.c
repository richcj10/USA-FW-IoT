/*
 * Graph.c
 *
 *  Created on: Jul 6, 2015
 *      Author: USAF_TPARSH
 */

#include <esp8266.h>
#include "Graph.h"


int ICACHE_FLASH_ATTR makeGraph(HttpdConnData *connData){
//   char out[100] = "";
//   char temp[100];
//   strcpy(out, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n");
//   strcpy(out,"<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n");
//   strcpy(out,"<g stroke=\"black\">\n");
// 	int y = rand() % 130;
// 	for (int x = 10; x < 390; x+= 10) {
// 		int y2 = rand() % 130;
// 		sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
// 		strcpy(out, temp);
// 		y = y2;
// 	}
// 	strcpy(out,"</g>\n</svg>\n");
//
//	httpdSend(connData,out, -1);
	return HTTPD_CGI_DONE;
}


