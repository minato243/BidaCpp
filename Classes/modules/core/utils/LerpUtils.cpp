#include "LerpUtils.h"
#include <data/GlobalVar.h>

USING_NS_CC;
using namespace ps;

const double NUMBER_LERP_STOP_RPT = 1e-4;
const double VECTOR_LERP_STOP_RPT = 1e-4;
const double SPHERICAL_LERP_STOP_RPT = 1e-4;

SphericalLerp:: SphericalLerp(double lerp, int context, std::function<void(Rpt)> listener)
{
	_current = Rpt(0, 0 , 0);
	_target = Rpt(0, 0, 0);
	this->_enabled = false;
	this->_listener = listener;
	this->_context = context;
	this->_completedListener = nullptr;
}

SphericalLerp::~SphericalLerp()
{
	this->_completedListener = NULL;
}

void SphericalLerp::setCurrent(Rpt current)
{
	this->_current = current;
}

void SphericalLerp::setTarget(Rpt target)
{
	this->_target = target;
}

void SphericalLerp::fastForward()
{
	setCurrent(this->_target);
}

void SphericalLerp::dispatch()
{
	//TODO
}

void SphericalLerp::update(float deltaTime)
{
	if (!this->_enabled) return;

	if (this->_target.r == this->_current.r &&
		this->_target.p == this->_current.p &&
		this->_target.t == this->_current.t
		) {
		if (this->_completedListener) {
			auto listener = this->_completedListener;
			this->_completedListener = nullptr;
			listener->handler();
		}
		return;
	}

	auto lerp = this->_lerp * deltaTime;
	auto dr = this->_target.r - this->_current.r;
	auto dp = this->_target.p - this->_current.p;
	auto dt = this->_target.t - this->_current.t;

	if ((abs(dr) + abs(dp) + abs(dt)) < SPHERICAL_LERP_STOP_RPT) {
		this->_current.r = this->_target.r;
		this->_current.p = this->_target.p;
		this->_current.t = this->_target.t;
	}
	else {
		this->_current.r = this->_current.r + dr * lerp;
		this->_current.p = this->_current.p + dp * lerp;
		this->_current.t = this->_current.t + dt * lerp;
	}
	this->_listener(this->_current);
	
}

void SphericalLerp::simplifyRadian()
{
	this->_current.p = ExtMath::simplifyRadian(this->_current.p);
	this->_target.p = ExtMath::simplifyRadian(this->_target.p);
}

Rpt SphericalLerp::getCurrent()
{
	return this->_current;
}

void SphericalLerp::onceCompleted(std::function<void()> callback)
{
	/*this->_completedListener = {
			context: context,
			handler : callback
	};*/
}

SphericalLerp* SphericalLerp::makeLerp(double lerp, int context, std::function<void(Rpt)> listener)
{
	return new SphericalLerp(lerp, context, listener);
}

VectorLerp::VectorLerp(double lerp, std::function<void(ps::ExtMath::vector)> listener) 
{
	this->_lerp = lerp * 60;
	this->_current = ExtMath::vector();
	this->_target = ExtMath::vector();
	this->_enabled = false;
	this->_listener = listener;
	this->_completedListener = nullptr;
}

VectorLerp::~VectorLerp() {

}

void VectorLerp::setCurrent(ExtMath::vector current) {
	this->_current.x = current.x;
	this->_current.y = current.y;
	this->_current.z = current.z;
}

void VectorLerp::setTarget(ExtMath::vector target) {
	this->_target.x = target.x;
	this->_target.y = target.y;
	this->_target.z = target.z;
}

void VectorLerp::fastForward()
{
	this->setCurrent(this->_target);
}

void VectorLerp::dispatch() {
	this->_listener(this->_current);
}

void VectorLerp::update(float dt) {
	if (!this->_enabled) return;

	if (this->_target.x == this->_current.x &&
		this->_target.y == this->_current.y &&
		this->_target.z == this->_current.z
		) {
		this->_enabled = false;
		if (this->_completedListener) {
			auto listener = this->_completedListener;
			this->_completedListener = nullptr;
			listener->handler();
		}
		return;
	};

	auto lerp = this->_lerp * dt;
	auto dx = this->_target.x - this->_current.x;
	auto dy = this->_target.y - this->_current.y;
	auto dz = this->_target.z - this->_current.z;

	if ((abs(dx) + abs(dy) + abs(dz)) < VECTOR_LERP_STOP_RPT) {
		this->_current.x = this->_target.x;
		this->_current.y = this->_target.y;
		this->_current.z = this->_target.z;
	}
	else {
		this->_current.x = this->_current.x + dx * lerp;
		this->_current.y = this->_current.y + dy * lerp;
		this->_current.z = this->_current.z + dz * lerp;
	}
	this->_listener(this->_current);
}

ps::ExtMath::vector VectorLerp::getCurrent()
{
	return this->_current;
}


void VectorLerp::onceCompleted(CUSTOM_CALLBACK callback)
{
	this->_completedListener = new LerpListener();
	this->_completedListener->handler = callback;
}


NumberLerp::NumberLerp(double lerp, std::function<void(double)> listener)
{
	this->_lerp = lerp * 60;
	this->_current = 0;
	this->_target = 0;
	this->_enabled = false;
	this->_listener = listener;
}

NumberLerp::~NumberLerp()
{
}

void NumberLerp::setCurrent(double current) 
{
	this->_current = current;
}

void NumberLerp::setTarget(double target) 
{
	this->_target = target;
}

void NumberLerp::fastForward()
{
	this->setCurrent(this->_target);
}

void NumberLerp::dispatch()
{
	this->_listener(this->_current);
}

void NumberLerp::update(float dt) {
	if (!this->_enabled) return;
	if (this->_target == this->_current) return;

	auto lerp = this->_lerp * dt;
	auto d = this->_target - this->_current;

	if (abs(d) < NUMBER_LERP_STOP_RPT) {
		this->_current = this->_target;
	}
	else {
		this->_current = this->_current + d * lerp;
	}
	this->_listener(this->_current);
}

