/*
 * tempsensor.hpp
 *
 *  Created on: May 5, 2012
 *      Author: marelab
 */

#ifndef AdapterModbus_HPP_
#define AdapterModbus_HPP_



using namespace std;
#include <string>
#include <time.h>
#include "Plugin.hpp"
#include "plugin-adapterconfig.h"


class AdapterModbus : public Plugin{
protected:



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
  }


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



  //IO
  virtual Json::Value hardwareToUse(){
  }
  virtual void Serialize( Json::Value& root ){
  }
  virtual void Deserialize( Json::Value& root){
  }
  virtual void SerializeAjax( Json::Value& root ){
  }
  virtual void GetConfigAsJSON( string& configval){
  }
  virtual void SetConfigAsJSON( Json::Value& para){
  }

};


// the class factories
extern "C" AdapterModbus* create() {
    return new AdapterModbus;
}

extern "C" void destroy(AdapterModbus* p) {
    delete p;
}
#endif



