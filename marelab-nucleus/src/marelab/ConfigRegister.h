/*
 * ConfigRegister.h
 *
 *  Created on: Nov 9, 2012
 *      Author: marc philipp hammermann
 *
 */

#ifndef CONFIGREGISTER_H_
#define CONFIGREGISTER_H_
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <dlfcn.h>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstring>   // Required by strcpy()
#include <cstdlib>   // Required by malloc()
#include <vector>

#include "../json/json.h"
#include "IJsonSerializable.h"
#include "../marelab/mlog.h"

#define CONFIGFILE "marelab_cfg/marelab.cfg"

using namespace std;

class ConfigRegister {
protected:
	vector<IJsonSerializable*> registry;
	string styled_output;
public:

	void addObj(IJsonSerializable* configobj) {
		registry.push_back(configobj);
	}
	;

	/*
	 * Write all OBJ that where added to the registry
	 * to the config file
	 */
    void writeConfig(){
    	string output;
    	string temp;
    	IJsonSerializable* configObj;
    	std::ofstream out(CONFIGFILE);
    	Json::Value serializeRoot;

    	for (unsigned int i=0; i<registry.size(); i++) {
    		configObj =  registry[i];
    		configObj->Serialize(serializeRoot);
    	}

    	Json::StyledWriter writer;
    	output = writer.write(serializeRoot);
    	out << output;
    	out.close();
    }
    ;

    /*
     *  reads config file and parse it to the OBJ that
     *  are registered at the registry obj
     */
    void readConfig(){
    	Json::Value rootin;
    	Json::Reader reader;
    	IJsonSerializable* configObj;
    	ifstream in(CONFIGFILE);
    	//ifstream in(CONFIGFILE);
    	std::string input,line;

    	// Read File
    	while(getline(in, line))
    	    	  input += line + "\n";

    	// Deserialize File to Obj
    	for (unsigned int i=0; i<registry.size(); i++) {
    		configObj =  registry[i];

    	    if (ConfigRegister::Deserialize(configObj,input)){
    	    	MLOG::log("Config: parse successfull",__LINE__,__FILE__);
    	    }
    	    else{
    	    	// report to the user the failure and their locations in the document.
    	    	string conffile = CONFIGFILE;
    	    	string readererr= reader.getFormatedErrorMessages();
    	    	MLOG::log("ERROR: Failed to parse configuration: " + conffile +" "+ readererr,__LINE__,__FILE__);
    	    }
    	}
    }
    ;

    static bool Serialize(IJsonSerializable* pObj, std::string& output,Json::Value serializeRoot) {
    		if (pObj == NULL)
    			return false;

    		//Json::Value serializeRoot;
    		pObj->Serialize(serializeRoot);
    		Json::StyledWriter writer;
    		output = writer.write(serializeRoot);
    		return true;
    }
    ;

    ~ConfigRegister() {
	}
	;



	static bool SerializeAjax(IJsonSerializable* pObj, std::string& output) {
		if (pObj == NULL)
			return false;

		Json::Value serializeRoot;
		pObj->SerializeAjax(serializeRoot);

		Json::FastWriter writer;
		//Json::StyledWriter writer;
		output = writer.write(serializeRoot);

		return true;
	}
	;

	static bool Deserialize(IJsonSerializable* pObj, std::string& input) {
		if (pObj == NULL)
			return false;

		Json::Value deserializeRoot;
		Json::Reader reader;

		if (!reader.parse(input, deserializeRoot))
			return false;

		pObj->Deserialize(deserializeRoot);

		return true;
	}
	;
	ConfigRegister(void) {
	}
	;

	//Config getConfigObj(string name){
	//	return 	(Config) NULL;
	//};

};
#endif /* CONFIGREGISTER_H_ */
