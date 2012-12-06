/*
 * PluginRegistry.cpp
 *
 *  Created on: May 5, 2012
 *      Author: marelab
 *
 *      This class loads all shared lib at runtime that are device drivers for marelab
 *      these drivers have to be in a directory that is configered over the
 *      marelabconf.h PLUGINDIR is the path constant
 *
 *
 *		ToDo:
 *		- Remove Hard Coded Device Lib Path
 */

#include "PluginRegistry.h"
#include <vector>
#include <string>
#include <iostream>
#include <dlfcn.h>
#include <cstring>
#include "ConfigRegister.h"
#include "mlog.h"
#include "../marelabconf.h"


using namespace std;



PluginRegistry::PluginRegistry() {
	// TODO Auto-generated constructor stub

}


bool PluginRegistry::isFilePlugin(string filename)
{
    void* pluginHandle = dlopen(filename.c_str(), RTLD_LAZY);
    if (!pluginHandle) {
    	cout << "PLUGIN Loader: File ("<< filename << ") is no Plugin :" << dlerror() << '\n';
       return false;
    }

    // load the symbols
    create_t* create_plugin = (create_t*) dlsym(pluginHandle, "create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
    	cout << "PLUGIN Loader: Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }

   destroy_t* destroy_plugin = (destroy_t*) dlsym(pluginHandle, "destroy");
   dlsym_error = dlerror();
   if (dlsym_error) {
	   cout << "PLUGIN Loader: Cannot load symbol destroy: " << dlsym_error << '\n';
       return 1;
   }
   return true;
}





/*
 * Clears the pluginList of the registry
 */
void PluginRegistry::ClearRegistry(){
	// Lets remove all entrys in the list
	// Destroy the object & clear the handle
	PluginObject *pluginobj;
	for (int i=0; i<pluginList.size(); i++) {
	    pluginobj =  pluginList[i];

	    pluginobj->plugin_destroyFunc(pluginobj->plugin);
	    dlclose(pluginobj->pluginHandle);
	}
	pluginList.clear();
}


void PluginRegistry::PluginsAddToConfig(ConfigRegister *configRegistry){
	PluginObject *pluginobj;
	for (int i=0; i<pluginList.size(); i++) {
		    pluginobj =  pluginList[i];
		    configRegistry->addObj(pluginobj->plugin);
		}
}


int PluginRegistry::loadPluging(string filename){
	    void* pluginhandle = dlopen(filename.c_str(), RTLD_LAZY);
	    if (!pluginhandle) {
	       cout << "PLUGIN Loader: File ("<< filename << ") is no Plugin :" << dlerror() << '\n';
	       return false;
	    }

	    // load the symbols
	    create_t* create_plugin = (create_t*) dlsym(pluginhandle, "create");
	    const char* dlsym_error = dlerror();
	    if (dlsym_error) {
	    	cout << "PLUGIN Loader: Cannot load symbol create: " << dlsym_error << '\n';
	        return 1;
	    }

	   destroy_t* destroy_plugin = (destroy_t*) dlsym(pluginhandle, "destroy");
	   dlsym_error = dlerror();
	   if (dlsym_error) {
		   cout << "PLUGIN Loader: Cannot load symbol destroy: " << dlsym_error << '\n';
	       return 1;
	   }

	   //polygon* poly = create_triangle();
	   // create an instance of the class
	   PluginObject *pluginobj = new PluginObject;
	   pluginobj->plugin_createFunc = create_plugin;
	   pluginobj->plugin_destroyFunc= destroy_plugin;
	   pluginobj->pluginHandle=pluginhandle;
	   pluginobj->plugin = create_plugin();

	   // use the class
	   string name = pluginobj->plugin->getName();
	   MLOG::log("Plugin: loaded pugin ["+name +"]",__LINE__,__FILE__ );
	   //pluginobj->plugin->SetLogger(&MLOG);
	   pluginList.push_back(pluginobj);
}

/*
 * Scans the plugin dir for all plugins and generates a list
 * for loading the plugins
 */
int PluginRegistry::ScanForPlugins()
{
	string dir = PLUGINDIR;
	//string dir = "/mnt/arm-marelab-dev/marelab-deepblue/marelab-drivers-led/marelab-drivers/";
	//vector<string> &files;

	DIR *dp;
	struct dirent *dirp;

	if((dp  = opendir(dir.c_str())) == NULL) {
	    cout << "PLUGIN Loader: Error opening PluginDir (" << errno << ") " << dir << endl;
	    return errno;
	 }

	while ((dirp = readdir(dp)) != NULL) {
		string filename = dirp->d_name;
		if (isFilePlugin (dir+filename))
		{
			loadPluging(dir+filename);
		}
	}
	closedir(dp);
	return 0;
}

PluginObject* PluginRegistry::GetPluginWithName(string name){
	PluginObject *pluginobj;
	for (unsigned int i=0; i < pluginList.size(); i++) {
		pluginobj =  pluginList[i];
		if (pluginobj->plugin->getName()== name)
			return pluginobj;
	}
	return NULL;
}

/*
 * Itterates throu the registry and gives back a JSON string with the
 * names of the founded plugins.
 */
string PluginRegistry::JSONgetPluginFileNames(){
	PluginObject *pluginobj;
	string jsonlist;
	string pathtoplugs = PLUGINDIR;
	jsonlist = "{";
	for (unsigned int i=0; i < pluginList.size(); i++) {
		pluginobj =  pluginList[i];
		if (i == (pluginList.size()-1) && (pluginList.size() != 1))
			jsonlist = jsonlist + "\"PLUGINNAME\":\""+pluginobj->plugin->getName()+"\",";
		else
			jsonlist = jsonlist + "\"PLUGINNAME\":\""+pluginobj->plugin->getName()+"\"";
	}
	jsonlist = jsonlist +"}";
	return jsonlist;
}

void PluginRegistry::Serialize( Json::Value& root ){

}
void PluginRegistry::Deserialize( Json::Value& root){

}
void PluginRegistry::SerializeAjax( Json::Value& root ){
	PluginObject *pluginobj;

	Json::Value obj_value(Json::objectValue);
	obj_value["COMMAND"] = "GET_PLUGININFO";

	//obj_value["PARAMETER"] = "GET_PLUGININFO";
	root["COMMAND"].append(obj_value);

	//{"COMMAND":"GET_PLUGININFO","PARAMETER":"
	for (unsigned int i=0; i < pluginList.size(); i++) {
			pluginobj =  pluginList[i];
			obj_value["PLUGINNAME"] = pluginobj->plugin->getName();
			obj_value["PLUGINVERSION"] = pluginobj->plugin->getVersion();
			root["PARAMETER"].append(obj_value);
			obj_value.clear();
	}
}
