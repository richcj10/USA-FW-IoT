/*
 * env.h
 *
 *  Created on: Jul 6, 2015
 *      Author: USAF_TPARSH
 */

#ifndef USER_ENV_H_
#define USER_ENV_H_

#include "httpd.h"

int cgiEnv(HttpdConnData *connData);

extern int tempF;
extern int hudP;

#endif /* USER_ENV_H_ */
