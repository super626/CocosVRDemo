//
//  HeadMountedDisplay.h
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-26.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

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
