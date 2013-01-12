/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Author      : Marc Philipp Hammermann
 * Version     :
 * Copyright © 2013 marc philipp hammermann  <marchammermann@googlemail.com>
 *
 *
 *
 *
 */
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
#include "ConfigMarelab.h"
#include "ipccom.h"


namespace std {


/*
 * Setup the Communictaion ipc socket
 */
ipccom::ipccom(ConfigMarelab *configMarelab) {
	this->configMarelab = configMarelab;
}

void ipccom::openServer() {
	 int len;
	 struct sockaddr_un local;

	 if ((socketid = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	 {
	   throw "Socket can't be opened";
	 }
	 else
	 {
		 local.sun_family = AF_UNIX;
		 strcpy(local.sun_path, configMarelab->getCfSockPath().c_str());
		 unlink(local.sun_path);
		 len = strlen(local.sun_path) + sizeof(local.sun_family);

		 if (bind(socketid, (struct sockaddr *)&local, len) == -1) {
			 throw "Socket can't be binded";
		 }

		 if (listen(socketid, 5) == -1) {
			 throw "Socket can't be listed";
		 }
		 syslog( LOG_ERR, "Server socket successfully created...");

	 }
}

void ipccom::openClient() {

	 int  len;

	 if ((socketid = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	 {
	   throw "Socket can't be opened";
	 }
	 else
	 {
		 remote.sun_family = AF_UNIX;
		 strcpy(remote.sun_path,  configMarelab->getCfSockPath().c_str());
		 len = strlen(remote.sun_path) + sizeof(remote.sun_family);
		 if (connect(socketid, (struct sockaddr *)&remote, len) == -1) {
			 string error =  strerror(errno);
			 throw error.c_str() ;
		 }
	 }
}


bool ipccom::recvSock(){
	int  n;
	socklen_t t;

    t = sizeof(remote);
    if ((socketret = accept(socketid, (struct sockaddr *)&remote,&t)) == -1) {
    	//string error =  strerror(errno);
    	//cout << error << endl;
    	return false;
    }
    else
    {
    	syslog( LOG_ERR, "Connection accepted...");
    	/* Connected lets get something to recv*/
    	memset( sockbuf,0,  TRANSFER_BUFFER);
        n = recv(socketret, sockbuf, TRANSFER_BUFFER, 0);
        if (n <= 0)
        {
           if (n < 0) {
        	   syslog( LOG_ERR, "Socket recv error...");
        	   close(socketret);
           }
           return false;
        }
        return true;
    }
    return false;
}

bool ipccom::recvSockClient(){
	int  n;

    memset( sockbuf,0, TRANSFER_BUFFER);
    n = recv(socketid, sockbuf, TRANSFER_BUFFER, 0);
    if (n <= 0)
    {
      if (n < 0) {
      syslog( LOG_ERR, "Socket recv error...");
      close(socketid);
      }
      return false;
    }
    return true;
}

string ipccom::getMsg()
{
	string msg;
	msg = "";
	msg = sockbuf;
	return msg;
}

bool ipccom::sendSock(string bytesToSend){
    if (send(socketid, bytesToSend.c_str(), strlen(bytesToSend.c_str()), 0) == -1)
    {
        return false;
    }
    return true;
}
bool ipccom::sendSockServer(string bytesToSend){
    if (send(socketret, bytesToSend.c_str(), strlen(bytesToSend.c_str()), 0) == -1)
    {
        return false;
    }
    return true;
}

ipccom::~ipccom() {
	close(socketret);
	close(socketid);
}

} /* namespace std */
