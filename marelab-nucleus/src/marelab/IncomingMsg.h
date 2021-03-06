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
#ifndef INCOMINGMSG_H_
#define INCOMINGMSG_H_

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>

#include "../json/json.h"
#include "../globals/IJsonSerializable.h"

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

