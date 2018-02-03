/*
 * game_config.h
 *
 *  Created on: 2018年1月13日
 *      Author: User
 */

#ifndef INC_GAME_CONFIG_H_
#define INC_GAME_CONFIG_H_

//#include "ball.h"
#include "sprite.h"
//#include <libbase/k60/mcg.h>
//using namespace sprite;
//class Ball;
//class Sprite;
class Judge1:public Sprite {
 public:
	explicit Judge1(libsc::Lcd* pLcd):
	Sprite([this](libsc::Lcd* plcd){
		Sprite::Config config;
		config.pLcd=plcd;
		config.width=6;
		config.height=4;
		config.x=116;
		config.y=0;
		config.fg_color=  0x07E0;
		return config;
	 }(pLcd)) {};


    //Ball *ball = nullptr;

	bool mark=false;
	void addmark(){
		mark=true;
		draw();
	}
	void render(){
		if(mark==true){
			draw();
		}
	}
};

class Judge2:public  Sprite {
public:
	explicit Judge2(libsc::Lcd* pLcd):Sprite([this](libsc::Lcd* plcd){
			Sprite::Config config;
			config.pLcd=plcd;
			config.width=6;
			config.height=4;
			config.fg_color=  0x07E0;
			config.x=106;
			config.y=0;
			return config;
		 }(pLcd)) {};

    //Ball *ball = nullptr;
	bool mark=false;
	void addmark(){
		mark=true;
		draw();
		}
	void render(){
			if(mark==true){
				draw();
			}
		}
};


class Judge3: public Sprite {
public:
	explicit Judge3(libsc::Lcd* pLcd):Sprite([this](libsc::Lcd* plcd){
			Sprite::Config config;
			config.pLcd=plcd;
			config.width=6;
			config.height=4;
			config.fg_color=  0x07E0;
			config.x=96;
			config.y=0;
			return config;
		 }(pLcd)) {};

    //Ball *ball = nullptr;
	bool mark=false;
	void addmark(){
		mark=true;
		draw();
		}
	void render(){
			if(mark==true){
				draw();
			}
		}
};


class Judge4:public Sprite {
 public:
	explicit Judge4(libsc::Lcd* pLcd):
	Sprite([this](libsc::Lcd* plcd){
		Sprite::Config config;
		config.pLcd=plcd;
		config.width=6;
		config.height=4;
		config.x=116;
		config.y=146;
		config.fg_color=  0x07E0;
		return config;
	 }(pLcd)) {};


    //Ball *ball = nullptr;

	bool mark=false;
	void addmark(){
		mark=true;
		draw();
	}
	void render(){
		if(mark==true){
			draw();
		}
	}
};

class Judge5:public  Sprite {
public:
	explicit Judge5(libsc::Lcd* pLcd):Sprite([this](libsc::Lcd* plcd){
			Sprite::Config config;
			config.pLcd=plcd;
			config.width=6;
			config.height=4;
			config.fg_color=  0x07E0;
			config.x=106;
			config.y=146;
			return config;
		 }(pLcd)) {};

    //Ball *ball = nullptr;
	bool mark=false;
	void addmark(){
		mark=true;
		draw();
		}
	void render(){
			if(mark==true){
				draw();
			}
		}
};


class Judge6: public Sprite {
public:
	explicit Judge6(libsc::Lcd* pLcd):Sprite([this](libsc::Lcd* plcd){
			Sprite::Config config;
			config.pLcd=plcd;
			config.width=6;
			config.height=4;
			config.fg_color=  0x07E0;
			config.x=96;
			config.y=146;
			return config;
		 }(pLcd)) {};

    //Ball *ball = nullptr;
	bool mark=false;
	void addmark(){
		mark=true;
		draw();
		}
	void render(){
			if(mark==true){
				draw();
			}
		}
};


class FinalJudge: public Sprite {
public:
	explicit FinalJudge(libsc::Lcd* pLcd):Sprite([this](libsc::Lcd* plcd){
			Sprite::Config config;
			config.pLcd=plcd;
			config.width=6;
			config.height=4;
			config.fg_color=  0x07E0;
			config.x=96;
			config.y=0;
			return config;
		 }(pLcd)) {};
	void setjudge(Judge1 * j1,Judge2 * j2,Judge3 * j3){
		mj1=j1;
		mj2=j2;
		mj3=j3;
	}
    Judge1*mj1 =nullptr;
    Judge2*mj2 =nullptr;
    Judge3*mj3 =nullptr;
	//Ball*ball = nullptr;
    //Ball* b = nullptr;
	bool win=false;
	void countmark(){
		if(mj1->mark==false&&mj2->mark==false&&mj3->mark==false){mj1->addmark();}
		else if(mj1->mark==true&&mj2->mark==false&&mj3->mark==false){mj2->addmark();}
		else if(mj1->mark==true&&mj2->mark==true&&mj3->mark==false){mj3->addmark();}
		else if(mj1->mark==true&&mj2->mark==true&&mj3->mark==true){
			win=true;
			//LcdTypewriter writer(Config::GetWriterConfig(&m_pLcd));
	    }
	};
};
class FinalJudgesl: public Sprite {
public:
	explicit FinalJudgesl(libsc::Lcd* pLcd):Sprite([this](libsc::Lcd* plcd){
			Sprite::Config config;
			config.pLcd=plcd;
			config.width=6;
			config.height=4;
			config.fg_color=  0x07E0;
			config.x=96;
			config.y=0;
			return config;
		 }(pLcd)) {};
	void setjudge(Judge4 * j1,Judge5 * j2,Judge6 * j3){
		mj1=j1;
		mj2=j2;
		mj3=j3;
	}
    Judge4*mj1 =nullptr;
    Judge5*mj2 =nullptr;
    Judge6*mj3 =nullptr;
	//Ball*ball = nullptr;
    //Ball* b = nullptr;
	bool win=false;
	void countmark(){
		if(mj1->mark==false&&mj2->mark==false&&mj3->mark==false){mj1->addmark();}
		else if(mj1->mark==true&&mj2->mark==false&&mj3->mark==false){mj2->addmark();}
		else if(mj1->mark==true&&mj2->mark==true&&mj3->mark==false){mj3->addmark();}
		else if(mj1->mark==true&&mj2->mark==true&&mj3->mark==true){
			win=true;
			//LcdTypewriter writer(Config::GetWriterConfig(&m_pLcd));
	    }
	};
};




#endif /* INC_GAME_CONFIG_H_ */
