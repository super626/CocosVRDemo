#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();


    // return the scene
    return layer;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    if (!initWithPhysics())
    {
        return false;
    }
    auto size = Director::getInstance()->getWinSize();
    auto camera = Camera::createPerspective(60, size.width / size.height, 0.1f, 500.f);
    
    camera->setCameraFlag(CameraFlag::USER1);
    auto brush = CameraBackgroundBrush::createSkyboxBrush("res/effects/skybox/right.jpg", "res/effects/skybox/left.jpg", "res/effects/skybox/top.jpg", "res/effects/skybox/bottom.jpg", "res/effects/skybox/back.jpg", "res/effects/skybox/front.jpg");
    camera->setBackgroundBrush(brush);
    addChild(camera);
    
    Terrain::DetailMap r("res/models/terrain/dirt.jpg"), g("res/models/terrain/Grass2.jpg"), b("res/models/terrain/rock.png"), a("res/models/terrain/snow.png");
    Terrain::TerrainData data("res/models/terrain/terrain.png", "res/models/terrain/alphamap.png", r, g, b, a);
    data._mapHeight = 100.0;
    data._mapScale = 1.0f;
    auto terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
    terrain->setLODDistance(3.2f, 6.4f, 9.6f);
    terrain->setMaxDetailMapAmount(4);
    terrain->setCameraMask((unsigned short)CameraFlag::USER1);
    terrain->setDrawWire(false);
    terrain->setLightDir(-Vec3::UNIT_Y);
    terrain->setCameraMask(2);
    addChild(terrain);
    
    _player = Tank::create();
    float h = terrain->getHeight(0.f, 80.f) + 1.f;
    _player->setPosition3D(Vec3(0.0f, h, 80.0f));
    _player->setCameraMask((unsigned short)CameraFlag::USER1);
    this->addChild(_player);
    
    auto pos = _player->getPosition3D();
    pos.y += 2.8f;
    camera->setPosition3D(Vec3(pos.x, pos.y, pos.z));
    camera->lookAt(pos);
//    setPhysics3DDebugCamera(camera);
//    getPhysics3DWorld()->setDebugDrawEnable(true);
    getPhysics3DWorld()->setGravity(Vec3(0, -0.5, 0));
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
