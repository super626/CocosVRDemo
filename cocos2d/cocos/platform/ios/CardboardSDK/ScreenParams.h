//
//  ScreenParams.h
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-26.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#ifndef __CardboardVR_iOS__ScreenParams__
#define __CardboardVR_iOS__ScreenParams__

class ScreenParams
{
public:
    ScreenParams();
    ScreenParams(ScreenParams *screenParams);
    void setWidth(int width);
    int getWidth();
    void setHeight(int height);
    int getHeight();
    float getWidthMeters();
    float getHeightMeters();
    void setBorderSizeMeters(float screenBorderSize);
    float getBorderSizeMeters();
    bool equals(ScreenParams *other);
private:
    int width;
    int height;
    float xMetersPerPixel;
    float yMetersPerPixel;
    float borderSizeMeters;
};


#endif
