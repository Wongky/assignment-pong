/*
 * main.cpp
 *
 * Author: 
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstring>
#include <string>
#include <libbase/k60/mcg.h>
#include <libsc/system.h>

#include <libsc/led.h>
#include <libsc/k60/jy_mcu_bt_106.h>
#include "libsc/led.h"
#include "libsc/joystick.h"
#include "libsc/st7735r.h"
#include "libsc/battery_meter.h"
#include "libsc/k60/jy_mcu_bt_106.h"
#include "libbase/k60/pit.h"
#include "libsc/lcd_typewriter.h"

//#include "debug.h"
//#include "pong.h"
#include "../inc/config.h"
#include "../inc/bluetooth.h"
#include "../inc/ball.h"
#include "../inc/platform.h"
#include "../inc/game_config.h"


namespace libbase
{
    namespace k60
    {

        Mcg::Config Mcg::GetMcgConfig()
        {
            Mcg::Config config;
            config.external_oscillator_khz = 50000;
            config.core_clock_khz = 150000;
            return config;
        }

    }
}

using libsc::System;
using namespace libsc;
using namespace libbase::k60;
//using std::string;


Led *pLed = nullptr;

bool Listener(const unsigned char*, const unsigned int){
	if(pLed)pLed->Switch();
	return 1;
}

char *str = "";

int main() {

	System::Init();

    Led led0(Config::GetLedConfig(0));
    Led led1(Config::GetLedConfig(1));
    Led led2(Config::GetLedConfig(2));
    Led led3(Config::GetLedConfig(3));
    pLed = &led0;

    St7735r lcd(Config::GetLcdConfig());

    LcdTypewriter writer(Config::GetWriterConfig(&lcd));
    LcdConsole console(Config::GetConsoleConfig(&lcd));
    lcd.SetRegion(Lcd::Rect(0,0,128,160));
    writer.WriteString("YOU WIN!");
/*
    Debug::message = "YO";
    Debug::pLcd = &lcd;
//    Debug::pWriter = &writer;
    Debug::pConsole = &console;
*/
    led0.SetEnable(1);
    led1.SetEnable(1);
    led2.SetEnable(1);
    led3.SetEnable(1);

    Bluetooth bt;


    Ball ball(&lcd);
    Platform plat(&lcd);
    Platformsl platsl(&lcd);
    ball.setplat(&plat,&platsl);


    Judge1 j1(&lcd);
    Judge2 j2(&lcd);
    Judge3 j3(&lcd);
    FinalJudge hj(&lcd);
    hj.setjudge(&j1,&j2,&j3);

    Judge4 j1sl(&lcd);
    Judge5 j2sl(&lcd);
    Judge6 j3sl(&lcd);
    FinalJudgesl hjsl(&lcd);
    hjsl.setjudge(&j1sl,&j2sl,&j3sl);

    ball.setjudge(&hj,&hjsl);
    int error;
    ////////master
    auto joyFunctionLeft = [&plat,&bt](const uint8_t id, const Joystick::State which){

    		plat.moveLeft();
    		bt.SendPackage({0,Bluetooth::PkgType::kMasterPlatform,{plat.getPosition().x}});

    	};
	auto joyFunctionRight= [&plat,&bt] (const uint8_t id, const Joystick::State which){
		 	plat.moveRight();
		 	bt.SendPackage({0,Bluetooth::PkgType::kMasterPlatform,{plat.getPosition().x}});
	};
	///////
	Joystick::Config JoystickConfig = Config::GetJoystickConfig();
	JoystickConfig.handlers[2] = joyFunctionLeft;
	JoystickConfig.handlers[3] = joyFunctionRight;
	Joystick joy(JoystickConfig);

	Comm::Package pkg;

    bt.SetHandler([&led0,&led1,&led2,&led3,&bt,&pkg,&ball,&plat,&platsl,&lcd,&writer,&error](Bluetooth::Package package){
    	pkg = package;
    	switch((int)package.type){
    	case Bluetooth::PkgType::kStart://for slave
    		led0.Switch();
    		bt.SendPackage({0,Bluetooth::PkgType::kStartACK,{}});
			break;
    	case Bluetooth::PkgType::kStartACK://for master
    		led1.Switch();
    		bt.SendPackage({0,Bluetooth::PkgType::kMasterPlatform,{plat.getPosition().x}});
    		break;
    	case Bluetooth::PkgType::kMasterPlatform://slave
    		led0.Switch();
    		plat.setPosition(pkg.data[0],8);
    		bt.SendPackage({0,Bluetooth::PkgType::kMasterPlatformACK,{}});
    		break;
    	case Bluetooth::PkgType::kMasterPlatformACK://for master
    		led1.Switch();

    		break;
    	case Bluetooth::PkgType::kSlavePlatform://for master
    		led1.Switch();
    		platsl.setPosition(pkg.data[0],142);
    		bt.SendPackage({0,Bluetooth::PkgType::kSlavePlatformACK,{}});
       		break;
    	case Bluetooth::PkgType::kSlavePlatformACK://for slave
    		led0.Switch();

    	 	break;
    	case Bluetooth::PkgType::kReflection://for slave
    		led0.Switch();
    		ball.rebound();
    		if(ball.getvelocity().x==pkg.data[0]&&ball.getvelocity().y==pkg.data[1]){
    			ball.setVelocity(pkg.data[0], pkg.data[1]);
    		}else{
    			error++;
    			ball.setVelocity(pkg.data[0], pkg.data[1]);
    		}

    	    break;
    	case Bluetooth::PkgType::kReflectionACK://for master
    		led1.Switch();
    	    break;
    	case Bluetooth::PkgType::kLocation://for slave
    		led0.Switch();
    		ball.move();
    		if(ball.getPosition().x==pkg.data[0]&&ball.getPosition().y==pkg.data[1]){
    			ball.setPosition(pkg.data[0],pkg.data[1]);
    		}else{
    			error++;
    			ball.setPosition(pkg.data[0],pkg.data[1]);
    		}
    	 	break;
    	case Bluetooth::PkgType::kLocationACK://for master
    		led1.Switch();
       	    break;
    	case Bluetooth::PkgType::kResult://for slave
    		led0.Switch();
    		if(pkg.data[0]==0){
    			lcd.SetRegion(Lcd::Rect(0,75,120,15));
    			writer.WriteString("YOU LOSE!");
    		}else if (pkg.data[0]==1){
    			lcd.SetRegion(Lcd::Rect(0,75,120,15));
    			writer.WriteString("YOU WIN!");

    		}
    	    break;
    	case Bluetooth::PkgType::kResultACK://for master
    		led1.Switch();
    	   	 break;
    	default :break;
    	}
    });

///master
    bt.SendPackage({0,Bluetooth::PkgType::kStart,{}});
    lcd.SetRegion(Lcd::Rect(0,0,120,150));
    lcd.FillColor(lcd.kWhite);

    //if(bt.IsWaitingACK()==false){

    while(1){
    	if(System::Time()%50==0){

    		lcd.SetRegion(Lcd::Rect(0,8,120,150));
    		lcd.FillColor(lcd.kWhite);
    		if(ball.rebound()==true){
        		bt.SendPackage({0,Bluetooth::PkgType::kReflection,{ball.getvelocity().x,ball.getvelocity().y}});

    		}
    		if(ball.needreset==true){
    			ball.reset();
    			ball.needreset=false;
    			bt.SendPackage({0,Bluetooth::PkgType::kLocation,{ball.getPosition().x,ball.getPosition().y}});
    			//j1.addmark();
    			//j2.addmark();
    		}
    		lcd.SetRegion(Lcd::Rect(0,0,120,8));
    		j1.render();
    		j2.render();
    		j3.render();
		j1sl.render();
    		j2sl.render();
    		j3sl.render();
    		if(hj.win){
    			lcd.SetRegion(Lcd::Rect(60,60,128,15));
    			bt.SendPackage({0,Bluetooth::PkgType::kResult,{0}});
    			    writer.WriteString("YOU WIN!");
    			    break;

    		}
    		if(hjsl.win){
    		    			lcd.SetRegion(Lcd::Rect(60,60,128,15));
    		    			bt.SendPackage({0,Bluetooth::PkgType::kResult,{1}});
    		    			    writer.WriteString("YOU LOSE!");
    		    			    break;

    		    		}
    		bt.SendPackage({0,Bluetooth::PkgType::kLocation,{ball.getPosition().x,ball.getPosition().y}});
    		ball.render();

    		//lcd.SetRegion(Lcd::Rect(0,0,120,150));
    		plat.render();
    		//lcd.SetRegion(Lcd::Rect(0,0,120,150));
    		if((platsl.getPosition().x+15)!=ball.getPosition().x){
    		if((platsl.getPosition().x+15)>ball.getPosition().x){
    			platsl.moveLeft();
    		}else
    		if((platsl.getPosition().x+15)<ball.getPosition().x){
    			platsl.moveRight();
    		}}
    		platsl.render();

//    		bt.SendPackage({0,Bluetooth::PkgType::kMasterPlatform,{plat.getPosition().x}});}

//			char c[10];
//			lcd.SetRegion(Lcd::Rect(0,0,100,15));
//			if(bt.IsTimerEnable()){
//				writer.WriteString("timer enabled");
//			}else{
//				writer.WriteString("timer disabled");
//			}
//			lcd.SetRegion(Lcd::Rect(0,15,100,15));
//			if(bt.IsWaitingACK()){
//				writer.WriteString("waiting");
//			}else{
//				writer.WriteString("not waiting");
//			}
//			lcd.SetRegion(Lcd::Rect(0,30,100,15));
//			sprintf(c,"size:%d",bt.queue.size());
//			writer.WriteBuffer(c,10);
//			lcd.SetRegion(Lcd::Rect(0,45,100,15));
//			sprintf(c,"last:%d",bt.send_time);
//			writer.WriteBuffer(c,10);
    	}

    }
  // }

    return 0;
}
