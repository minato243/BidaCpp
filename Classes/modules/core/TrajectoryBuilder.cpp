#include "TrajectoryBuilder.h"

using namespace ExtMath;
TrajectoryBuilder::TrajectoryBuilder()
{
}
void TrajectoryBuilder::buildBasicTrajectory(PhysicsCue* cue, BallBody* cueBall, std::vector<BallBody*> balls)
{
	auto cueDir = cue->getDirection();
	auto dir = vector::unit(vector(cueDir.x, cueDir.y));

	//const trajectory = new BasicTrajElem(0, cueBall.position, dir);
	//trajectory.checkCollisions(cue, balls);

	//return trajectory;

	// const start = vector.vec(cueBall.position.x, cueBall.position.y, constants.BALL_RADIUS);
	// const end = trajectory.calc(trajectory.endTime);
	// end.z = constants.BALL_RADIUS;

	// return [ start, end ];
}
