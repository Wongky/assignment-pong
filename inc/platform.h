//
// Created by Daniel on 24/12/2017.
//

#ifndef INC_PLATFORM_H
#define INC_PLATFORM_H


#include "sprite.h"
//#include "game_config.h"

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
    void moveLeft();

    /**
     * Moves the platform one step to the right.
     */
    void moveRight();

    /**
     * Renders the platform.
     */
    void render() override;

};

class Platformsl :public Platform{
  public:
	explicit Platformsl(libsc::Lcd* pLcd);
	   // ~Platform(){};
	    /**
	     * Moves the platform one step to the left.
	     */
	    void moveLeft();

	    /**
	     * Moves the platform one step to the right.
	     */
	    void moveRight();

	    /**
	     * Renders the platform.
	     */
	    void render() override;

};
#endif //INC_PLATFORM_H
