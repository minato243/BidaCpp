#ifndef TRAJECTORY_BUILDER_H
#define	TRAJECTORY_BUILDER_H

#include <modules/core/PhysicsCue.hpp>
#include <modules/core/BallBody.hpp>

using namespace ps;

class TrajectoryBuilder
{
	TrajectoryBuilder();
	~TrajectoryBuilder();

	void buildBasicTrajectory(PhysicsCue* cue, BallBody *cueBall, std::vector<BallBody*> balls);
	void buildTrajectories(double force, PhysicsCue* cue, BallBody* cueBall, std::vector<BallBody*> balls);

};

#endif // !TRAJECTORY_BUILDER_H


