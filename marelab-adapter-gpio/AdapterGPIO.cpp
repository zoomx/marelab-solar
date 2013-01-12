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

#ifndef AdapterGPIO_HPP_
#define AdapterGPIO_HPP_



using namespace std;
#include <string>
#include <time.h>
#include "Plugin.hpp"
#include "plugin-adapterconfig.h"


class AdapterGPIO : public Plugin{

public:

    virtual string getName(){
    	string name = pluginname;
    	return name ;
    }

    virtual string getVersion(){
    	string version = pluginversion;
    	return version;
    }

    virtual string getTypeOfPlugin(){
        	string typeofplugin = TYPE_OF_PLUGIN;
        	return typeofplugin;
    }

  virtual void work(tm calltime){
    	time_t 	tim;				// SystemTime
    	tim=time(NULL);
    	tm *timeNow = localtime(&tim);
  }

  virtual Json::Value hardwareToUse(){}

  //IO
  virtual void Serialize( Json::Value& root ){}
  virtual void Deserialize( Json::Value& root){}
  virtual void SerializeAjax( Json::Value& root ){}
  virtual void GetConfigAsJSON( string& configval){}
  virtual void SetConfigAsJSON( Json::Value& para) {}




  // Functions that triggers the hardware direkt

  virtual bool getBoolState(string varName){
	  return false;
  }

  virtual void setBoolState(string varName,bool state){
  }

  virtual string getStringState(string varName){
	  return "";
  }

  virtual void   setStringState(string varName,string state){
  }

  virtual unsigned int getIntegerState(string varName){
	  return 0;
  }
  virtual void   setIntegerState(string varName,unsigned int state){

  }

  virtual unsigned int* getArrayState(string varName){
	  return NULL;
  }

  virtual void   setArrayState(string varName,unsigned int* statearray){
  }


};


// the class factories
extern "C" Plugin* create() {
    return new AdapterGPIO;
}

extern "C" void destroy(Plugin* p) {
    delete p;
}
#endif



