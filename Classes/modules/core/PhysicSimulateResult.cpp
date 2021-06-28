#include "PhysicSimulateResult.hpp"

using namespace ps;

PhysicSimulateResult::TimeStampResult::TimeStampResult(double timestamp) : timestamp(timestamp) {}

PhysicSimulateResult::BallIDPair::BallIDPair(double timestamp, int id_1, int id_2)
	: TimeStampResult(timestamp), id_1(id_1), id_2(id_2) {}

PhysicSimulateResult::BallCushionPair::BallCushionPair(double timestamp, int ballId, int cushionId)
	: TimeStampResult(timestamp), ballId(ballId), cushionId(cushionId) {}

PhysicSimulateResult::FallenBallPair::FallenBallPair(double timestamp, int ballId, int pocketId)
	: TimeStampResult(timestamp), ballId(ballId), pocketId(pocketId) {}

PhysicSimulateResult::BallOverBall::BallOverBall(double timestamp, int underBallId, int overBallId)
	: TimeStampResult(timestamp), underBallId(underBallId), overBallId(overBallId) {}

void PhysicSimulateResult::reset() {
    ballCollisionIdPairs.clear();
    cushionCollisions.clear();
    floorCollisionIds.clear();
    fallenBalls.clear();
    ousideTableBallIds.clear();
    ballOverBallPairs.clear();
    valid = false;
}
