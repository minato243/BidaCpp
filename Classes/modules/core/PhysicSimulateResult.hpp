#ifndef __PHYSICS_SIMULATE_RESULT__
#define __PHYSICS_SIMULATE_RESULT__
#include <list>

namespace ps {
	class PhysicSimulateResult {
	public:
		class TimeStampResult {
		public:
			double timestamp;
			TimeStampResult(double timestamp);
		};
		
		class BallIDPair : public TimeStampResult {
		public:
			int id_1;
			int id_2;

			BallIDPair(double timestamp, int id_1, int id_2);
		};
		
		class BallCushionPair : public TimeStampResult {
		public:
			int ballId;
			int cushionId;

			BallCushionPair(double timestamp, int ballId, int cushionId);
		};
		
		class FallenBallPair : public TimeStampResult {
		public:
			int ballId;
			int pocketId;

			FallenBallPair(double timestamp, int ballId, int pocketId);
		};

		class BallOverBall : public TimeStampResult {
			int underBallId;
			int overBallId;

			BallOverBall(double timestamp, int underBallId, int overBallId);
		};

	public:
		void reset();

		std::list<BallIDPair> ballCollisionIdPairs;
		std::list<BallCushionPair> cushionCollisions;
		std::list<int> floorCollisionIds;
		std::list<FallenBallPair> fallenBalls;
		std::list<int> ousideTableBallIds;
		std::list<BallOverBall> ballOverBallPairs;

		bool valid = true;
	};
}

#endif
