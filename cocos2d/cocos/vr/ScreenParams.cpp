//
//  ScreenParams.mm
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-26.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#include "ScreenParams.h"
#include "base/CCDirector.h"
#include "platform/CCGLView.h"
#include "platform/CCDevice.h"
#include "base/ccMacros.h"

static float convertDistanceFromPixelToInch(float pixels)
{
    return pixels / cocos2d::Device::getDPI();
}

ScreenParams::ScreenParams()
{
    const float metersPerInch = 0.0254f;
    const float defaultBorderSizeMeters = 0.003f;
    this->borderSizeMeters = defaultBorderSizeMeters;
    
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    this->width = glview->getFrameSize().width;
    this->height = glview->getFrameSize().height;
    float inchPerPixel = convertDistanceFromPixelToInch(1.f);
    this->xMetersPerPixel = (metersPerInch * inchPerPixel);
    this->yMetersPerPixel = (metersPerInch * inchPerPixel);
}

ScreenParams::ScreenParams(ScreenParams *screenParams)
{
    this->width = screenParams->width;
    this->height = screenParams->height;
    this->xMetersPerPixel = screenParams->xMetersPerPixel;
    this->yMetersPerPixel = screenParams->yMetersPerPixel;
    this->borderSizeMeters = screenParams->borderSizeMeters;
}

void ScreenParams::setWidth(int width)
{
    this->width = width;
}

int ScreenParams::getWidth()
{
    return this->width;
}

void ScreenParams::setHeight(int height)
{
    this->height = height;
}

int ScreenParams::getHeight()
{
    return this->height;
}

float ScreenParams::getWidthMeters()
{
    float meters = this->width * this->xMetersPerPixel;
    return meters;
}

float ScreenParams::getHeightMeters()
{
    float meters = this->height * this->yMetersPerPixel;
    return meters;
}

void ScreenParams::setBorderSizeMeters(float screenBorderSize)
{
    this->borderSizeMeters = screenBorderSize;
}

float ScreenParams::getBorderSizeMeters()
{
    return this->borderSizeMeters;
}

bool ScreenParams::equals(ScreenParams *other)
{
    if (other == nullptr) {
        return false;
    }
    if (other == this) {
        return true;
    }
    return (this->getWidth() == other->getWidth()) && (this->getHeight() == other->getHeight()) && (this->getWidthMeters() == other->getWidthMeters()) && (this->getHeightMeters() == other->getHeightMeters()) && (this->getBorderSizeMeters() == other->getBorderSizeMeters());
}
