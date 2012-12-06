/*
 * IncomingMsg.cpp
 *
 *  Created on: 26.02.2012
 *      Author: ths
 */

//#include "LedString.h"
//#include "LedTimerListe.h"
#include "IncomingMsg.h"
#include "IJsonSerializable.h"

IncomingMsg::IncomingMsg()
{

}
IncomingMsg::~IncomingMsg()
{

}

void IncomingMsg::SerializeAjax( Json::Value& root )
{

}

void IncomingMsg::Serialize( Json::Value& root )
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


Json::Value IncomingMsg::getCommand() const
{
    return command;
}

Json::Value IncomingMsg::getParameter() const
{
    return parameter;
}

Json::Value IncomingMsg::getParameter(string parametername) const
{
	if (parameter.isMember(parametername))
		return parameter.get("parametername","");
	else
	    return parameter.null;
}

void IncomingMsg::setCommand(Json::Value command)
{
    this->command = command;
}

void IncomingMsg::setParameter(Json::Value parameter)
{
    this->parameter = parameter;
}

void IncomingMsg::Deserialize( Json::Value& root )
{
	command = root.get("COMMAND","");
	parameter = root.get("PARAMETER","");
	//cout << "PARA beim Parsen:"<< parameter.get("PARAMETER","") << endl;
	plugin =  parameter.get("PARAMETER","").asString();
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
string IncomingMsg::getPlugin(){
	return plugin;
}







