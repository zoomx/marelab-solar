/*
 * IJsonSerializable.h
 *
 *  Created on: 26.02.2012
 *      Author: ths
 */

#ifndef IJSONSERIALIZABLE_H_
#define IJSONSERIALIZABLE_H_

class IJsonSerializable
{
public:
   virtual ~IJsonSerializable(){};
   virtual void Serialize( Json::Value& root ) =0;
   virtual void Deserialize( Json::Value& root) =0;
   virtual void SerializeAjax( Json::Value& root ) =0;
};


#endif /* IJSONSERIALIZABLE_H_ */
