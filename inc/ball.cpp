/*
 * ball.cpp
 *
 *  Created on: 2018¦~1¤ë6¤é
 *      Author: User
 */

#include"ball.h"

	Ball::Ball(libsc::Lcd* pLcd){
		m_pLcd=pLcd;
		m_width=30;
		m_height=4;
		m_position.x=60;
		m_position.y=75;
		m_v_x = -4;
		m_v_y = 7;
	};

    /**
     * Sets velocity of the ball.
     * @param v_x Velocity in x direction.
     * @param v_y Velocity in y direction.
     */
    void Ball::setVelocity(int v_x, int v_y){
    	m_v_x = v_x;
    	m_v_y = v_y;
    };

    /**
     * Calculate movements for the next frame.
     */
    void Ball::move(){};

    /**
     * Renders the ball.
     */
    void Ball::render() {
    	clean();
    	draw();
    };

    void Ball::rebound(){
    	P
    };

