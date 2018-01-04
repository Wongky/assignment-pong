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

using libsc::Led;
using libsc::Lcd;
using libsc::Joystick;
using libsc::St7735r;
using libsc::LcdTypewriter;
using libsc::LcdConsole;
using libsc::BatteryMeter;
using libsc::k60::JyMcuBt106;
using libbase::k60::Pit;

class Config{
public:

	static Led::Config GetLedConfig(int id){
		Led::Config config;
		config.id = id;
		return config;
	}

    static Joystick::Config GetJoystickConfig() {
        //TODO: finish it
    	Joystick::Config newjok;

    }

    static St7735r::Config GetLcdConfig() {
    	St7735r::Config newlcd;
    	newlcd.fps=60;
    	newlcd.is_bgr=false;
    	newlcd.orientation=0;
        //TODO: finish it
    }

    static JyMcuBt106::Config GetBluetoothConfig(std::function<bool(const Byte *data, const size_t size)> isr) {

    	//TODO: finish it
    	/*JyMcuBt106::Config newbtcon;
    	newbtcon.baud_rate = 4;
    	newbtcon.is_rx_irq_threshold_percentage = false;
    	newbtcon.tx_buf_size=14;
    	newbtcon.tx_dma_channel=static_cast<uint8_t>(-1);
    	newbtcon.rx_irq_threshold=1;
    	newbtcon.rx_isr = isr;
    	return newbtcon;*/
    	JyMcuBt106::Config uartConfig;
    	uartConfig.id = 0;
    	uartConfig.baud_rate = libbase::k60::Uart::Config::BaudRate::k115200;
    	uartConfig.rx_isr = isr;
      	return uartConfig;

    }

    static Pit::Config GetBluetoothPitConfig(std::function<void(Pit*)> isr){
    	//TODO: finish it
    	/*Pit::Config newpit;
    	newpit.channel= 0;
    	newpit.count;
    	newpit.is_enable=true;
    	newpit.isr=isr;
        return newpit;*/
        Pit::Config pitConfig;

        pitConfig.channel = 0;

        pitConfig.count = 75000*100; //job executed once per 250ms

        pitConfig.isr = isr;

        return pitConfig;

    }
};


#endif /* INC_CONFIG_H_ */
