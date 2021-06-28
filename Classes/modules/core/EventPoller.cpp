#include "EventPoller.hpp"

using namespace ps;
using namespace std::placeholders;

EventPoller::EventPoller(PhysicsDispatcher * dispatcher) : _dispatcher(dispatcher), _enabled(false) {
	int id_1 = dispatcher->subscribeBallCollision(std::bind(&EventPoller::handleBallCollision, this, _1, _2, _3));
	int id_2 = dispatcher->subscribeCushionCollision(std::bind(&EventPoller::handleCushionCollision, this, _1, _2, _3));
	int id_3 = dispatcher->subscribeFloorCollision(std::bind(&EventPoller::handleFloorCollision, this, _1, _2));
	int id_4 = dispatcher->subscribeEdgeCollision(std::bind(&EventPoller::handleEdgeCollision, this, _1, _2));
	int id_5 = dispatcher->subscribeBallFallen(std::bind(&EventPoller::handleBallFallen, this, _1, _2, _3));
	int id_6 = dispatcher->subscribeWorldPaused(std::bind(&EventPoller::handleWorldPaused, this, _1));

	_handlerIds.push_back(id_1);
	_handlerIds.push_back(id_2);
	_handlerIds.push_back(id_3);
	_handlerIds.push_back(id_4);
	_handlerIds.push_back(id_5);
	_handlerIds.push_back(id_6);
}

EventPoller::~EventPoller() {
	for (auto it = _handlerIds.begin(); it != _handlerIds.end(); ++it) {
		_dispatcher->unsubscribe(*it);
	}
}

void EventPoller::handleBallCollision(double timestamp, BallBody * ball_1, BallBody * ball_2) {
	if (_enabled) {
		BallCollisionEvent * evt = static_cast<BallCollisionEvent *>(_pool.getEvent(BALL_COLLISION_TYPE));
		evt->timestamp = timestamp;
		evt->ball_1 = ball_1;
		evt->ball_2 = ball_2;
		_events.push_back(evt);
	}
}

void EventPoller::handleCushionCollision(double timestamp, BallBody * ball, Cushion * cushion) {
	if (_enabled) {
		CushionCollisionEvent * evt = static_cast<CushionCollisionEvent *>(_pool.getEvent(CUSHION_COLLISION_TYPE));
		evt->timestamp = timestamp;
		evt->ball = ball;
		evt->cushion = cushion;
		_events.push_back(evt);
	}
}

void EventPoller::handleFloorCollision(double timestamp, BallBody * ball) {
	if (_enabled) {
		FloorCollisionEvent * evt = static_cast<FloorCollisionEvent *>(_pool.getEvent(FLOOR_COLLISION_TYPE));
		evt->timestamp = timestamp;
		evt->ball = ball;
		_events.push_back(evt);
	}
}

void EventPoller::handleEdgeCollision(double timestamp, BallBody * ball) {
	if (_enabled) {
		EdgeCollisionEvent * evt = static_cast<EdgeCollisionEvent *>(_pool.getEvent(EDGE_COLLISION_TYPE));
		evt->timestamp = timestamp;
		evt->ball = ball;
		_events.push_back(evt);
	}
}

void EventPoller::handleBallFallen(double timestamp, BallBody * ball, Pocket * pocket) {
	if (_enabled) {
		BallFallenEvent * evt = static_cast<BallFallenEvent *>(_pool.getEvent(BALL_FALLEN_TYPE));
		evt->timestamp = timestamp;
		evt->ball = ball;
		evt->pocket = pocket;
		_events.push_back(evt);
	}
}

void EventPoller::handleWorldPaused(double timestamp) {
	if (_enabled) {
		BallCollisionEvent * evt = static_cast<BallCollisionEvent *>(_pool.getEvent(WORLD_PAUSED_TYPE));
		evt->timestamp = timestamp;
		_events.push_back(evt);
	}
}

void EventPoller::setEnabled(bool enabled) {
	_enabled = enabled;
}

std::list<EventPoller::Event *> * EventPoller::pollEvents() {
	return &_events;
}

void EventPoller::releaseEvents() {
	for (auto it = _events.begin(); it != _events.end(); ++it) {
		_pool.releaseEvent(*it);
	}
	_events.clear();
}

EventPoller::Event::Event(int eventType) : eventType(eventType), timestamp(0.0) {}

EventPoller::Event::~Event() {}

EventPoller::BallCollisionEvent::BallCollisionEvent() : Event(BALL_COLLISION_TYPE) {}

EventPoller::BallCollisionEvent::~BallCollisionEvent() {}

EventPoller::CushionCollisionEvent::CushionCollisionEvent() : Event(CUSHION_COLLISION_TYPE) {}

EventPoller::CushionCollisionEvent::~CushionCollisionEvent() {}

EventPoller::FloorCollisionEvent::FloorCollisionEvent() : Event(FLOOR_COLLISION_TYPE) {}

EventPoller::FloorCollisionEvent::~FloorCollisionEvent() {}

EventPoller::EdgeCollisionEvent::EdgeCollisionEvent() : Event(EDGE_COLLISION_TYPE) {}

EventPoller::EdgeCollisionEvent::~EdgeCollisionEvent() {}

EventPoller::BallFallenEvent::BallFallenEvent() : Event(BALL_FALLEN_TYPE) {}

EventPoller::BallFallenEvent::~BallFallenEvent() {}

EventPoller::WorldPausedEvent::WorldPausedEvent() : Event(WORLD_PAUSED_TYPE) {}

EventPoller::WorldPausedEvent::~WorldPausedEvent() {}

EventPoller::EventPool::EventPool() {}

EventPoller::EventPool::~EventPool() {
	for (auto it = _events.begin(); it != _events.end(); ++it) {
		delete (*it);
	}
}

EventPoller::Event * EventPoller::EventPool::getEvent(int type) {
	if (type < 0 || type >= TOTAL_EVENT_TYPE) return nullptr;

	std::list<Event *> & pool = _pools[type];
	Event * evt;

	if (pool.size() == 0) {
		switch (type) {
		case BALL_COLLISION_TYPE:
			evt = new BallCollisionEvent();
			break;
		case CUSHION_COLLISION_TYPE:
			evt =  new CushionCollisionEvent();
			break;
		case FLOOR_COLLISION_TYPE:
			evt =  new FloorCollisionEvent();
			break;
		case EDGE_COLLISION_TYPE:
			evt =  new EdgeCollisionEvent();
			break;
		case BALL_FALLEN_TYPE:
			evt =  new BallFallenEvent();
			break;
		case WORLD_PAUSED_TYPE:
			evt =  new WorldPausedEvent();
			break;
		default:
			evt = nullptr;
		}
		if (evt) {
			_events.push_back(evt);
		}
	} else {
		evt = pool.front();
		pool.pop_front();
	}
	return evt;
}

bool EventPoller::EventPool::releaseEvent(Event * evt) {
	int type = evt->eventType;
	if (type < 0 || type >= TOTAL_EVENT_TYPE) {
		return false;
	}
	std::list<Event *> & pool = _pools[type];
	pool.push_back(evt);
	return true;
}
