#include "Tank.h"
#include "vr/CocosVRRender.h"

USING_NS_CC;

#define BULLET_RELOAD_TIME 3.0
#define MOVE_AREA_RADIUS   150.0f
#define SMOKE_PS_TAG  0x0F

void recycleSetTexture(Node *node, Texture2D *texture)
{
	if (dynamic_cast<Sprite3D *>(node)) {
		dynamic_cast<Sprite3D *>(node)->setTexture(texture);
	}
	for (auto &child : node->getChildren()) {
		recycleSetTexture(child, texture);
	}
}

Tank::Tank()
	: _cannonStageAngle(180.0f)
	, _cannonGunAngle(0.0f)
	, _hp(100.0f)
	, _latestShootTime(0.0f)
    , _cannonStageSpeed(90)
    , _stillTime(0)
{

}

bool Tank::init()
{
	Sprite3D::initWithFile("res/models/tank/tank.c3b");
	this->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	this->setPosition3D(Vec3::ZERO);
	this->setScale(1.0f);

	_cannon = this->getChildByName("cannon");
	_gunfire = PUParticleSystem3D::create("res/effects/Particle3D/scripts/gunfire.pu");
	_cannon->getChildByName("gun")->getChildByName("gunfire")->addChild(_gunfire);

	scheduleUpdate();
	return true;
}

void Tank::shot(float speed)
{
	if (BULLET_RELOAD_TIME < _latestShootTime) {
		shotBullet(speed);
		_gunfire->startParticleSystem();
		_latestShootTime = 0.0f;
	}
}

void Tank::shot(const Vec3 &target, float speed)
{
	float dis = (target - this->getPosition3D()).length();
	float gunAngle = dis / 14.5f - 6.0f;
	rotateCannonGun(gunAngle - _cannonGunAngle);

	Vec3 dir = target - this->getPosition3D();
	dir.normalize();
	Quaternion rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(_cannonStageAngle - 180.0f)) * this->getRotationQuat();
	Vec3 gunDir = rot * -Vec3::UNIT_Z;
	gunDir.normalize();
	float theta = acos(clampf(gunDir.dot(dir), 0.0f, 1.0f));
	Vec3 up;
	Vec3::cross(gunDir, dir, &up);
	if (up.y < 0.0)
		theta = -theta;
	rotateCannonStage(CC_RADIANS_TO_DEGREES(theta));
	shot(speed);
}

void Tank::rotateCannonStage(float angle)
{
	_cannonStageAngle += angle;
	_cannon->setRotation3D(Vec3(-90.0f, _cannonStageAngle, 0.0f));
}

void Tank::rotateCannonGun(float angle)
{
	_cannonGunAngle += angle;
	if (10.0f < _cannonGunAngle) _cannonGunAngle = 10.0f;
	if (_cannonGunAngle < -5.0f) _cannonGunAngle = -5.0f;
	auto gun = _cannon->getChildByName("gun");
	gun->setRotation3D(Vec3(-_cannonGunAngle, 0.0f, 0.0f));
}

bool Tank::move(float force)
{
	Vec3 requestPos = Vec3(this->getPosition3D() + this->getRotationQuat() * Vec3(0.0f, 0.0f, -force));
	if (requestPos.length() < MOVE_AREA_RADIUS) {
		this->setPosition3D(requestPos);
		return true;
	}
	return false;
}

void Tank::turn(float torque)
{
	auto rot = Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(torque));
	this->setRotationQuat(this->getRotationQuat() * rot);
}

void Tank::shotBullet(float speed)
{
    Director::getInstance()->getRunningScene()->removeChildByTag(100);
    
	Mat4 bulletWorldMat = _cannon->getChildByName("gun")->getChildByName("gunfire")->getNodeToWorldTransform();
	Vec3 bulletPos;
	Quaternion bulletRot;
	bulletWorldMat.decompose(nullptr, &bulletRot, &bulletPos);

	Physics3DRigidBodyDes rbDes;
	rbDes.mass = 100.f;
	rbDes.shape = Physics3DShape::createCapsule(0.1f, 0.2f);
	auto bullet = PhysicsSprite3D::create("res/models/tank/bullet.c3b", &rbDes);
    bullet->setScale(2.f);
	bullet->setTexture("res/models/tank/orange_edit.png");
	Director::getInstance()->getRunningScene()->addChild(bullet, 100);

	Vec3 rotate = this->getRotation3D();

	Vec3 linearVel = Vec3(cos(CC_DEGREES_TO_RADIANS(_cannonGunAngle)) * sin(CC_DEGREES_TO_RADIANS(_cannonStageAngle))
		, sin(CC_DEGREES_TO_RADIANS(_cannonGunAngle))
		, cos(CC_DEGREES_TO_RADIANS(_cannonGunAngle)) * cos(CC_DEGREES_TO_RADIANS(_cannonStageAngle)));

	linearVel = this->getRotationQuat() * linearVel;

	auto rigidBody = static_cast<Physics3DRigidBody*>(bullet->getPhysicsObj());
	rigidBody->setLinearFactor(Vec3::ONE);
	rigidBody->setLinearVelocity(linearVel * speed);
	rigidBody->setAngularVelocity(Vec3::ZERO);
	rigidBody->setCcdMotionThreshold(0.5f);
	rigidBody->setCcdSweptSphereRadius(0.4f);
	//rigidBody->setMask(0xffffffff);
	rigidBody->setUserData(bullet);

	bullet->setCameraMask(this->getCameraMask());
	bullet->setPosition3D(bulletPos);
	bullet->setRotationQuat(Quaternion(Vec3::UNIT_Y, CC_DEGREES_TO_RADIANS(_cannonStageAngle)) * this->getRotationQuat() * Quaternion(Vec3::UNIT_X, CC_DEGREES_TO_RADIANS(-_cannonGunAngle + 85.0f)));
	//bullet->setRotation3D(this->getRotationQuat() * Vec3(0.0f, _cannonStageAngle, 0.0f) + Vec3(-_cannonGunAngle + 85.0f, 0.0f, 0.0f));
	bullet->syncNodeToPhysics();
	bullet->setSyncFlag(Physics3DComponent::PhysicsSyncFlag::PHYSICS_TO_NODE);


	rigidBody->setCollisionCallback([bullet, this](const Physics3DCollisionInfo &ci) {
		if (!ci.collisionPointList.empty()) {
			bullet->runAction(CallFunc::create([bullet]() {
				bullet->removeFromParent();
			}));
			if (this->_callBack != nullptr) {
				this->_callBack(ci);
			}
		}
	});
}

void Tank::setTexture(const std::string &texFile)
{
	auto tex = Director::getInstance()->getTextureCache()->addImage(texFile);
	recycleSetTexture(_cannon, tex);
	recycleSetTexture(this->getChildByName("body"), tex);
}

void Tank::setHP(float hp)
{
	if (hp < 0.0)
		_hp = 0.0;
	else
		_hp = hp;

	if (_hp == 0.0f) {
		this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([this]() {
			this->removeFromParent();
		}), nullptr));

		auto explosion = PUParticleSystem3D::create("res/effects/Particle3D/scripts/explosionSystem.pu");
		explosion->setScale(5.0f);
		explosion->startParticleSystem();
		explosion->setCameraMask(this->getCameraMask());
		explosion->setPosition3D(this->getPosition3D());
		Director::getInstance()->getRunningScene()->addChild(explosion);
		explosion->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([explosion]() {
			explosion->removeFromParent();
		}), nullptr));
	}
	else if (_hp <= 50.0f) {
		if (!this->getChildByTag(SMOKE_PS_TAG)) {
			auto smoke = PUParticleSystem3D::create("res/effects/Particle3D/scripts/smoke.pu");
			smoke->setScale(0.05f);
			smoke->startParticleSystem();
			smoke->setCameraMask(this->getCameraMask());
			this->addChild(smoke, 0, SMOKE_PS_TAG);
		}
	}
}

void Tank::update(float delta)
{
	_latestShootTime += delta;
    
    const float* dir = Director::getInstance()->getVRRender()->getHeadEuler();
    float angle = dir[1];//
    angle = CC_RADIANS_TO_DEGREES(angle) - _cannonStageAngle + 180;
    static float stillEps = CC_RADIANS_TO_DEGREES(1);
    if (angle < stillEps)
        _stillTime += delta;
    else
        _stillTime = 0.f;
    if (_stillTime > 2.f)
        shot(10.f);
    
    float maxAngle = delta * _cannonStageSpeed;
    if (angle > maxAngle)
        angle = maxAngle;
    else if(angle < -maxAngle)
        angle = -maxAngle;
    rotateCannonStage(angle);
    
    angle = dir[0];//
    angle = CC_RADIANS_TO_DEGREES(angle) - _cannonGunAngle;
    rotateCannonGun(angle);
}
