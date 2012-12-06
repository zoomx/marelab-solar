/*
 * tempsensor.hpp
 *
 *  Created on: May 5, 2012
 *      Author: marelab
 */

#ifndef LEDDIMMER_HPP_
#define LEDDIMMER_HPP_



using namespace std;
#include <string>
#include <time.h>
#include "LedString.h"
#include "LedTimerListe.h"
#include "Plugin.hpp"
#include "CJsonSerializer.h"
#include "plugin-config.h"


class LedDimmer : public Plugin{
protected:
    LedTimerListe ledListe;				// List of all LED Strings


public:

    virtual string getName(){
    	string name = pluginname;
    	return name ;
    }

    virtual string getVersion(){
    	string version = pluginversion;
    	return pluginversion;
    }

   // Save the leddimmer obj to the JSON root var
   // for storing
   virtual void Serialize( Json::Value& root )
    {
	   //SetConfigAsJSON( root);
	   ledListe.Serialize(root);
    }


    virtual void Deserialize( Json::Value& root){
    	ledListe.Deserialize(root);
    }

    virtual void SerializeAjax( Json::Value& root ){
    	ledListe.SerializeAjax(root);
    }

  virtual void work(tm calltime){
    	time_t 	tim;				// SystemTime
    	tim=time(NULL);
    	tm *timeNow = localtime(&tim);
    	for (int counter=0; counter <  ledListe.CountLed(); counter++)
    		ledListe.GetPowerValue(counter,timeNow);
    }

  virtual void GetConfigAsJSON( string& configval){
	  CJsonSerializer::SerializeAjax(&ledListe, configval);
  }

  /*
   * Getting the parameter for the LED that
   * has to be stored out of the JSON request
   * LDI2CHANNEL  	-> CHANNEL for marelab control
   * LDID			-> ID in the internal marelab list
   * LDNAME			-> Name of the LED STRING
   * LDLEDARRAY		-> 48 Values for a 24h day
   */
  virtual void SetConfigAsJSON( Json::Value& para){

	cout << "FROM-PLUGIN-SAVE:"<< endl;
	// Convert recv. JSON led_leiste to a C object
  	string ldid;
  	if (para != Json::Value::null) {
  		if (para["oper"].asString().compare("del") == 0) {
  			// Delete LED Entry
  			string id2del = para["id"].asString();
  			//MLOG::log("SAVELEDROW: DELETE .. " + id2del,__LINE__,__FILE__);
  			ledListe.DelLed(atoi(id2del.c_str()));
  		}
  		else {
  			string ldname = para["LDNAME"].asString();
  			if (para["id"].asString().compare("_empty") == 0) {
  				ldid = para["id"].asString();
  			} else {
  				ldid = para["LDID"].asString();
  			}
  			string ldchannel = para["LDI2CCHANNEL"].asString();
  			string intensity = para["LDLEDARRAY"].asString();
  			ledListe.AddChangeLed(ldid, ldchannel, ldname, intensity);
  			cout << "added change" << endl;
  		}
  	}

  	string output;
  	if (CJsonSerializer::SerializeAjax(&ledListe, output)) {
  		cout << "SAVE OKAY" << endl;
  		//MLOG::log("SAVELEDROW: COMMAND ["+command+"] activated  ...",__LINE__,__FILE__);
  		// Sending Back Result for the incoming msg
  		//if (ipcs.sendSockServer("{COMMAND=SAVEROWACKOK}"))
  		//  			MLOG::log("SAVELEDROW: Send ACK OK msg to client..",__LINE__,__FILE__);
  	}
    }
  /*
  virtual void SetLogger(void (*)logger ){
	  Logger = logger;
  }
  */

};


// the class factories
extern "C" Plugin* create() {
    return new LedDimmer;
}

extern "C" void destroy(Plugin* p) {
    delete p;
}
#endif



