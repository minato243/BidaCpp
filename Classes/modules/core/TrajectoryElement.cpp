#include "TrajectoryElement.h"
#include <modules/core/PhysicsConstants.cpp>

TrajectoryElement::TrajectoryElement(double startTime, vector position, vector lineVel, vector lineAccel)
	:isFinished(false), collidedBall(nullptr), collidedCushion(vector())
{
	this->startTime = startTime;
	this->position = position;
	this->lineVel = lineVel;
	this->lineAccel = lineAccel;
	this->duration = this->calcDuration();
	this->endTime = this->startTime + this->duration;
}

vector TrajectoryElement::calc(double time)
{
	auto t = time - this->startTime;
	auto haflTSquare = 0.5 * t * t;
	return vector(
		this->position.x + this->lineVel.x * t + this->lineAccel.x * haflTSquare,
		this->position.y + this->lineVel.y * t + this->lineAccel.y * haflTSquare,
		this->position.z + this->lineVel.z * t + this->lineAccel.z * haflTSquare
	);
	return vector();
}

double TrajectoryElement::calcX(double time)
{
	auto t = time - this->startTime;
	auto haflTSquare = 0.5 * t * t;
	return this->position.x + this->lineVel.x * t + this->lineAccel.x * haflTSquare;
}

double TrajectoryElement::calcY(double time)
{
	auto t = time - this->startTime;
	auto haflTSquare = 0.5 * t * t;
	return this->position.y + this->lineVel.y * t + this->lineAccel.y * haflTSquare;
}

vector TrajectoryElement::calcVelocity(double time) {
	auto t = time - this->startTime;
	auto v = this->lineVel;
	auto a = this->lineAccel;
	return vector(
		v.x + a.x * t,
		v.y + a.y * t,
		v.z + a.z * t
	);
}

TrajectoryData TrajectoryElement::calcAll(double time)
{
	auto t = time - this->startTime;
	auto a = this->lineAccel;
	auto v = this->lineVel;
	auto p = this->position;

	auto atx = a.x * t;
	auto aty = a.y * t;
	auto atz = a.z * t;

	auto velocity = vector(
		v.x + atx,
		v.y + aty,
		v.z + atz
	);
	auto position = vector(
		p.x + (v.x + atx * 0.5) * t,
		p.y + (v.y + aty * 0.5) * t,
		p.z + (v.z + atz * 0.5) * t
	);
	return TrajectoryData{ velocity, position };
}

double TrajectoryElement::calcDuration()
{
	return 0.0;
}

const double COLLISION_DISTANCE_SQUARE = 4 * PhysicsConstants::BALL_RADIUS_SQUARE;

void TrajectoryElement::checkCollisions(BallBody* cueBall, std::vector<BallBody*> balls)
{
	auto la = this->lineAccel;
	auto lv = this->lineVel;
	auto pos = this->position;

	auto a = 0.25 * vector::dot(la, la);
	auto b = vector::dot(la, lv);
	auto vv = vector::dot(lv, lv);
	auto len = balls.size();

	auto i = 0;
	BallBody* collidedBall = nullptr;
	BallBody* ball;
	vector dp;
	double t;

	// auto c, d, e;
	while (i < len) {
		ball = balls[i++];
		if (1 <= ball->id() && ball->id() <= 15) {
			dp = vector::sub(pos, ball->position());
			// c = vv + vector::dot(dp, la)
			// d = 2 * vector::dot(dp, lv)
			// e = vector::dot(dp, dp) - COLLISION_DISTANCE_SQUARE
			auto res = ExtMath::resolveQuarticEquation(
				a, b,
				vv + vector::dot(dp, la), 2 * vector::dot(dp, lv),
				vector::dot(dp, dp) - COLLISION_DISTANCE_SQUARE
			);

			for (auto j = 0; j < res.size(); j++) {
				if (res[j] >= 0 && res[j] < t) {
					t = res[j];
					collidedBall = ball;
				}
			}
		}
	}

	if (t <= this->duration) {
		this->isFinished = true;
		this->duration = t;
		this->endTime = this->startTime + this->duration;
		this->collidedBall = collidedBall;
		this->collidedCushion.x = this->collidedCushion.y = 0;
	}
}



FlyTrajElem::FlyTrajElem(double startTime, vector position, vector lineVel)
	:TrajectoryElement(startTime, position, lineVel, PhysicsConstants::GRAVITY)
{
	
}


double FlyTrajElem::calcDuration()
{
	return 0.0;
}
