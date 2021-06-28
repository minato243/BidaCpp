#include "BallOverBallHelper.hpp"

using namespace ps;

BallOverBallHelper::BallOverBallHelper() {
	_ballOverBallResult = new std::list<PhysicSimulateResult::BallOverBall>();
	_prevBallOverBallList = new std::list<PhysicSimulateResult::BallOverBall>();
}

BallOverBallHelper::~BallOverBallHelper() {
	delete _ballOverBallResult;
	delete _prevBallOverBallList;
}

void BallOverBallHelper::reset() {

}

void BallOverBallHelper::submit(std::list<PhysicSimulateResult::BallOverBall> * currentBallOverBallList, std::list<CollisionBall> * ballCollisions) {

}

std::list<PhysicSimulateResult::BallOverBall> * BallOverBallHelper::result() {
	return _ballOverBallResult;
}

bool ** BallOverBallHelper::resetCacheMatrix() {
	
	return _cacheMatrix;
}

void BallOverBallHelper::swapCacheMatrix(bool ** matrix) {

}

void BallOverBallHelper::buildCollidedMap(std::list<CollisionBall> * ballCollisions, int * collidedMap) {

}
