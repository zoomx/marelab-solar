/*
 * mconfig.h
 *
 *  Created on: Nov 13, 2012
 *      Author: marelab
 */

#ifndef MCONFIG_H_
#define MCONFIG_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cwctype>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include <string>
namespace std
{

   extern string  loggmsg ;				// Central Log message string that is send to the client

   static string i2str(int i) {
   		std::ostringstream buffer;
   		buffer << i;
   		return buffer.str();
   }



   static string encodeForJSON( const string &sSrc )
   {
       ostringstream sRet;

       for( string::const_iterator iter = sSrc.begin(); iter!=sSrc.end(); iter++ )
       {
            unsigned char c = (unsigned char)*iter;

            switch( c )
            {
                case '"':
                	sRet << "'";
                	break;

                case '{':
                     sRet << "-";
                     break;
                case '}':
                     sRet << "-";
                     break;
                case '[':
                     sRet << "-";
                     break;
                case ']':
                     sRet << "-";
                     break;
                case '\r':
                     sRet << "<br id='loginfo'>";
                     break;
                case '\n':
                     sRet << "<br id='loginfo'>";
                     break;
                default:
                    sRet << c;
                    break;
            }
       }
       return sRet.str();
   }

}

#endif /* MCONFIG_H_ */
