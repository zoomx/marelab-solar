/*
 * LedString.h
 *
 *  Created on: 02.03.2012
 *      Author: ths
 */

#ifndef LEDSTRING_H_
#define LEDSTRING_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <syslog.h>
#include <vector>
#include <string>
#include <cstdio>
#include <vector>

#include "../marelab-nucleus/src/marelab/ipccom.h"
#include "../marelab-nucleus/src/marelabconf.h"
/*
 * LedString
 *
 *
 *
 *
 *
 */
class LedString {
private:
	int LD_i2c_channel;
	std::string LD_name;
	int LD_number;
	int LD_TimeArray[DAY_SENSE_POINT];
public:
	LedString(void);
	int getLdI2cChannel() const;
	std::string getLdName() const;
	int getLdNumber() const;
	int* getLdTimeArray();
	void setLdI2cChannel(int ldI2cChannel);
	void setLdName(std::string ldName);
	void setLdNumber(int ldNumber);
	void setLdTimeArray(int no, int value);
	void printLedString();

};

#endif /* LEDSTRING_H_ */
