/*
 * bluetooth.h
 *
 * bluetooth and pit implementation of comm
 *
 *  Created on: Dec 23, 2017
 *      Author: dipsy
 */

#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#include <vector>
#include <functional>

#include <libbase/k60/mcg.h>
#include <libsc/system.h>
#include "libsc/k60/jy_mcu_bt_106.h"
#include "libbase/k60/pit.h"
#include "comm.h"
#include "config.h"

using libsc::k60::JyMcuBt106;
using libbase::k60::Pit;
using std::vector;
using std::function;

class Bluetooth: public Comm {
public:
/*
    bool blueListener(const Byte* data, const size_t size)
    {
    	bool flag = Listener(data,size);

    	//disable resend if ack received;
    	if(IsTimerEnable())
    	{
    		if(!IsWaitingACK())
    		{
    			EnableTimer(false);
    		}
    	}
    	return flag;
    }
*/

	//initialize member bluetooth and member pit in this constructor
	//http://en.cppreference.com/w/cpp/utility/functional/function
	//http://en.cppreference.com/w/cpp/language/lambda
	//https://stackoverflow.com/questions/4940259/lambdas-require-capturing-this-to-call-static-member-function
	//https://stackoverflow.com/questions/16323032/why-cant-i-capture-this-by-reference-this-in-lambda
	//https://stackoverflow.com/questions/29286863/invalid-use-of-non-static-member-function
	//bool Comm::Listener(const Byte* data, const size_t size);
	Bluetooth():m_bt(Config::GetBluetoothConfig(std::function<bool(const Byte *data, const size_t size)>([this](const Byte *data, const size_t size)->bool{return this->Listener(data,size);}))),m_pit(Config::GetBluetoothPitConfig(std::function<void(Pit*)>([this](Pit*){this->SendFirst();}))){};

	//https://stackoverflow.com/questions/22750855/undefined-reference-to-method-in-parent-class
	virtual ~Bluetooth()
	{

	}

    //implement send buffer
    virtual void SendBuffer(const Byte *buff, const size_t &size)
    {
    	m_bt.SendBuffer(buff,size);
    }


    //getter
    bool IsTimerEnable(){return is_timer_enabled;}
protected:

	/**
	 * deliver the first package in queue
	 */
	virtual void SendFirst()
	{
		if(IsWaitingACK() && !IsTimerEnable())
		{
		    EnableTimer(true);
		 }
		else if(!IsWaitingACK() && IsTimerEnable())
		{
			EnableTimer(false);
		}

		if(!queue.empty())
		{
			Package temp_package = queue[0];
			//resend is not needed
			if(!IsTimerEnable())
			{
				//pop the first package
				queue.erase(queue.begin());
			}


			//http://zh.cppreference.com/w/cpp/types/size_t
			//calculate the size of buffer array to be send
			const size_t size = 3 + temp_package.data.size(); //sum of frame id , PkgType, labeling end of package, data size

			//construct send buffer
			Byte buff[size];

			//buff[0] = temp_package.frame_id;
			//buff[1] = temp_package.type;
			memcpy(buff,&temp_package.frame_id,1);
			memcpy(buff+1,&temp_package.type,1);

			int i = 2;
			Byte buffer_data;
			for (std::vector<Byte>::iterator it = temp_package.data.begin(); it != temp_package.data.end(); ++it, ++i)
			{
				//buff[i] = *it;
				buffer_data = *it;
				memcpy(buff+i,&buffer_data,1);
			}

			//buff[size-1] = labelDetermine(int(temp_package.type));
			int temp_type = labelDetermine(temp_package.type);
			memcpy(buff+(size-1),&temp_type,1);

			//set send_time should be on main?
			send_time=System::Time();
			//send buffer
			SendBuffer(buff, size);
		}
	}

private:
    JyMcuBt106 m_bt;
    Pit m_pit;

    bool is_timer_enabled=false;

    //implement enable timer
    void EnableTimer(bool flag)
    {
    	is_timer_enabled=flag;
    	m_pit.SetEnable(flag); //enable resend package per 10ms until receive ACK proccess
    }


};



#endif /* INC_BLUETOOTH_H_ */
