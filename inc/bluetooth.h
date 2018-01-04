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
	//initialize member bluetooth and member pit in this constructor
    Bluetooth();

    //implement send buffer
    virtual void SendBuffer(const Byte *buff, const int &size);

    //getter
    bool IsTimerEnable(){return is_timer_enabled;}
private:
    JyMcuBt106 m_bt;
    Pit m_pit;

    bool is_timer_enabled=false;

    //implement enable timer
    void EnableTimer(bool flag);
};



#endif /* INC_BLUETOOTH_H_ */
