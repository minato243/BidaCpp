#ifndef TRAJECTORYELEMENT_H_
#define TRAJECTORYELEMENT_H_

#include "ExtensionMath.hpp"
#include <modules/core/BallBody.hpp>


using namespace ps;
using namespace ps::ExtMath;

struct TrajectoryData {
	vector velocity;
	vector position;
};

class TrajectoryElement
{
public:
	double startTime;
	ExtMath::vector position;
	vector lineVel;
	vector lineAccel;
	bool isFinished;
	BallBody *collidedBall;
	vector collidedCushion;
	double duration;
	double endTime;

	TrajectoryElement(double startTime, vector position, vector lineVel, vector lineAccel);
	vector calc(double time);
	double calcX(double time);
	double calcY(double time);
	vector calcVelocity(double time);
	TrajectoryData calcAll(double time);
	void checkCollisions(BallBody* cueBall, std::vector<BallBody*> balls);
	virtual double calcDuration();
};

class FlyTrajElem : public TrajectoryElement{
public:
	FlyTrajElem(double startTime, vector position, vector lineVel);
	double calcDuration();
};

#endif // !TRAJECTORYELEMENT_H_

