#ifndef __BALL_OVER_BALL_HELPER__
#define __BALL_OVER_BALL_HELPER__
#include <list>
#include "PhysicSimulateResult.hpp"
#include "PhysicsEntities.hpp"

// TODO: Implement Ball Over Ball Helper
namespace ps {
	class BallOverBallHelper {
	public:
		BallOverBallHelper();
		~BallOverBallHelper();

		void reset();

		// TODO: Check and implement ball over ball
		void submit(std::list<PhysicSimulateResult::BallOverBall> * currentBallOverBallList, std::list<CollisionBall> * ballCollisions);

		std::list<PhysicSimulateResult::BallOverBall> * result();

	private:
		bool ** resetCacheMatrix();

		void swapCacheMatrix(bool ** matrix);

		void buildCollidedMap(std::list<CollisionBall> * ballCollisions, int * collidedMap);
	private:
		double ** _timestampMatrix;
		bool ** _collidedBalls;
		bool ** _prevOverMatrix;
		std::list<PhysicSimulateResult::BallOverBall> * _ballOverBallResult;
		std::list<PhysicSimulateResult::BallOverBall> * _prevBallOverBallList;

		bool ** _cacheMatrix;
	};
}

#endif
