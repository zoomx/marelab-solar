/*
 * LConnection.h
 *
 *  Created on: 27.03.2013
 *      Author: marelab
 */

#ifndef LCONNECTION_H_
#define LCONNECTION_H_
#include <vector>
#include <string>
#include <iostream>
#include "json/json.h"

namespace std {

enum CONNECTION_TYP{IO, DA, AD};
enum CONNECTION_DIR{IN, OUT, BI};

typedef enum CONNECTION_TYP hardwaretype;
typedef enum CONNECTION_DIR direction;

class Connector {     		/* Defines a phy connection the plugin needs */
public:
	Connector(	hardwaretype   vhdType,string vdescription,direction vdir){
		hdType 		= vhdType;
		description = vdescription;
		dir			= vdir;
	};
	hardwaretype   hdType;
    string description;     /* Description for the connector */
    direction dir;
  };


class LConnection {
private:

public:
	vector<Connector*> 	connectorList;			// List of connections
	LConnection(){};
	virtual ~LConnection(){
		Clear();
	};

	void Add(Connector* connector){
		connectorList.push_back(connector);
	};

	void Clear(){
		for (unsigned int i =0;i< connectorList.size();i++)
			delete connectorList[i];
		connectorList.clear();
	};

	int Count(){
		return connectorList.size();
	};

	Connector* Get(int id){
		return connectorList[id];
	};

	void DebugConnectorList(){
		for (unsigned int i =0;i < connectorList.size();i++){
			cout << "ConnectorListe " + connectorList[i]->description << endl;
		}
	}

	Json::Value ConGetConnectors(){
		Json::Value jsonConnectorList = Json::nullValue;
		Json::Value jsonConnectorItem = Json::nullValue;
		for (unsigned int i =0;i < connectorList.size();i++){
			jsonConnectorItem["HWTYPE"] 		=  connectorList[i]->hdType;
			jsonConnectorItem["DESCRIPTION"]	=  connectorList[i]->description;
			jsonConnectorItem["DIR"]			=  connectorList[i]->dir;
			jsonConnectorList.append(jsonConnectorItem);
		}
		return jsonConnectorList;
	}
};

} /* namespace std */
#endif /* LCONNECTION_H_ */

