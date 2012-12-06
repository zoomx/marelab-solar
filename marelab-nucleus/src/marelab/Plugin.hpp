/*
 * Plugin.hpp
 *
 *  Created on: May 5, 2012
 *      Author: marelab
 */

#ifndef PLUGIN_HPP_
#define PLUGIN_HPP_
#include <cstdio>
#include <string>

#include "../json/json.h"
#include "IJsonSerializable.h"

using namespace std;

class Plugin : public IJsonSerializable{
protected:
     int adress;

public:
    Plugin(){}
    virtual ~Plugin(){}
    virtual string getName()=0;
    virtual string getVersion()=0;
    virtual void work(tm calltime) = 0;
    virtual void Serialize( Json::Value& root ) = 0;
    virtual void Deserialize( Json::Value& root) = 0;
    virtual void SerializeAjax( Json::Value& root ) = 0;
    virtual void GetConfigAsJSON( string& configval) = 0;
    virtual void SetConfigAsJSON( Json::Value& para) = 0;
    //virtual void SetLogger( void* logger ) =0;
};

// the types of the class factories
typedef Plugin* create_t();
typedef void destroy_t(Plugin*);

#endif


