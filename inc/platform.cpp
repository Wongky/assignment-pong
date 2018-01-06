/*
 * platform.cpp
 *
 *  Created on: 2018¦~1¤ë6¤é
 *      Author: User
 */

#include "platform.h"

	Platform::Platform(libsc::Lcd* pLcd){


	m_pLcd=pLcd;
	m_width=5;
	m_height=10;
	m_position.x=60;
	m_position.y=3;



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
