/*
 ============================================================================
 Name        : marelab-solar-deamon.c
 Author      :
 Version     :
 Copyright   : Marc Philipp Hammermann (c) 2012
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#ifndef MARELAB_STRUCT_H
#define MARELAB_STRUCT_H


#include <iostream>
#include <cstring>   // Required by strcpy()
#include <cstdlib>   // Required by malloc()
#include <vector>

#include "../json/json.h"
#include "IJsonSerializable.h"
#include "CJsonSerializer.h"

#define TIMERSTORECOUNT 48


using namespace std;
/*
 * Timer stucts
 *
 */
class LedString
{
private:
    int LD_number;
    std::string LD_name;
    int LD_TimeArray[48];
    int LD_i2c_channel;
public:
    LedString(void);
    int getLdI2cChannel() const;
    std::string getLdName() const;
    int getLdNumber() const;
    int *getLdTimeArray();
    void setLdI2cChannel(int ldI2cChannel);
    void setLdName(std::string ldName);
    void setLdNumber(int ldNumber);
    void setLdTimeArray(int no, int value);
    void printLedString();
};
#endif
