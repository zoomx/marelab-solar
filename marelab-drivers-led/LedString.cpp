/*
 ============================================================================
 Name        : marelab-solar-deamon.c
 Author      :
 Version     :
 Copyright   : Marc Philipp Hammermann (c) 2012
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <cstdio>
#include <vector>
#include "LedString.h"

#define c_fileversion "0.1"

int LedString::getLdI2cChannel() const
{
    return LD_i2c_channel;
}

std::string LedString::getLdName() const
{
    return LD_name;
}

int LedString::getLdNumber() const
{
    return LD_number;
}

int* LedString::getLdTimeArray()
{
    return LD_TimeArray;
}

void LedString::setLdI2cChannel(int ldI2cChannel)
{
    LD_i2c_channel = ldI2cChannel;
}

void LedString::setLdName(std::string ldName)
{
    LD_name = ldName;
}

void LedString::setLdNumber(int ldNumber)
{
    LD_number = ldNumber;
}

void LedString::setLdTimeArray(int no, int value)
{
	LD_TimeArray[no]=value;
}

void LedString::printLedString()
{
	printf("\nLedString: %i %s I2C: %i Time/Value:\n",LD_number,LD_name.c_str(),LD_i2c_channel);
	for (int i=0; i< DAY_SENSE_POINT;i++)
		printf("%i ",LD_TimeArray[i]);

    //int ;
}

LedString::LedString(void)
{
	for (int i=0; i < DAY_SENSE_POINT;i++)
		LD_TimeArray[i]=0;
}






