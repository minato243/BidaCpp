#ifndef __EVENT_POLLER__
#define __EVENT_POLLER__
#include "PhysicsDispatcher.hpp"
#include <vector>
#include <list>

#define BALL_COLLISION_TYPE 0
#define CUSHION_COLLISION_TYPE 1
#define FLOOR_COLLISION_TYPE 2
#define EDGE_COLLISION_TYPE 3
#define BALL_FALLEN_TYPE 4
#define WORLD_PAUSED_TYPE 5
#define TOTAL_EVENT_TYPE 6

// Poll events mechanic
namespace ps {
	class EventPoller {
	public:
		class Event {
		public:
			Event(int eventType);
			virtual ~Event();

			double timestamp;
			int eventType;
		};

		class BallCollisionEvent : public Event {
		public:
			BallCollisionEvent();
			~BallCollisionEvent();

			BallBody * ball_1;
			BallBody * ball_2;
		};

		class CushionCollisionEvent : public Event {
		public:
			CushionCollisionEvent();
			~CushionCollisionEvent();

			BallBody * ball;
			Cushion * cushion;
		};

		class FloorCollisionEvent : public Event {
		public:
			FloorCollisionEvent();
			~FloorCollisionEvent();

			BallBody * ball;
		};

		class EdgeCollisionEvent : public Event {
		public:
			EdgeCollisionEvent();
			~EdgeCollisionEvent();

			BallBody * ball;
		};

		class BallFallenEvent : public Event {
		public:
			BallFallenEvent();
			~BallFallenEvent();

			BallBody * ball;
			Pocket * pocket;
		};

		class WorldPausedEvent : public Event {
		public:
			WorldPausedEvent();
			~WorldPausedEvent();
		};

		class EventPool {
		public:
			EventPool();
			~EventPool();

			Event * getEvent(int type);
			bool releaseEvent(Event * evt);
		private:
			std::list<Event *> _pools[TOTAL_EVENT_TYPE];
			std::list<Event *> _events; // Store all events created
		};
	public:
		EventPoller(PhysicsDispatcher * dispatcher);
		~EventPoller();

		void setEnabled(bool enabled);
		std::list<Event *> * pollEvents();
		void releaseEvents();
	private:
		void handleBallCollision(double timestamp, BallBody * ball_1, BallBody * ball_2);
		void handleCushionCollision(double timestamp, BallBody * ball, Cushion * cushion);
		void handleFloorCollision(double timestamp, BallBody * ball);
		void handleEdgeCollision(double timestamp, BallBody * ball);
		void handleBallFallen(double timestamp, BallBody * ball, Pocket * pocket);
		void handleWorldPaused(double timestamp);

	private:
		std::vector<int> _handlerIds;
		PhysicsDispatcher * _dispatcher;
		std::list<Event *> _events;
		EventPool _pool;
		bool _enabled;
	};
}

#endif
