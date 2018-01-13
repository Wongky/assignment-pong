/*
 * ball.cpp
 *
 *  
 *      Author: User
 */

#include"ball.h"

	Ball::Ball(libsc::Lcd* pLcd){
		m_pLcd=pLcd;
		m_width=5;
		m_height=5;
		//setPosition(60,75);
		m_position.x=60;
		m_position.y=75;
		m_v_x = -4;
		m_v_y = 7;
		m_fg_color=0xFD20;
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
    void Ball::move(){

    	m_position.x+=m_v_x;
    	m_position.y+=m_v_y;
    };

    /**
     * Renders the ball.
     */
    void Ball::render() {
    	clean();
    	draw();
    };

    void Ball::rebound(Platform P){
    	Coord coor =P.getPosition();
    	if(m_position.x==4||m_position.x+5==116){m_v_x*=-1;}
    	if(m_position.y==coor.y+4){
    		m_v_y*=-1;
    		switch(m_position.x+2-coor.x){
    			case 0:case 1:case 28:case 29:
    				m_v_x =7;break;
    			case 2: case 3:case 26:case 27:
    				m_v_x =6;break;
    			case 4:case 5:case 24:case 25:
    				m_v_x =5;break;
    			case 6 :case 7:case 22:case 23:
    				m_v_x =4;break;
    			case 8:case 9:case 20:case 21:
    				m_v_x =3;break;
    			case 10:case 11:case 18: case19:
					m_v_x =2;break;
    			case 12:case 13:case 16:case 17:
    				m_v_x =1;break;
    			case 14:case 15:
    				m_v_x =0;break;

    		}
    	}
    }
