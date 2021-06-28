#ifndef __PHYSICS_DISPATCHER__
#define __PHYSICS_DISPATCHER__
#include <functional>
#include <list>

#define BallCollisionFunc void(double, BallBody *, BallBody *)
#define CushionCollisionFunc void(double, BallBody *, Cushion *)
#define FloorCollisionFunc void(double, BallBody *)
#define EdgeCollisionFunc void(double, BallBody *)
#define BallFallenFunc void(double, BallBody *, Pocket *)
#define WorldPausedFunc void(double)

namespace ps {
	class BallBody;
	class Cushion;
	class Pocket;
	class PhysicsWorld;

	class PhysicsDispatcher {
	private:
		class BallCollisionHandler {
		public:
			BallCollisionHandler(int id, const std::function<BallCollisionFunc> & func);

			std::function<BallCollisionFunc> func;
			int id;
		};

		class CushionCollisionHandler {
		public:
			CushionCollisionHandler(int id, const std::function<CushionCollisionFunc> & func);

			std::function<CushionCollisionFunc> func;
			int id;
		};

		class FloorCollisionHandler {
		public:
			FloorCollisionHandler(int id, const std::function<FloorCollisionFunc> & func);
			
			std::function<FloorCollisionFunc> func;
			int id;
		};

		class EdgeCollisionHandler {
		public:
			EdgeCollisionHandler(int id, const std::function<EdgeCollisionFunc> & func);
			
			std::function<EdgeCollisionFunc> func;
			int id;
		};

		class BallFallenHandler {
		public:
			BallFallenHandler(int id, const std::function<BallFallenFunc> & func);
			
			std::function<BallFallenFunc> func;
			int id;
		};

		class WorldPausedHandler {
		public:
			WorldPausedHandler(int id, const std::function<WorldPausedFunc> & func);
			
			std::function<WorldPausedFunc> func;
			int id;
		};
	public:
		PhysicsDispatcher(PhysicsWorld * world);
		~PhysicsDispatcher();

		void dispatchBallCollision(double timestamp, BallBody * ball_1, BallBody * ball_2);
		void dispatchCushionCollision(double timestamp, BallBody * ball, Cushion * cushion);
		void dispatchFloorCollision(double timestamp, BallBody * ball);
		void dispatchEdgeCollision(double timestamp, BallBody * ball);
		void dispatchBallFallen(double timestamp, BallBody * ball, Pocket * pocket);
		void dispatchWorldPaused(double timestamp);

		int subscribeBallCollision(const std::function<BallCollisionFunc> & func);
		int subscribeCushionCollision(const std::function<CushionCollisionFunc> & func);
		int subscribeFloorCollision(const std::function<FloorCollisionFunc> & func);
		int subscribeEdgeCollision(const std::function<EdgeCollisionFunc> & func);
		int subscribeBallFallen(const std::function<BallFallenFunc> & func);
		int subscribeWorldPaused(const std::function<WorldPausedFunc> & func);

		void unsubscribe(int handlerID);
	private:
		PhysicsWorld * _world;
		std::list<BallCollisionHandler> _ballCollisionHandlers;
		std::list<CushionCollisionHandler> _cushionCollisionHandlers;
		std::list<FloorCollisionHandler> _floorCollisionHandlers;
		std::list<EdgeCollisionHandler> _edgeCollisionHandlers;
		std::list<BallFallenHandler> _ballFallenHandlers;
		std::list<WorldPausedHandler> _worldPausedHandlers;

		int _handlerID;
	};
}

#endif
