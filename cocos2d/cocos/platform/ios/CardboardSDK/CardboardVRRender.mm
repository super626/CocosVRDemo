//
//  CardboardViewController.mm
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-09-04.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#import "CardboardVRRender.h"

#include "CardboardDeviceParams.h"
#include "DistortionRenderer.h"
#include "EyeParams.h"
#include "HeadTracker.h"
#include "HeadTransform.h"
#include "HeadMountedDisplay.h"
#include "MagnetSensor.h"
#include "Viewport.h"

#include "base/CCDirector.h"
#include "renderer/CCFrameBuffer.h"
#include "renderer/ccGLStateCache.h"

#import "DebugUtils.h"
#import "GLHelpers.h"

#import <OpenGLES/ES2/glext.h>

class VRRenderer
{
public:
    VRRenderer(){}
    ~VRRenderer(){}
    
    void drawFrameWithHeadTransform(HeadTransform* headTransform, EyeParams*leftEyeParams, EyeParams* rightEyeParams)
    {
        
        checkGLError();
        
        auto director = cocos2d::Director::getInstance();
    
        // NSLog(@"%@", NSStringFromGLKMatrix4(leftEyeParams->getTransform()->getEyeView()));
    
//        [self.stereoRendererDelegate prepareNewFrameWithHeadTransform:headTransform];
    
        checkGLError();
    
        glEnable(GL_SCISSOR_TEST);
        leftEyeParams->getViewport()->setGLViewport();
        leftEyeParams->getViewport()->setGLScissor();
    
        checkGLError();
    
//        [self.stereoRendererDelegate drawEyeWithTransform:leftEyeParams->getTransform()
//                                                  eyeType:leftEyeParams->getEye()];
        
    
        director->drawVRScene(leftEyeParams->getTransform()->getEyeView().m);
        checkGLError();
    
        if (rightEyeParams == nullptr) { return; }
    
        rightEyeParams->getViewport()->setGLViewport();
        rightEyeParams->getViewport()->setGLScissor();
    
        checkGLError();
    
//        [self.stereoRendererDelegate drawEyeWithTransform:rightEyeParams->getTransform()
//                                                  eyeType:rightEyeParams->getEye()];
        director->drawVRScene(rightEyeParams->getTransform()->getEyeView().m);
    
        checkGLError();
    }
    void finishFrame(Viewport *viewport)
    {
        viewport->setGLViewport();
        viewport->setGLScissor();
    }

    void bindDefaultFBO()
    {
        //TODO
        cocos2d::experimental::FrameBuffer::applyDefaultFBO();
        cocos2d::GL::enableVertexAttribs(0);
    }
};

CardboardVRRender::CardboardVRRender()
{
    magnetSensor = new MagnetSensor();
    headTracker = new HeadTracker();
    headTransform = new HeadTransform();
    headMountedDisplay = new HeadMountedDisplay([UIScreen mainScreen]);
    
    monocularParams = new EyeParams(EyeParamsTypeMonocular);
    leftEyeParams = new EyeParams(EyeParamsTypeLeft);
    rightEyeParams = new EyeParams(EyeParamsTypeRight);
    
    distortionRenderer = new DistortionRenderer();
    
    renderer = new VRRenderer();
//    stereoRenderer = [StereoRenderer new];
    distortionCorrectionScale = 1.0f;
    
    isVRModeEnabled = true;
    distortionCorrectionEnabled = true;
    
    zNear = 0.1f;
    zFar = 100.0f;
    
    projectionChanged = true;
    
    startHeadTracking();
}

CardboardVRRender::~CardboardVRRender()
{
    delete magnetSensor;
    delete headTracker;
    delete headTransform;
    delete headMountedDisplay;
    
    delete monocularParams;
    delete leftEyeParams;
    delete rightEyeParams;
    
    delete distortionRenderer;
    delete renderer;
}

void CardboardVRRender::startHeadTracking()
{
    headTracker->startTracking();
    magnetSensor->start();
}

void CardboardVRRender::stopHeadTracking()
{
    headTracker->stopTracking();
    magnetSensor->stop();
}

void CardboardVRRender::setVRModeEnable(bool isEnable)
{
    if (isVRModeEnabled != isEnable)
    {
        isVRModeEnabled = isEnable;
        isVRModeEnabled ? startHeadTracking() : stopHeadTracking();
    }
}

const float* CardboardVRRender::getHeadEuler()
{
    GLKVector3 dir = headTransform->getEulerAngles();
    headEuler[0] = dir.v[0];
    headEuler[1] = dir.v[1];
    headEuler[2] = dir.v[2];
    return headEuler;
}

void CardboardVRRender::render()
{
    
    CardboardDeviceParams *cardboardDeviceParams = headMountedDisplay->getCardboard();
    
    headTransform->setHeadView(headTracker->getLastHeadView());
    float halfInterpupillaryDistance = cardboardDeviceParams->getInterpupillaryDistance() * 0.5f;
    
    // NSLog(@"%@", NSStringFromGLKMatrix4(_headTracker->getLastHeadView()));
    
    if (isVRModeEnabled)
    {
        GLKMatrix4 leftEyeTranslate = GLKMatrix4Identity;
        GLKMatrix4 rightEyeTranslate = GLKMatrix4Identity;
        
        leftEyeTranslate = GLKMatrix4Translate(leftEyeTranslate, halfInterpupillaryDistance, 0, 0);
        rightEyeTranslate = GLKMatrix4Translate(rightEyeTranslate, -halfInterpupillaryDistance, 0, 0);
        
        // NSLog(@"%@", NSStringFromGLKMatrix4(_headTransform->getHeadView()));
        
        leftEyeParams->getTransform()->setEyeView( GLKMatrix4Multiply(leftEyeTranslate, headTransform->getHeadView()));
        rightEyeParams->getTransform()->setEyeView( GLKMatrix4Multiply(rightEyeTranslate, headTransform->getHeadView()));
        
    }
    else
    {
        monocularParams->getTransform()->setEyeView(headTransform->getHeadView());
    }
    
    if (projectionChanged)
    {
        ScreenParams *screenParams = headMountedDisplay->getScreen();
        monocularParams->getViewport()->setViewport(0, 0, screenParams->getWidth(), screenParams->getHeight());
        
        
        if (!isVRModeEnabled)
        {
            float aspectRatio = screenParams->getWidth() / screenParams->getHeight();
            monocularParams->getTransform()->setPerspective(
                                                             GLKMatrix4MakePerspective(GLKMathDegreesToRadians(headMountedDisplay->getCardboard()->getFovY()),
                                                                                       aspectRatio,
                                                                                       zNear,
                                                                                       zFar));
        }
        else if (distortionCorrectionEnabled)
        {
            
            updateFovs(leftEyeParams->getFov(), rightEyeParams->getFov());
            distortionRenderer->onProjectionChanged(headMountedDisplay, leftEyeParams, rightEyeParams, zNear, zFar);
        }
        else
        {
            float eyeToScreenDistance = cardboardDeviceParams->getVisibleViewportSize() / 2.0f / tanf(GLKMathDegreesToRadians(cardboardDeviceParams->getFovY()) / 2.0f );
            
            float left = screenParams->getWidthMeters() / 2.0f - halfInterpupillaryDistance;
            float right = halfInterpupillaryDistance;
            float bottom = cardboardDeviceParams->getVerticalDistanceToLensCenter() - screenParams->getBorderSizeMeters();
            float top = screenParams->getBorderSizeMeters() + screenParams->getHeightMeters() - cardboardDeviceParams->getVerticalDistanceToLensCenter();
            
            FieldOfView *leftEyeFov = leftEyeParams->getFov();
            leftEyeFov->setLeft(GLKMathRadiansToDegrees(atan2f(left, eyeToScreenDistance)));
            leftEyeFov->setRight(GLKMathRadiansToDegrees(atan2f(right, eyeToScreenDistance)));
            leftEyeFov->setBottom(GLKMathRadiansToDegrees(atan2f(bottom, eyeToScreenDistance)));
            leftEyeFov->setTop(GLKMathRadiansToDegrees(atan2f(top, eyeToScreenDistance)));
            
            FieldOfView *rightEyeFov = rightEyeParams->getFov();
            rightEyeFov->setLeft(leftEyeFov->getRight());
            rightEyeFov->setRight(leftEyeFov->getLeft());
            rightEyeFov->setBottom(leftEyeFov->getBottom());
            rightEyeFov->setTop(leftEyeFov->getTop());
            
            leftEyeParams->getTransform()->setPerspective( leftEyeFov->toPerspectiveMatrix(zNear, zFar));
            rightEyeParams->getTransform()->setPerspective( rightEyeFov->toPerspectiveMatrix(zNear, zFar));
            
            leftEyeParams->getViewport()->setViewport(0, 0, screenParams->getWidth() / 2, screenParams->getHeight());
            rightEyeParams->getViewport()->setViewport(screenParams->getWidth() / 2, 0, screenParams->getWidth() / 2, screenParams->getHeight());
        }
        
        projectionChanged = false;
    }
    
    if (isVRModeEnabled)
    {
        if (distortionCorrectionEnabled)
        {
            
            distortionRenderer->beforeDrawFrame();
            
            if (distortionCorrectionScale == 1.0f)
            {
                renderer->drawFrameWithHeadTransform(headTransform, leftEyeParams, rightEyeParams);
            }
            else
            {
                int leftX = leftEyeParams->getViewport()->x;
                int leftY = leftEyeParams->getViewport()->y;
                int leftWidth = leftEyeParams->getViewport()->width;
                int leftHeight = leftEyeParams->getViewport()->height;
                int rightX = rightEyeParams->getViewport()->x;
                int rightY = rightEyeParams->getViewport()->y;
                int rightWidth = rightEyeParams->getViewport()->width;
                int rightHeight = rightEyeParams->getViewport()->height;
                
                leftEyeParams->getViewport()->setViewport((int)(leftX * distortionCorrectionScale),
                                                           (int)(leftY * distortionCorrectionScale),
                                                           (int)(leftWidth * distortionCorrectionScale),
                                                           (int)(leftHeight * distortionCorrectionScale));
                
                rightEyeParams->getViewport()->setViewport((int)(rightX * distortionCorrectionScale),
                                                            (int)(rightY * distortionCorrectionScale),
                                                            (int)(rightWidth * distortionCorrectionScale),
                                                            (int)(rightHeight * distortionCorrectionScale));
                

                renderer->drawFrameWithHeadTransform(headTransform, leftEyeParams, rightEyeParams);
                
                leftEyeParams->getViewport()->setViewport(leftX, leftY, leftWidth, leftHeight);
                rightEyeParams->getViewport()->setViewport(rightX, rightY, rightWidth, rightHeight);
            }
            
            checkGLError();
            
            // Rebind original framebuffer
//            [self.view bindDrawable]; //TODO: bind camera default framebuffer
            renderer->bindDefaultFBO();
            distortionRenderer->afterDrawFrame();
            
            checkGLError();
        }
        else
        {
            renderer->drawFrameWithHeadTransform(headTransform, leftEyeParams, rightEyeParams);
        }
    }
    else
    {
        renderer->drawFrameWithHeadTransform(headTransform, monocularParams, nullptr);
    }
    
    renderer->finishFrame(monocularParams->getViewport());
    
    glDisable(GL_SCISSOR_TEST);
    checkGLError();
}

void CardboardVRRender::updateFovs(FieldOfView* leftEyeFov, FieldOfView* rightEyeFov)
{
    CardboardDeviceParams *cardboardDeviceParams = headMountedDisplay->getCardboard();
    ScreenParams *screenParams = headMountedDisplay->getScreen();
    Distortion *distortion = cardboardDeviceParams->getDistortion();
    
    float idealFovAngle = GLKMathRadiansToDegrees(atan2f(cardboardDeviceParams->getLensDiameter() / 2.0f,
                                                         cardboardDeviceParams->getEyeToLensDistance()));
    float eyeToScreenDistance = cardboardDeviceParams->getEyeToLensDistance() + cardboardDeviceParams->getScreenToLensDistance();
    float outerDistance = ( screenParams->getWidthMeters() - cardboardDeviceParams->getInterpupillaryDistance() ) / 2.0f;
    float innerDistance = cardboardDeviceParams->getInterpupillaryDistance() / 2.0f;
    float bottomDistance = cardboardDeviceParams->getVerticalDistanceToLensCenter() - screenParams->getBorderSizeMeters();
    float topDistance = screenParams->getHeightMeters() + screenParams->getBorderSizeMeters() - cardboardDeviceParams->getVerticalDistanceToLensCenter();
    
    float outerAngle = GLKMathRadiansToDegrees(atan2f(distortion->distort(outerDistance), eyeToScreenDistance));
    float innerAngle = GLKMathRadiansToDegrees(atan2f(distortion->distort(innerDistance), eyeToScreenDistance));
    float bottomAngle = GLKMathRadiansToDegrees(atan2f(distortion->distort(bottomDistance), eyeToScreenDistance));
    float topAngle = GLKMathRadiansToDegrees(atan2f(distortion->distort(topDistance), eyeToScreenDistance));
    
    leftEyeFov->setLeft(MIN(outerAngle, idealFovAngle));
    leftEyeFov->setRight(MIN(innerAngle, idealFovAngle));
    leftEyeFov->setBottom(MIN(bottomAngle, idealFovAngle));
    leftEyeFov->setTop(MIN(topAngle, idealFovAngle));
    
    rightEyeFov->setLeft(MIN(innerAngle, idealFovAngle));
    rightEyeFov->setRight(MIN(outerAngle, idealFovAngle));
    rightEyeFov->setBottom(MIN(bottomAngle, idealFovAngle));
    rightEyeFov->setTop(MIN(topAngle, idealFovAngle));
}
