/*
 ============================================================================
 Name        : IncomingMsg.h
 Author      :
 Version     :
 Copyright   : Marc Philipp Hammermann (c) 2012
 Description : STORES THE LAST JSON MESSAGE OBJECT THAT WAS SEND TO
               THE DEAMON
 ============================================================================
 */
#ifndef INCOMINGMSG_H_
#define INCOMINGMSG_H_

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>

#include "../json/json.h"
#include "IJsonSerializable.h"
//#include "CJsonSerializer.h"


using namespace std;

class IncomingMsg : public IJsonSerializable
{
private:
	string msg;
	string plugin;
	Json::Value command;
	Json::Value parameter;

public:
	IncomingMsg();
    ~IncomingMsg();
    virtual void Serialize( Json::Value& root );
    virtual void Deserialize( Json::Value& root);
    virtual void SerializeAjax( Json::Value& root );
	string* GetMsg();
    Json::Value getCommand() const;
    Json::Value getParameter() const;
    Json::Value getParameter(string parametername) const;
    void setCommand(Json::Value command);
    void setParameter(Json::Value parameter);
    string getPlugin();
};

#endif /* INCOMINGMSG_H_ */

