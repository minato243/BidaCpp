#include "Cue.h"
#include <data/Resource.h>
#include <modules/core/PhysicsConstants.hpp>

USING_NS_CC;
using namespace ps;

const double Cue::OFFSET_DISTANCE = -4;
const double Cue::MOVE_IN_DISTANCE = -100;
const double Cue::MOVE_OUT_DISTANCE = -100;

Cue::Cue():_cue(nullptr)
{
	PhysicsCue();
	this->init();
}

Cue::~Cue()
{
	CC_SAFE_DELETE(this->_moveInLerp);
	CC_SAFE_DELETE(this->_moveOutLerp);
	CC_SAFE_DELETE(this->_rotateLerp);
}


bool Cue::init()
{
	if (!Node::init()) {
		return false;
	}
	this->initCue();
	std::function<void(double)> moveInFunc = std::bind(&Cue::updateCueMoveIn, this , std::placeholders::_1);
	std::function<void(double)> moveOutFunc = std::bind(&Cue::updateCueMoveOut, this, std::placeholders::_1);
	std::function<void(ps::ExtMath::vector)> cueRotateFunc = std::bind(&Cue::updateCueRotate, this, std::placeholders::_1);
	this->_moveInLerp = new NumberLerp(0.08, moveInFunc);
	this->_moveOutLerp = new NumberLerp(0.125, moveOutFunc);
	this->_rotateLerp = new VectorLerp(0.125, cueRotateFunc);
	return true;
}

void Cue::initCue()
{
	CueModelData modelData;
	modelData.model = Res::CUE_3D;
	this->loadModel(modelData);
}

void Cue::loadModel(CueModelData paths)
{
	if (_cue != nullptr) {
		_cue->removeFromParent();
	}
	_cue = Sprite3D::create(paths.model);
	this->addChild(_cue);

	bool isDefault = paths.model == Res::CUE_3D;
	if (!isDefault) {
		_cue->setTexture(paths.tex);
		_cue->setRotation3D(Vec3(0, 90, 0));
		//_cue->_MRAOTexturePath = paths.mrao;
		//this->setUpShader(_cue, this->shaderPrograms[this->qualityLevel], this->qualityLevel);
	}
	_cue->setScale(isDefault ? 1 : 0.45);
	if (!isDefault) {
		_cue->setPosition3D(Vec3(Cue::OFFSET_DISTANCE, 0, 0));
	}
}

void Cue::cueMoveByForce(double forcePercent) {
	auto pos = this->_cue->getPosition3D();
	pos.x = Cue::OFFSET_DISTANCE - forcePercent / 100 * 50;
	this->setAllCueModelPosition3D(pos);
}

void Cue::updateCueMoveIn(double x) {
	auto pos = this->_cue->getPosition3D();
	pos.x = x;
	this->setAllCueModelPosition3D(pos);
}

void Cue::updateCueMoveOut(double x) {
	auto pos = this->_cue->getPosition3D();
	pos.x = x;
	this->setAllCueModelPosition3D(pos);
	if (abs(x - Cue::MOVE_OUT_DISTANCE) < 1) {
		CCLOG("Disable");
		this->setVisible(false);
		pos.x = Cue::OFFSET_DISTANCE;
		this->setAllCueModelPosition3D(pos);
		this->_moveOutLerp->setEnabled(false);
	}
}

void Cue::setAllCueModelPosition3D(Vec3 pos) 
{

}

//void Cue::initShader(shader, level) {
//this->qualityLevel = level;
//this->shaderPrograms[level] = shader;
//
//for (auto i = 0; i < this->_cues.length; i++) {
//	auto cue = this->_cues[i];
//
//	this->setUpShader(cue, shader, level);
//}
//},
//
//setUpShader: function(cue, shader, level) {
//	auto state = cc.GLProgramState.create(shader);
//
//	if (level >= 0) {
//		state.setUniformVec3("u_light_color", cc.math.vec3(1, 1, 1));
//		state.setUniformVec3("u_view_pos", cc.math.vec3(0, 160, 0));
//	}
//	if (level >= 0) {
//		auto mraoMap = cc.textureCache.addImage(cue._MRAOTexturePath || res.Cue_MRAO);
//		state.setUniformTexture("u_mrao_map", mraoMap);
//		// state.setUniformFloat("u_lod_level", 1);
//		state.setUniformTexture("u_env_map", gv.settingMgr._tableEnvTexture);
//		state.setUniformTexture("u_irradiance_map", gv.settingMgr._tableIrradianceTexture);
//		state.setUniformTexture("u_brdf_lut", gv.settingMgr._brdfTexture);
//		state.setUniformFloat('u_metallic', -1);
//		state.setUniformFloat('u_roughness', -1);
//		state.setUniformFloat("u_opacity", 1);
//	}
//
//	cue.setGLProgramState(state);
//	cue.setLightMask(0);
//
//	cue._state = state;
//	this->setLights(Ball.DefaultLights, Ball.DefaultLightColor);
//},

void Cue::setCuePosition(vector position) {
	this->setPosition3D(Vec3(
		position.x,
		position.z + ps::PhysicsConstants::BALL_RADIUS,
		-position.y
	));
}

void Cue::updateCueRotate(vector direction) {
	this->setCueDirection(direction);
}

void Cue::setCueDirection(const ps::ExtMath::vector& direction) {
	CCLOG(" Cue::setCueDirection");
	auto l1 = direction.x * direction.x + direction.y * direction.y;
	auto l2 = l1 + direction.z * direction.z;

	auto pitch = std::acos(std::min(1., sqrt(l1 / l2)));

	auto yaw = std::acos(std::min(1., direction.x / sqrt(l1)));
	if (direction.y < 0) {
		yaw = 2 * ExtMath::PI - yaw;
	}

	// // Add 5 degree to make the cue visually higher => not touch the table
	auto quaternion_1 = ExtMath::quaternion::quaternion(
		ExtMath::vector(0, -pitch, 0)
	);
	auto quaternion_2 = ExtMath::quaternion::quaternion(
		ExtMath::vector(0, 0, yaw)
	);
	auto quaternion = ExtMath::quaternion::multiply(quaternion_1, quaternion_2);
	// auto quaternion = ExtMath::quaternion::quatFromVec(direction);
	auto curQua = this->getRotationQuat();
	this->setRotationQuat(cocos2d::Quaternion(
		quaternion.x,
		quaternion.z,
		quaternion.y,
		quaternion.w
	));

	// this->updateGuildLine(-yaw, pitch);

	this->setDirection(ExtMath::vector::normalize(direction));
}

void Cue::doShootAnimation(double force, std::function<void()> &callback) {
	auto p = this->getPosition3D();
	auto dir = this->getDirection();
	force *= 100.f / 600;
	// this->moveTo3D(0.5, cc.math.vec3(p.x - dir.x * force, p.y - dir.z * force, p.z + dir.y * force), function(){
	// 	this->moveTo3D(0.1, p, function(){
	// 		CCLOG("SHOOT!");
	// 	})
	// }.bind(this));
	this->chainMoveTo3D(std::vector<float>{0.3, 0.1}, std::vector<Vec3>{
		Vec3(p.x - dir.x * force, p.y - dir.z * force, p.z + dir.y * force),
		Vec3(p.x + dir.x * force * 0.05, p.y + dir.z * force * 0.05, p.z - dir.y * force * 0.05)
	}, callback);
}

void Cue::chainMoveTo3D(std::vector<float> intervals, std::vector<cocos2d::Vec3> positions, std::function<void()> callback)
{
	if(positions.size() != intervals.size()) {
		CCLOG("need equal positions length + intervals length");
		return;
	}
	if(positions.size() < 1) return;		
	
	auto interval = intervals[0];
	auto position = positions[0];
	std::vector<float> newIntervals = std::vector<float>(intervals.begin()+1, intervals.end());
	std::vector<Vec3> newPositions = std::vector<Vec3>(positions.begin() + 1, positions.end());

	std::function<void()> newCallback = std::bind(&Cue::chainMoveTo3D, this, newIntervals, newPositions, callback);
	moveTo3D(interval, position, newCallback);
}

void Cue::moveTo3D(float interval, cocos2d::Vec3 position, std::function<void()> callback)
{
	this->_moveToStartPos = this->getPosition3D();
	this->_moveTo = position;
	this->_moveToCallback = callback;
	this->_moveToCurrentInterval = 0;
	this->_moveToInterval = interval;
}

void Cue::onEnter()
{
	Node::onEnter();
	this->scheduleUpdate();
}

void Cue::update(float dt)
{
	Node::update(dt);
	this->_moveInLerp->update(dt); 
	this->_moveOutLerp->update(dt);

	if (this->_moveToCurrentInterval < this->_moveToInterval) {
		auto  lastMoveToUpdate = false;
		this->_moveToCurrentInterval += dt;
		if (this->_moveToCurrentInterval >= this->_moveToInterval) {
			this->_moveToCurrentInterval = this->_moveToInterval;
			lastMoveToUpdate = true;
		}
		auto s = this->_moveToStartPos; // start
		auto d = this->_moveTo;			// destination
		auto cur = this->_moveToCurrentInterval;
		auto t = this->_moveToInterval;
		this->setPosition3D(
			Vec3(
				s.x * (t - cur) / t + d.x * cur / t,
				s.y * (t - cur) / t + d.y * cur / t,
				s.z * (t - cur) / t + d.z * cur / t
			));
		if (lastMoveToUpdate) {
			this->_moveToCallback();
		}
	}
	// if(this->_extrapolateDirection){
	// 	auto  delta = gv.ALMOST_REALTIME_UPDATE_DELTA;
	// 	auto  delay = this->_updateDelay;
	// 	if(delay > delta*3.5){
	// 		this->_extrapolatePosition = false;
	// 		this->_updateDelay = 0;
	// 		this->setCueDirection(gv.ExtensionMath.vector.unit(this->_newDir));
	// 		// this->body().position = this->_newPos;
	// 		if(this->_onDirectionChangeCallback){
	// 			this->_onDirectionChangeCallback(this->_newDir);
	// 		}
	// 		return;
	// 	}
	//
	// 	if(this->_updateDelay > delta*1.5){
	// 		this->_oldDir = this->_curDir;
	// 		delay = dt;
	// 	}
	// 	// this->_updateDelay += dt;
	// 	auto  newDir = gv.ExtensionMath.vector.mix(this->_oldDir, this->_newDir, delay/delta);
	// 	this->_curDir = newDir;
	// 	if(this->_onDirectionChangeCallback){
	// 		this->_onDirectionChangeCallback(newDir);
	// 	}
	// 	this->setCueDirection(gv.ExtensionMath.vector.unit(newDir));
	// 	// cc.log("extrapolating " + JSON.stringify(newPos));
	// 	this->_updateDelay += dt;
	// }
}

void Cue::animateVisible(){
	this->setVisible(true);
	// const pos = this->_cue.getPosition3D();
	// this->_cue.setPosition3D(cc.math.vec3(-100, pos.y, pos.z));

	this->_moveInLerp->setCurrent(Cue::MOVE_IN_DISTANCE);
	this->_moveInLerp->setTarget(Cue::OFFSET_DISTANCE);
	this->_moveInLerp->setEnabled(true);
	this->updateCueMoveIn(Cue::MOVE_IN_DISTANCE);
}

void Cue::animateInvisible(){
	this->_moveOutLerp->setCurrent(Cue::OFFSET_DISTANCE);
	this->_moveOutLerp->setTarget(Cue::MOVE_OUT_DISTANCE);
	this->_moveOutLerp->setEnabled(true);
	this->updateCueMoveOut(Cue::OFFSET_DISTANCE);
}

void Cue::setVisible(bool visible) {
	Node::setVisible(visible);
	if (!visible) {
		this->_moveInLerp->setEnabled(false);
	}
}
