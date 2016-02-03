//
//  Viewport.mm
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-08-26.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#include "Viewport.h"
#include "platform/CCGL.h"

void Viewport::setViewport(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void Viewport::setGLViewport()
{
    glViewport(this->x, this->y, this->width, this->height);
}

void Viewport::setGLScissor()
{
    glScissor(this->x, this->y, this->width, this->height);
}

std::string Viewport::toString()
{
    char str[256];
    sprintf(str, "Viewport {x:%d y:%d width:%d height:%d}", this->x, this->y, this->width, this->height);
    return std::string(str);
}