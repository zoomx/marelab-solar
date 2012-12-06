/*
 * MsgOut.h
 *
 *  Created on: 27.02.2012
 *      Author: ths
 */

#ifndef MSGOUT_H_
#define MSGOUT_H_
#include "../json/json.h"
#include "IJsonSerializable.h"
//#include "CJsonSerializer.h"

namespace std {

class MsgOut : public IJsonSerializable
{
public:
	MsgOut();
	virtual ~MsgOut();
	void Serialize( Json::Value& root );
	void Deserialize( Json::Value& root );
};

} /* namespace std */
#endif /* MSGOUT_H_ */
