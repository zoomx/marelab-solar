/*
 * ipccom.cpp
 *
 *  Created on: 28.02.2012
 *      Author: ths
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
