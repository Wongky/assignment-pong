//
// Created by Daniel on 24/12/2017.
//

#ifndef INC_PLATFORM_H
#define INC_PLATFORM_H


#include "sprite.h"
#include "game_config.h"

/**
 * Platform Sprite for the Pong game.
 */
class Platform: public Sprite {
public:
    explicit Platform(libsc::Lcd* pLcd):Sprite([this](libsc::Lcd* pLcd){
    	Sprite::Config config;
    	config.pLcd=pLcd;
    	config.width=30;
    	config.height=4;
    	config.x=60;
    	config.y=8;
    	config.fg_color=  0x0000;
    	return config;
    }(pLcd)) {};
   // ~Platform(){};
    /**
     * Moves the platform one step to the left.
     */
    void moveLeft(){
    	if(m_position.x-4>=4){
    	    m_position.x -=4;
    	}
    };

    /**
     * Moves the platform one step to the right.
     */
    void moveRight(){
    	if(m_position.x+4<=116){
    		m_position.x +=4;
    	}
    };

    /**
     * Renders the platform.
     */
    void render() override{
    	clean();
    	draw();
    };

};

class Platformsl :public Sprite{
  public:
	 explicit Platformsl(libsc::Lcd* pLcd):Sprite([this](libsc::Lcd* pLcd){
	    	Sprite::Config config;
	    	config.pLcd=pLcd;
	    	config.width=30;
	    	config.height=4;
	    	config.x=60;
	    	config.y=142;
	    	config.fg_color=  0x0000;
	    	return config;
	    }(pLcd)) {};
	   // ~Platform(){};
	    /**
	     * Moves the platform one step to the left.
	     */
	    void moveLeft(){
	    	if(m_position.x-4>=4){
	    	m_position.x -=4;
	    	}
	    };

	    /**
	     * Moves the platform one step to the right.
	     */
	    void moveRight(){
	    	if(m_position.x+4<=116){
	    	    	    	m_position.x +=4;
	    	    	    	}
	    };

	    /**
	     * Renders the platform.
	     */
	    void render() override{
	    	clean();
	    	 draw();
	    };

};
#endif //INC_PLATFORM_H
