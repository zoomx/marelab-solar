/*
 * MsgOut.cpp
 *
 *  Created on: 27.02.2012
 *      Author: ths
 */

#include "MsgOut.h"

namespace std {

MsgOut::MsgOut() {
	// TODO Auto-generated constructor stub

}

MsgOut::~MsgOut() {
	// TODO Auto-generated destructor stub
}



void MsgOut::Serialize( Json::Value& root )
{
	/*
   // serialize primitives
	int size = LD_StringListe.size();
	Json::Value timer_value(Json::arrayValue); // []
	//arr_value.append("Test1");
	//arr_value.append("Test2");
	Json::Value obj_value(Json::objectValue);
	for(int i=0; i<size; i++)
	{
		obj_value["LedString"]["LDNAME"] = LD_StringListe[i].getLdName();
		obj_value["LedString"]["LDNUMBER"] = LD_StringListe[i].getLdNumber();
		obj_value["LedString"]["LDI2CCHANNEL"] = LD_StringListe[i].getLdI2cChannel();
		for (int ii=0;ii<TIMERSTORECOUNT;ii++)
		{
			obj_value["LedString"]["LDTIMEARRAY"].append(LD_StringListe[i].getLdTimeArray()[ii]);
		}
		root["LedListe"].append(obj_value);
		obj_value.clear();
		obj_value["LedString"]["LDTIMEARRAY"].clear();
	}
	// version tag into stream
	root["version"] = c_fileversion;
	*/
}


void MsgOut::Deserialize( Json::Value& root )
{
	/*
	string version =  root.get("version", "").asString();

	// Delete of the list
	this->Clear();

	int countLedLeisten = root.get("LedListe", "").size();
	cout << "listen  size =" << countLedLeisten;
	for(int i=0; i<countLedLeisten; i++)
	{
		LedString ledstring;
		//cout << root.get("LedListe", "")[i].get("LedString","").get("LDNAME","").asString() << "\n";
		ledstring.setLdName(root.get("LedListe", "")[i].get("LedString","").get("LDNAME","").asString());
		ledstring.setLdI2cChannel(root.get("LedListe", "")[i].get("LedString","").get("LDI2CCHANNEL","").asInt());
		ledstring.setLdNumber(root.get("LedListe", "")[i].get("LedString","").get("LDNUMBER","").asInt());
		for (int ii=0;ii<TIMERSTORECOUNT;ii++)
		{
			ledstring.setLdTimeArray( ii,root.get("LedListe", "")[i].get("LedString","").get("LDTIMEARRAY","")[ii].asInt());
		}
		this->AddLed(ledstring);
	}

   m_nTestInt = root.get("LedString",0).asInt();
   m_fTestFloat = root.get("LedString", 0.0).asDouble();
   m_TestString = root.get("LedString", "").asString();
   m_bTestBool = root.get("LedString", false).asBool();
*/
}



} /* namespace std */
