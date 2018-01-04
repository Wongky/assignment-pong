/*
 * config.h
 *
 * configure all the peripherals here
 *
 *  Created on: Dec 23, 2017
 *      Author: dipsy
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include <functional>

#include "libsc/led.h"
#include "libsc/joystick.h"
#include "libsc/st7735r.h"
#include "libsc/lcd_typewriter.h"
#include "libsc/lcd_console.h"
#include "libsc/battery_meter.h"
#include "libsc/k60/jy_mcu_bt_106.h"
#include "libbase/k60/pit.h"

#include "libbase/k60/uart.h"

using libsc::Led;
using libsc::Lcd;
using libsc::Joystick;
using libsc::St7735r;
using libsc::LcdTypewriter;
using libsc::LcdConsole;
using libsc::BatteryMeter;
using libsc::k60::JyMcuBt106;
using libbase::k60::Pit;

using std::function;

class Config{
public:

	static Led::Config GetLedConfig(int id){
		Led::Config config;
		config.id = id;
		return config;
	}

    static Joystick::Config GetJoystickConfig() {
        //TODO: finish it
    }

    static St7735r::Config GetLcdConfig() {
        //TODO: finish it
    }

    static JyMcuBt106::Config GetBluetoothConfig(std::function<bool(const Byte *data, const size_t size)> isr) {
        //TODO: finish it
    	JyMcuBt106::Config uartConfig;
    	uartConfig.id = 0;
    	uartConfig.baud_rate = libbase::k60::Uart::Config::BaudRate::k115200;
    	uartConfig.rx_isr = isr;
    	return uartConfig;
    }

    static Pit::Config GetBluetoothPitConfig(std::function<void(Pit*)> isr){
    	//TODO: finish it
    	Pit::Config pitConfig;
    	pitConfig.channel = 0;
    	pitConfig.count = 75000*250; //job executed once per 250ms
    	pitConfig.isr = isr;
    	return pitConfig;
    }
};


#endif /* INC_CONFIG_H_ */
