/*
 * ipccom.h
 *
 *  Created on: 28.02.2012
 *      Author: ths
 */
#include <string>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include "ConfigMarelab.h"

#ifndef IPCCOM_H_
#define IPCCOM_H_

#define TRANSFER_BUFFER 9192

namespace std {

class ipccom {
private:
	struct sockaddr_un local;
	struct sockaddr_un remote;
	int socketid;
	int socketret;
	char sockbuf[TRANSFER_BUFFER];
	ConfigMarelab *configMarelab;
	string rcvString;


public:
	ipccom(ConfigMarelab *configMarelab);
	void openServer();
	void openClient();

	bool recvSock();
	bool recvSockClient();
	bool sendSock(string bytesToSend);
	bool sendSockServer(string bytesToSend);
	string getMsg();

	virtual ~ipccom();
};

} /* namespace std */
#endif /* IPCCOM_H_ */
