/*
 * comm.h
 *
 * Communication Protocol used for Pong, use of hardware components
 * to be specified in bluetooth.h
 *
 *  Created on: Dec 27, 2017
 *      Author: dipsy
 */

#ifndef INC_COMM_H_
#define INC_COMM_H_

#include <vector>
#include <functional>

#include <libbase/k60/mcg.h>
#include <libsc/system.h>
#include <libbase/misc_types.h>

//#include "debug.h"

using std::vector;
using std::function;
using libsc::System;

class Pong;

class Comm {
public:
	/**
	 * Package Types (better looking of 0-11 integers)
	 */
	enum PkgType{
		kStart=0,
		kStartACK=1,
		kMasterPlatform=2,
		kMasterPlatformACK=3,
		kSlavePlatform=4,
		kSlavePlatformACK=5,
		kReflection=6,
		kReflectionACK=7,
		kLocation=8,
		kLocationACK=9,
		kResult=10,
		kResultACK=11
	};

	/**
	 * Special Byte for labeling end of package
	 */
	struct BitConsts {
	    static constexpr Byte kSTART = 0xF0;
	    static constexpr Byte kEND = 0xFF;
	    static constexpr Byte kACK = 0xF1;
	};

	/**
	 * Structure of a single package
	 */
	struct Package{
		Byte frame_id;
		PkgType type;
		//vector<Byte> data;
		vector<Byte> data;
	};

	/**
	 * type of package handler function
	 */
	typedef function<void(Package)> PackageHandler;

	/**
	 * Constructor and destructor
	 */
	Comm();
	virtual ~Comm();

	/*
	 * determine the ending label of package based on pkgType
	 * return 0 is error
	 */
	Byte labelDetermine(const int t)
	{
		switch(t)
		{
		case 0:
			return Comm::BitConsts::kSTART;
		case 2: case 4: case 6: case 8: case 10:
			return Comm::BitConsts::kEND;
		case 1: case 3: case 5: case 7: case 9: case 11:
			return Comm::BitConsts::kACK;
		default:
			return 0;
		}
	}

	//TX helpers
	/**
	 * SendPackage will send package
	 * if need_ack, keep sending package until receive ack
	 * else send out the package directly
	 * tips: use a queue
	 */
	void SendPackage(const Package& pkg, bool need_ack = true);

	/**
	 * Implementation of sending out bytes shall be implemented in derived class
	 */
	virtual void SendBuffer(const Byte *buff, const size_t &size)=0;

	/**
	 * Listener (will be irs for bluetooth) when bunch of Bytes arrive
	 * You may want to parse the Byte array into packages
	 * return true if the data is consumed
	 */
	bool Listener(const Byte* data, const size_t size)
	{
		//https://stackoverflow.com/questions/1951519/when-should-i-use-stdsize-t
		size_t i = 0;
		//put data to buffer, cover from beginning
		for(std::vector<Byte>::iterator it = buffer.begin();i<size;i++, it++)
		{
			buffer.insert(it,data[i]);
		}

		//parse the byte if end of data is end label
		if(data[size-1]== BitConsts::kSTART||data[size-1]== BitConsts::kEND||data[size-1]== BitConsts::kACK)
		{
			Comm::BuildBufferPackage(size);
		}
		return true;
	}

	/**
	 * Set the package handler, which shall be called when a package is parsed
	 */
	void SetHandler(PackageHandler handler){this->Handler = handler;}

	//some variable getters
	bool IsWaitingACK(){return is_waiting_ack;}

	//tx variables
	vector<Package> queue;	//pending send package
	uint32_t send_time;		//last send time

protected:
	bool is_waiting_ack;

	/**
	 * you need to resend package per 10ms until receive ACK
	 * this function set enable this proccess or not
	 */
	virtual void EnableTimer(bool flag)=0;

	/**
	 * deliver the first package in queue
	 */
	virtual void SendFirst();

private:
	/**
	 * package handler function
	 */
	PackageHandler Handler;

	//rx veriable
	vector<Byte> buffer;	//temporary storage for incoming Bytes

	/**
	 * when the buffer contains a complete package bytes,
	 * build the package, call handler/ handle the packages directly
	 * and clear buffer
	 */
	//void BuildBufferPackage()
	void BuildBufferPackage(const size_t& size)
	{
		if(buffer.empty())return;
		if(buffer.size()<size)return;

		//check if package format
		if(buffer[size-1]!= BitConsts::kSTART||buffer[size-1]!= BitConsts::kEND||buffer[size-1]== BitConsts::kACK)
		{
			switch(buffer[size-1])
			{
			case BitConsts::kSTART:
			{
				if(size!=3)return; //check package size
				if(buffer[1]!=0)return; //check msg type (start msg is 0)
				if(buffer[0]!=0)return; //check frame ID (start frame ID is 0)

				//build package
				//Package temp_package{0,0,{}};
				Package temp_package;
				memcpy(&temp_package.frame_id,&buffer[0],1);
				memcpy(&temp_package.frame_id,&buffer[1],1);
				temp_package.data = {};

				//clear buffer
				buffer.clear();
				//call package handler
				Handler(temp_package);
				break;
			}
			case BitConsts::kACK:
			{
				if(size!=3)return; //check package size
				if(labelDetermine(buffer[1])!=BitConsts::kACK)return; //check msg type is for ack

				//build package
				Package temp_package;
				//Package temp_package{buffer[0],buffer[1],{}};
				memcpy(&temp_package.frame_id,&buffer[0],1);
				memcpy(&temp_package.frame_id,&buffer[1],1);
				temp_package.data = {};
				//clear buffer
				buffer.clear();
	////////////may not correct
				//receive ack
				is_waiting_ack = false;
				//call package handler
				Handler(temp_package);
				break;
			}
			case BitConsts::kEND:
			{
				if(size<3)return; //check package size
				if(labelDetermine(buffer[1])!=BitConsts::kEND)return; //check msg type is for ack
				//check enough data
				if(buffer[1]==2||buffer[1]==4||buffer[1]==10)
				{
					if(size!=4)return;
				}
				else if(buffer[1]==6||buffer[1]==8)
				{
					if(size!=5)return;
				}

				//build package
				Package temp_package;
				//Package temp_package{buffer[0],buffer[1],{}};
				memcpy(&temp_package.frame_id,&buffer[0],1);
				memcpy(&temp_package.frame_id,&buffer[1],1);
				temp_package.data = {};
				for(size_t i = 2; i<size;i++)
				{
					temp_package.data.push_back(buffer[i]);
				}
				//clear buffer
				buffer.clear();
				//call package handler
				Handler(temp_package);
				break;
			}
			}
		}

	/*
		//check if package format
		if(buffer[buffer.size()-1]!= BitConsts::kSTART||buffer[buffer.size()-1]!= BitConsts::kEND||buffer[buffer.size()-1]== BitConsts::kACK)
		{
			switch(buffer[buffer.size()-1])
			{
			case BitConsts::kSTART:
			{
				if(buffer.size()<3)return; //check package size
				if(buffer[buffer.size()-2]!=0)return; //check msg type (start msg is 0)
				if(buffer[buffer.size()-3]!=0)return; //check frame ID (start frame ID is 0)

				//build package
				Package temp_package{0,0,{}};
				//clear buffer
				buffer.clear;
				//call package handler
				PackageHandler(temp_package);
				break;
			}
			case BitConsts::kACK:
			{
				if(buffer.size()<3)return; //check package size
				if(labelDetermine(buffer[buffer.size()-2])!=BitConsts::kACK)return; //check msg type is for ack

				//build package
				Package temp_package{buffer[buffer.size()-3],buffer[buffer.size()-2],{}};
				//clear buffer
				buffer.clear;
				//call package handler
				PackageHandler(temp_package);
				break;
			}
			case BitConsts::kEND:
			{
				if(buffer.size()<3)return; //check package size
				if(labelDetermine(buffer[buffer.size()-2])!=BitConsts::kEND)return; //check msg type is for ack

				//build package
				//one data
				if(buffer[buffer.size()-2]==2||buffer[buffer.size()-2]==4||buffer[buffer.size()-2]==10)
				{
					Package temp_package{buffer[buffer.size()-3],buffer[buffer.size()-2],{}};
					PackageHandler(temp_package);
				}
				Package temp_package{buffer[buffer.size()-3],buffer[buffer.size()-2],{}};
				PackageHandler(temp_package);
				break;
			}
			}
		}
	*/

	}

};



#endif /* INC_COMM_H_ */
