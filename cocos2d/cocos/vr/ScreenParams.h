/**
 Copyright (c) 2014 Peter Tribe.
 Copyright (c) 2015-2016 Chukong Technologies
 
 
 Original file from Peter Tribe
 
 This file was modified to fit the cocos2d-x project by super626
 */

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
