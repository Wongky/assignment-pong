*
 * sprite.cpp
 *
 *  Created on: 2018¦~1¤ë6¤é
 *      Author: User
 */


#include"sprite.h"

    /**
     * Constructor of sprite which accepts a sprite config.
     * @param config Config object to initialize default values.
     */
    Sprite::Sprite(Config config){
    	m_width=config.width;
		m_height=config.height;
		m_fg_color=config.fg_color;
		m_bg_color=config.bg_color;
		m_position.x=config.x;
		m_position.y=config.y;
		m_pLcd=config.pLcd;
    }

    /**
     * Renders the sprite
     */
    virtual void Sprite::render(){
    	draw();
    };

    /**
     * Sets position of sprite using 2 integers x and y.
     * @param x Position x.
     * @param y Position y.
     */
    void  Sprite::setPosition(int x, int y){

    	m_position.x=x;
    	m_position.y=y;
    };

    /**
     * Sets position of sprite using a Coord.
     * @param coord Coordinate containing position x and y.
     */
    void  Sprite::setPosition(Coord coord){
    	m_position = coord;
    };

    /**
     * Gets position of the sprite.
     * @return position of sprite with Coord.
     */
    Coord  Sprite::getPosition(){
    	return m_position;
    };


    /**
        * Draws the sprite in the new frame.
        */
     virtual void  Sprite::draw(){
    	 clean();
    	 libsc::Lcd::Rect nrect{m_position.x,m_position.y,m_width,m_height};
    	 m_pLcd->SetRegion(nrect);
    	 m_pLcd->FillColor(0x001F);

     }

       /**
        * Removes the sprite with the previous location in the new frame.
        */
     virtual void Sprite::clean(){
    	 m_pLcd->ClearRegion();
     };
