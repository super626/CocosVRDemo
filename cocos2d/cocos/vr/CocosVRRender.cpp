//
//  CardboardViewController.mm
//  CardboardVR-iOS
//
//  Created by Peter Tribe on 2014-09-04.
//  Copyright (c) 2014 Peter Tribe. All rights reserved.
//

#include "CocosVRRender.h"

#include <algorithm>
#include "CardboardDeviceParams.h"
#include "DistortionRenderer.h"
#include "EyeParams.h"

#include "HeadTransform.h"
#include "HeadMountedDisplay.h"
#include "HeadSensorWrapper.h"

#include "Viewport.h"

#include "base/CCDirector.h"
#include "base/ccMacros.h"
#include "renderer/CCFrameBuffer.h"
#include "renderer/ccGLStateCache.h"

#ifdef DEBUG

inline void checkGLError()
{
    CHECK_GL_ERROR_DEBUG();
}

#else

#define checkGLError() ;

#endif

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
        
    
        director->drawVRScene(leftEyeParams->getTransform()->getEyeView());
        checkGLError();
    
        if (rightEyeParams == nullptr) { return; }
    
        rightEyeParams->getViewport()->setGLViewport();
        rightEyeParams->getViewport()->setGLScissor();
    
        checkGLError();
    
//        [self.stereoRendererDelegate drawEyeWithTransform:rightEyeParams->getTransform()
//                                                  eyeType:rightEyeParams->getEye()];
        director->drawVRScene(rightEyeParams->getTransform()->getEyeView());
    
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

CocosVRRender::CocosVRRender()
{
    headSensor = new HeadSensorWrapper();
    headTransform = new HeadTransform();
    headMountedDisplay = nullptr;//
    
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

CocosVRRender::~CocosVRRender()
{
    delete headSensor;
    delete headTransform;
    delete headMountedDisplay;
    
    delete monocularParams;
    delete leftEyeParams;
    delete rightEyeParams;
    
    delete distortionRenderer;
    delete renderer;
}

void CocosVRRender::startHeadTracking()
{
    headSensor->startTracking();
}

void CocosVRRender::stopHeadTracking()
{
    headSensor->stopTracking();
}

void CocosVRRender::setVRModeEnable(bool isEnable)
{
    if (isVRModeEnabled != isEnable)
    {
        isVRModeEnabled = isEnable;
        isVRModeEnabled ? startHeadTracking() : stopHeadTracking();
    }
}

const float* CocosVRRender::getHeadEuler()
{
    auto dir = headTransform->getEulerAngles();
    headEuler[0] = dir.x;
    headEuler[1] = dir.y;
    headEuler[2] = dir.z;
    return headEuler;
}

void CocosVRRender::setHeadView(const cocos2d::Mat4& headview)
{
    headSensor->setHeadView(headview);
}

void CocosVRRender::render()
{
    if (headMountedDisplay == nullptr)
        headMountedDisplay = new HeadMountedDisplay();
    
    CardboardDeviceParams *cardboardDeviceParams = headMountedDisplay->getCardboard();
    
    headTransform->setHeadView(headSensor->getLastHeadView());
    float halfInterpupillaryDistance = cardboardDeviceParams->getInterpupillaryDistance() * 0.5f;
    
    // NSLog(@"%@", NSStringFromGLKMatrix4(_headTracker->getLastHeadView()));
    
    if (isVRModeEnabled)
    {
        cocos2d::Mat4 leftEyeTranslate;
        cocos2d::Mat4 rightEyeTranslate;
        
        cocos2d::Mat4::createTranslation(cocos2d::Vec3(-halfInterpupillaryDistance, 0, 0), &leftEyeTranslate);//to do
        cocos2d::Mat4::createTranslation(cocos2d::Vec3(halfInterpupillaryDistance, 0, 0), &rightEyeTranslate);
        
        // NSLog(@"%@", NSStringFromGLKMatrix4(_headTransform->getHeadView()));
        
        leftEyeParams->getTransform()->setEyeView( leftEyeTranslate * headTransform->getHeadView());
        rightEyeParams->getTransform()->setEyeView( rightEyeTranslate * headTransform->getHeadView());
        
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
            cocos2d::Mat4 perspectMat;
            cocos2d::Mat4::createPerspective(CC_DEGREES_TO_RADIANS(headMountedDisplay->getCardboard()->getFovY()), aspectRatio, zNear, zFar, &perspectMat);
            monocularParams->getTransform()->setPerspective(perspectMat);
        }
        else if (distortionCorrectionEnabled)
        {
            
            updateFovs(leftEyeParams->getFov(), rightEyeParams->getFov());
            distortionRenderer->onProjectionChanged(headMountedDisplay, leftEyeParams, rightEyeParams, zNear, zFar);
        }
        else
        {
            float eyeToScreenDistance = cardboardDeviceParams->getVisibleViewportSize() / 2.0f / tanf(CC_RADIANS_TO_DEGREES(cardboardDeviceParams->getFovY()) / 2.0f );
            
            float left = screenParams->getWidthMeters() / 2.0f - halfInterpupillaryDistance;
            float right = halfInterpupillaryDistance;
            float bottom = cardboardDeviceParams->getVerticalDistanceToLensCenter() - screenParams->getBorderSizeMeters();
            float top = screenParams->getBorderSizeMeters() + screenParams->getHeightMeters() - cardboardDeviceParams->getVerticalDistanceToLensCenter();
            
            FieldOfView *leftEyeFov = leftEyeParams->getFov();
            leftEyeFov->setLeft(CC_RADIANS_TO_DEGREES(atan2f(left, eyeToScreenDistance)));
            leftEyeFov->setRight(CC_RADIANS_TO_DEGREES(atan2f(right, eyeToScreenDistance)));
            leftEyeFov->setBottom(CC_RADIANS_TO_DEGREES(atan2f(bottom, eyeToScreenDistance)));
            leftEyeFov->setTop(CC_RADIANS_TO_DEGREES(atan2f(top, eyeToScreenDistance)));
            
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

void CocosVRRender::updateFovs(FieldOfView* leftEyeFov, FieldOfView* rightEyeFov)
{
    CardboardDeviceParams *cardboardDeviceParams = headMountedDisplay->getCardboard();
    ScreenParams *screenParams = headMountedDisplay->getScreen();
    Distortion *distortion = cardboardDeviceParams->getDistortion();
    
    float idealFovAngle = CC_RADIANS_TO_DEGREES(atan2f(cardboardDeviceParams->getLensDiameter() / 2.0f,
                                                         cardboardDeviceParams->getEyeToLensDistance()));
    float eyeToScreenDistance = cardboardDeviceParams->getEyeToLensDistance() + cardboardDeviceParams->getScreenToLensDistance();
    float outerDistance = ( screenParams->getWidthMeters() - cardboardDeviceParams->getInterpupillaryDistance() ) / 2.0f;
    float innerDistance = cardboardDeviceParams->getInterpupillaryDistance() / 2.0f;
    float bottomDistance = cardboardDeviceParams->getVerticalDistanceToLensCenter() - screenParams->getBorderSizeMeters();
    float topDistance = screenParams->getHeightMeters() + screenParams->getBorderSizeMeters() - cardboardDeviceParams->getVerticalDistanceToLensCenter();
    
    float outerAngle = CC_RADIANS_TO_DEGREES(atan2f(distortion->distort(outerDistance), eyeToScreenDistance));
    float innerAngle = CC_RADIANS_TO_DEGREES(atan2f(distortion->distort(innerDistance), eyeToScreenDistance));
    float bottomAngle = CC_RADIANS_TO_DEGREES(atan2f(distortion->distort(bottomDistance), eyeToScreenDistance));
    float topAngle = CC_RADIANS_TO_DEGREES(atan2f(distortion->distort(topDistance), eyeToScreenDistance));
    
    leftEyeFov->setLeft(std::min(outerAngle, idealFovAngle));
    leftEyeFov->setRight(std::min(innerAngle, idealFovAngle));
    leftEyeFov->setBottom(std::min(bottomAngle, idealFovAngle));
    leftEyeFov->setTop(std::min(topAngle, idealFovAngle));
    
    rightEyeFov->setLeft(std::min(innerAngle, idealFovAngle));
    rightEyeFov->setRight(std::min(outerAngle, idealFovAngle));
    rightEyeFov->setBottom(std::min(bottomAngle, idealFovAngle));
    rightEyeFov->setTop(std::min(topAngle, idealFovAngle));
}
