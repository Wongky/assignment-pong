//
// Created by Daniel on 24/12/2017.
//

#ifndef INC_BALL_H
#define INC_BALL_H


#include "sprite.h"
#include "platform.h"
#include "game_config.h"

/**
 * Ball sprite for the Pong game.
 */
class Ball: public Sprite {
public:
    explicit Ball(libsc::Lcd* pLcd):
	Sprite([this](libsc::Lcd* plcd){
    	Sprite::Config config;
    	config.pLcd=plcd;
    	config.width=5;
    	config.height=5;
		config.x=60;
		config.y=75;
		config.fg_color=0xFD20;
    	return config;
    }(pLcd)) {};
    /**
     * Sets velocity of the ball.
     * @param v_x Velocity in x direction.
     * @param v_y Velocity in y direction.
     */
    void setVelocity(int v_x, int v_y);

    /**
     * Calculate movements for the next frame.
     */
    void move();

    /**
     * Renders the ball.
     */
    void render() override;

    void setplat(Platform * p,Platformsl * psl){
    	plat = p;
    	platsl = psl;
    }

private:

    /**
     * Velocity in X direction of the ball in pixels.
     */
    int m_v_x = 0;

    /**
     * Velocity in Y direction of the ball in pixels.
     */
    int m_v_y = 7;
    Platform * plat = nullptr;
    Platform * platsl = nullptr;

    /**
     * Executes rebound logic.
     */
    void rebound(Platform plat,Platformsl platsl);
};


#endif //INC_BALL_H
