/* platform.cpp
 *
 *  Created on: 2018年1月6日
 *      Author: User
 */

#include "platform.h"

	Platform::Platform(libsc::Lcd* pLcd){


	m_pLcd=pLcd;
	m_width=30;
	m_height=4;
	m_position.x=60;
	m_position.y=8;



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
