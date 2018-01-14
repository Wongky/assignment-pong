/* platform.cpp
 *
 *  Created on: 2018
 *      Author: User
 */

#include "platform.h"

	Platform::Platform(libsc::Lcd* pLcd){


	m_pLcd=pLcd;
	m_width=30;
	m_height=4;
	m_position.x=60;
	m_position.y=8;
	m_fg_color=  0x0000;



}
/**
     * Moves the platform one step to the left.
     */
    void Platform::moveLeft(){
    	if(m_position-4>=4){
    	m_position -=4;
    	}
    };

    /**
     * Moves the platform one step to the right.
     */
    void Platform::moveRight(){

    	if(m_position+4>=116){
    	    	m_position +=4;
    	    	}
    };

    /**
     * Renders the platform.
     */
    void Platform::render(){
    	clean();
    	draw();
    };
	Platformsl::Platformsl(libsc::Lcd* pLcd){


	m_pLcd=pLcd;
	m_width=30;
	m_height=4;
	m_position.x=60;
	m_position.y=142;
	m_fg_color=  0x0000;



}
/**
     * Moves the platform one step to the left.
     */
    void Platformsl::moveRight(){
    	if(m_position-4>=4){
    	m_position -=4;
    	}
    };

    /**
     * Moves the platform one step to the right.
     */
    void Platformsl::moveLeft(){

    	if(m_position+4>=116){
    	    	m_position +=4;
    	    	}
    };

    /**
     * Renders the platform.
     */
    void Platformsl::render(){
    	clean();
    	draw();
    };
