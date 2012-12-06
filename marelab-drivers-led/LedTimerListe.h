/*
 * LedTimerListe.h
 *
 *  Created on: 28.02.2012
 *      Author: ths
 */

#ifndef LEDTIMERLISTE_H_
#define LEDTIMERLISTE_H_
#include <iostream>
#include <cstring>   // Required by strcpy()
#include <cstdlib>   // Required by malloc()
#include <vector>

#include "json/json.h"
#include "../marelab-nucleus/src/marelab/IJsonSerializable.h"
#include "../marelab-nucleus/src/marelab/CJsonSerializer.h"
#include "LedString.h"

#define TIMERSTORECOUNT 48

namespace std {

class LedTimerListe  : public IJsonSerializable
{
private:
	vector<LedString> LD_StringListe;
	vector<int> lastPower;
	string filestore;

public:
	LedTimerListe(void){};
    ~LedTimerListe( void );
    virtual void Serialize( Json::Value& root );
    virtual void Deserialize( Json::Value& root);
    virtual void SerializeAjax( Json::Value& root);
    void SetFileStore(string fileStore);
    void SaveFile();
	void AddLed(LedString ledstring);
	void AddChangeLed(string led_number, string led_channel, string led_name, string led_intensity);
	vector<string> split(const string& strValue, char separator);
	int DelLed(int led_number);
	int CountLed();
	void printLedListe();
	void Clear();
	LedString* GetLed(int led_number);
	int GetPowerValue(int led_number,tm *time2Check);
};

} /* namespace std */
#endif /* LEDTIMERLISTE_H_ */
