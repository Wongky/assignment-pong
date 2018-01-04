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
		int frame_id;
		PkgType type;
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
	Byte labelDetermine(const Comm::PkgType& t);

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
	virtual void SendBuffer(const Byte *buff, const int &size)=0;

	/**
	 * Listener (will be irs for bluetooth) when bunch of Bytes arrive
	 * You may want to parse the Byte array into packages
	 * return true if the data is consumed
	 */
	bool Listener(const Byte* data, const size_t size);

	/**
	 * Set the package handler, which shall be called when a package is parsed
	 */
	void SetHandler(PackageHandler handler){this->Handler = handler;}

	//some variable getters
	bool IsWaitingACK(){return is_waiting_ack;}

	//tx variables
	vector<Package> queue;	//pending send package
	int send_time;			//last send time

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
	void BuildBufferPackage();
};



#endif /* INC_COMM_H_ */
