#include "Ball.h"
#include <data/Resource.h>
#include <modules/core/ExtensionMath.hpp>
#include <modules/core/PhysicsConstants.hpp>
#include <cmath>
#include <data/GlobalVar.h>
#include <modules/core/GameMgr.h>

USING_NS_CC;
using namespace ps;

Ball* Ball::createBall()
{
	return Ball::create();
}

Ball* Ball::createBall(int id)
{
	Ball *ball = Ball::create();
	ball->body()->setId(id);
	return ball;
}


bool Ball::init()
{
	if (!Node::init()) {
		return false;
	}
	this->initBall();
	this->initPhysicsBody();
	return true;
}

void Ball::initBall()
{
	//TODO
	auto ball = Sprite3D::create(Res::BALL_HI_POLY_2);
	ball->setScale(1);
	this->_ball = ball;
	this->_specular = 4;
	this->metal = 0.02;
	this->rough = 0.23;

	/*auto properties = Properties::createNonRefCounted("res/material/3d_ball.material#ball");

	this->_mat = Material::createWithProperties(properties);
	ball->setMaterial(this->_mat);*/
	// this->initShader();
	this->addChild(ball);
	this->initShadow();
	this->initHighlight();

	// DEBUG
	this->initDebugIndicator();
	// this->setMovingArrowEnabled(false);

}

void Ball::initPhysicsBody()
{
	auto body = new ps::BallBody();
	this->_body = body;
	std::function<void()> func = std::bind(&Ball::onBodyChange, this);
	this->_body->setSubcribe(func);
}

void Ball::setNumber(int number)
{
	this->_number = number;
	this->_ball->setTexture(Res::Ball_Textures[(number - 1) % 15 + 1]);
}

void Ball::initShadow()
{
	//todo
	CCLOG("initShadow");
	auto shadow = Sprite3D::create();
	auto circle = Sprite::create(Res::BALL_SHADOW);
	auto node = Node::create();
	circle->setScale(0.018);
	circle->setRotation3D(Vec3(-90, 0, 0));
	circle->setOpacity(120);

	auto circle2 = Sprite::create(Res::BALL_SHADOW);
	circle2->setScale(0.018);
	circle2->setPosition(-1.5, 0);
	circle2->setOpacity(120);
	circle2->setRotation3D(Vec3(-90, 0, 0));
	node->addChild(circle2);

	auto circle3 = Sprite::create(Res::BALL_SHADOW);
	circle3->setScale(0.018);
	circle3->setPosition(1.5, 0);
	circle3->setOpacity(120);
	circle3->setRotation3D(Vec3(-90, 0, 0));
	node->addChild(circle3);

	shadow->addChild(node);
	node->addChild(circle);
	shadow->setPosition3D(Vec3(0, 1, 0));
	this->addChild(shadow);
	this->_shadow = shadow;
}

void Ball::initHighlight()
{
	//todo
	CCLOG("initHighlight");
}

void Ball::initDebugIndicator()
{
	//todo
	CCLOG("initDebugIndicator");
}

BallBody* Ball::body()
{
	return this->_body;
}

void Ball::update(float dt)
{
	Node::update(dt);

	//if (this->_updateRotate) {
	//	auto quat = this->_ball->getRotationQuat();
	//	auto ourQuat = ExtMath::quaternion(quat.w, quat.x, -quat.z, quat.y);

	//	const offset = gv.gameMgr.cue._offset;
	//	auto direction = ExtMath::vector( gv.gameMgr.cue._direction.x,gv.gameMgr.cue._direction.y );
	//	double dirLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	//	direction.x /= dirLength; direction.y /= dirLength;

	//	const rotateXVelo = offset.v * dt * ExtMath::PI * (-direction.y);
	//	const rotateYVelo = offset.h * dt * ExtMath::PI;
	//	const rotateZVelo = offset.v * dt * ExtMath::PI * direction.x;

	//	const rotate = gv.ExtMath.quaternion.quatFromVec({ x: rotateXVelo, y : rotateZVelo, z : rotateYVelo });
	//	ourQuat = gv.ExtMath.quaternion.multiply(rotate, ourQuat);
	//	quat = new cc.math.quaternion(ourQuat.x, ourQuat.z, -ourQuat.y, ourQuat.w);
	//	this->_ball->setRotationQuat(quat);
	//	this->body().quaternion = ourQuat;
	//}

	if (this->_extrapolatePosition) {
		auto delta = gv::ALMOST_REALTIME_UPDATE_DELTA;
		if (this->_updateDelay > delta * 3 / 2) {
			this->_extrapolatePosition = false;
			// this->body().position = this->_newPos;
			return;
		}
		this->_updateDelay += dt;
		auto newPos = ps::ExtMath::vector::mix(this->_oldPos, this->_newPos, this->_updateDelay / delta);
		this->body()->position() = newPos;
		// cc.log("extrapolating " + JSON.stringify(newPos));
	}
}

void Ball::onBodyChange()
{
	this->setBallPosition(this->_body->position());
	this->setBallQuaternion(this->_body->quaternionRotation());
}

void Ball::setBallPosition(ps::ExtMath::vector position)
{
	auto onTableEdge = ps::PhysicsWorld::floorSurface()->isOnTableEdges(position);
	auto outsideTable = ps::PhysicsWorld::floorSurface()->isOutSideTable(position);

	auto minZ = onTableEdge ? std::min(position.z + 0.1, 0.5 + ps::PhysicsConstants::CUSHION_HEIGHT) : std::min(position.z + 0.1, 0.1);
	if (!onTableEdge && outsideTable) minZ = -50;
	this->_shadow->setPosition3D(Vec3(
		position.x,
		minZ,
		-position.y
	));
	this->_shadow->setScale(1 + position.z / 75);
	//// auto opacity =Math.max(30, 120 - (position.z - minZ) * 4.5);
	//// this->_shadow.circle.setSubShadowOpacityAndBlur(opacity);
	//// this->_shadow.circle.setSubShadowsDistance(1.5 + (position.z - minZ)*0.015);
	this->_ball->setPosition3D(Vec3(
		position.x,
		ps::PhysicsConstants::BALL_RADIUS + position.z,
		-position.y
	));
	this->_body->setPosition(position);

	this->setVisible(!this->_body->isTotallyInHole() && !this->_body->isDisable());
		
	if (this->_draw != nullptr) {
		this->_draw->setPosition3D(this->_ball->getPosition3D());
	}
	//if (this->_positionCallback) {
	//	auto previousPosition = this->_previousPosition || position;
	//	auto delta = gv.ExtensionMath.vector.sub(position, previousPosition);
	//	this->_positionCallback.call(null, position, delta);
	//	this->_previousPosition = position;
	//}

	//if (gv::gameMgr->table != nullptr) {
	//	gv::gameMgr->table->onBallChangePosition(this);
	//}

}

void Ball::setBallQuaternion(ps::ExtMath::quaternion quaternion)
{
	auto quat = cocos2d::Quaternion(
		quaternion.x,
		quaternion.z,
		-quaternion.y,
		quaternion.w
	);
	this->_ball->setRotationQuat(quat);
	// this.updateQuaternionShader(quat);
}
