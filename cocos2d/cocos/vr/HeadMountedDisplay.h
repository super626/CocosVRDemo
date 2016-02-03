/**
 Copyright (c) 2014 Peter Tribe.
 Copyright (c) 2015-2016 Chukong Technologies
 
 
 Original file from Peter Tribe
 
 This file was modified to fit the cocos2d-x project by super626
 */

#ifndef __CardboardVR_iOS__HeadMountedDisplay__
#define __CardboardVR_iOS__HeadMountedDisplay__

#include "ScreenParams.h"
#include "CardboardDeviceParams.h"

class HeadMountedDisplay
{
public:
    HeadMountedDisplay();
    HeadMountedDisplay(HeadMountedDisplay *hmd);
    ~HeadMountedDisplay();
    void setScreen(ScreenParams* screen);
    ScreenParams* getScreen();
    void setCardboard(CardboardDeviceParams *cardboard);
    CardboardDeviceParams* getCardboard();
    bool equals(HeadMountedDisplay *other);
private:
    ScreenParams *screen;
    CardboardDeviceParams *cardboard;
};

#endif 
