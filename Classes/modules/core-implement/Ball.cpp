#include "Ball.h"
#include <data/Resource.h>
#include <modules/core/ExtensionMath.hpp>
#include <cmath>
#include <data/GlobalVar.h>

USING_NS_CC;
using namespace ps;

Ball* Ball::createBall()
{
	return Ball::create();
}

bool Ball::init()
{
	if (!Node::init()) {
		return false;
	}
	this->initBall();
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

void Ball::setNumber(int number)
{
	this->_number = number;
	this->_ball->setTexture(Res::Ball_Textures[(number - 1) % 15 + 1]);
}

void Ball::initShadow()
{
	//todo
	CCLOG("initShadow");
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
