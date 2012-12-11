/*
 * PluginRegistry.h
 *
 *  Created on: May 5, 2012
 *      Author: marelab
 *
 *      This class loads all shared lib at runtime that are device drivers for marelab
 *      these drivers have to be in a directory that is configered over the
 *       marelabconf.h PLUGINDIR is the path constat
 */

#ifndef PLUGINREGISTRY_H_
#define PLUGINREGISTRY_H_

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <dlfcn.h>
#include "ConfigRegister.h"
#include "Plugin.hpp"
#include "ConfigMarelab.h"

using namespace std;

class PluginObject{
public:
	void* pluginHandle;
	create_t* plugin_createFunc;
	destroy_t* plugin_destroyFunc;
	Plugin* plugin;
};

class PluginRegistry : public IJsonSerializable{
private:
	vector<PluginObject*> pluginList;
	ConfigMarelab* configMarelab;

public:
	PluginRegistry(ConfigMarelab *configMarelab);
	virtual ~PluginRegistry(){
		ClearRegistry();
	}
	void ClearRegistry();
	int ScanForPlugins(); 						// scans for Plugins
	bool isFilePlugin(string filename); 		// checks file is it a plugin
	int loadPluging(string filename);			// loads a Plugin
	void PluginsAddToConfig(ConfigRegister *configRegistry);
	PluginObject* GetPluginWithName(string name);
	string JSONgetPluginFileNames();

	//PERSISTENT INTERFACE
	virtual void Serialize( Json::Value& root );
	virtual void Deserialize( Json::Value& root);
	virtual void SerializeAjax( Json::Value& root );
};



#endif /* PLUGINREGISTRY_H_ */
