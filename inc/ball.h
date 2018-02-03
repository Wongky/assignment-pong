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
    void setVelocity(int v_x, int v_y){
    	m_v_x = v_x;
    	m_v_y = v_y;
    };


    /**
     * Calculate movements for the next frame.
     */
    void move(){
    	m_position.x+=m_v_x;
    	m_position.y+=m_v_y;};

    /**
     * Renders the ball.
     */
    void render() override{
    	clean();
    	move();
    	draw();
    	};
    void setjudge(FinalJudge * j,FinalJudge * jsl){
    	j1=j;
    	j2sl=jsl;
    }
    void setplat(Platform * p,Platformsl * psl){
    	plat = p;
    	platsl = psl;
    }
   void reset(){
	   m_position.x=60;
	   m_position.y=75;
	   m_v_y = 7;
	   m_v_x = 0;
	  // render();
   }
   bool rebound(){
    	Coord coor =plat->getPosition();
    	Coord coorsl =platsl->getPosition();
    	int tempx;
    	int tempy;
    	if(m_position.x<=4||m_position.x+5>=116){m_v_x*=-1;}
    	if(m_position.y==coor.y+4){
    		tempx = m_position.x;
    		tempy = m_position.y;

    		if((m_position.x+2-coor.x)>=0&&(m_position.x+2-coor.x)<=29){
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
    			case 10:case 11:case 18: case 19:
					m_v_x =2;break;
    			case 12:case 13:case 16:case 17:
    				m_v_x =1;break;
    			case 14:case 15:
    				m_v_x =0;break;
    			default:break;

    		}
    		m_position.x = tempx+4/8*m_v_x;
    		m_position.y = tempy+4/8*m_v_y;
    		return true;
    		}else if(!(m_position.x+2-coor.x)>=0&&!(m_position.x+2-coor.x)<=29){
    			j2sl->countmark();
    			needreset=true;
    			return false;
    		}
    	}
    	if(m_position.y==coorsl.y-4){
    		tempx = m_position.x;
    		tempy = m_position.y;

			if((m_position.x+2-coorsl.x)>=0&&(m_position.x+2-coorsl.x)<=29){
				m_v_y*=-1;
			switch(m_position.x+2-coorsl.x){
				case 0:case 1:case 28:case 29:
					m_v_x =-7;break;
				case 2: case 3:case 26:case 27:
					m_v_x =-6;break;
				case 4:case 5:case 24:case 25:
					m_v_x =-5;break;
				case 6 :case 7:case 22:case 23:
					m_v_x =-4;break;
				case 8:case 9:case 20:case 21:
					m_v_x =-3;break;
				case 10:case 11:case 18: case 19:
					m_v_x =-2;break;
				case 12:case 13:case 16:case 17:
					m_v_x =-1;break;
				case 14:case 15:
					m_v_x =0;break;
				default:break;

    	    		}
			m_position.x = tempx+(4/8)*m_v_x;
				m_position.y = tempy+(4/8)*m_v_y;
			return true;}
    	else if(!(m_position.x+2-coorsl.x)>=0&&!(m_position.x+2-coorsl.x)<=29){
				j1->countmark();
    			needreset=true;
				return false;

		}
    	}

    }
   bool needreset=false;
   Coord getvelocity(){
	  Coord vel(m_v_x,m_v_y);
	  return vel;
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
    Platformsl * platsl = nullptr;
    FinalJudge *j1=nullptr;
    FinalJudge *j2sl =nullptr;

    /**
     * Executes rebound logic.
     */


    };

#endif //INC_BALL_H
