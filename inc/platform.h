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
    explicit Platform(libsc::Lcd* pLcd);

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
