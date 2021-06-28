#include "PhysicsDispatcher.hpp"

using namespace ps;

PhysicsDispatcher::BallCollisionHandler::BallCollisionHandler(int id, const std::function<BallCollisionFunc> & func)
    : id(id), func(func) {}

PhysicsDispatcher::CushionCollisionHandler::CushionCollisionHandler(int id, const std::function<CushionCollisionFunc> & func)
    : id(id), func(func) {}

PhysicsDispatcher::FloorCollisionHandler::FloorCollisionHandler(int id, const std::function<FloorCollisionFunc> & func)
    : id(id), func(func) {}

PhysicsDispatcher::EdgeCollisionHandler::EdgeCollisionHandler(int id, const std::function<EdgeCollisionFunc> & func)
    : id(id), func(func) {}

PhysicsDispatcher::BallFallenHandler::BallFallenHandler(int id, const std::function<BallFallenFunc> & func)
    : id(id), func(func) {}

PhysicsDispatcher::WorldPausedHandler::WorldPausedHandler(int id, const std::function<WorldPausedFunc> & func)
    : id(id), func(func) {}

PhysicsDispatcher::PhysicsDispatcher(PhysicsWorld * world) : _world(world), _handlerID(0) {}

PhysicsDispatcher::~PhysicsDispatcher() {}

void PhysicsDispatcher::dispatchBallCollision(double timestamp, BallBody * ball_1, BallBody * ball_2) {
    for (auto it = _ballCollisionHandlers.begin(); it != _ballCollisionHandlers.end(); ++it) {
        BallCollisionHandler & handler = *it;
        handler.func(timestamp, ball_1, ball_2);
    }
}

void PhysicsDispatcher::dispatchCushionCollision(double timestamp, BallBody * ball, Cushion * cushion) {
    for (auto it = _cushionCollisionHandlers.begin(); it != _cushionCollisionHandlers.end(); ++it) {
        CushionCollisionHandler & handler = *it;
        handler.func(timestamp, ball, cushion);
    }
}

void PhysicsDispatcher::dispatchFloorCollision(double timestamp, BallBody * ball) {
    for (auto it = _floorCollisionHandlers.begin(); it != _floorCollisionHandlers.end(); ++it) {
        FloorCollisionHandler & handler = *it;
        handler.func(timestamp, ball);
    }
}

void PhysicsDispatcher::dispatchEdgeCollision(double timestamp, BallBody * ball) {
    for (auto it = _edgeCollisionHandlers.begin(); it != _edgeCollisionHandlers.end(); ++it) {
        EdgeCollisionHandler & handler = *it;
        handler.func(timestamp, ball);
    }
}

void PhysicsDispatcher::dispatchBallFallen(double timestamp, BallBody * ball, Pocket * pocket) {
    for (auto it = _ballFallenHandlers.begin(); it != _ballFallenHandlers.end(); ++it) {
        BallFallenHandler & handler = *it;
        handler.func(timestamp, ball, pocket);
    }
}

void PhysicsDispatcher::dispatchWorldPaused(double timestamp) {
    for (auto it = _worldPausedHandlers.begin(); it != _worldPausedHandlers.end(); ++it) {
        WorldPausedHandler & handler = *it;
        handler.func(timestamp);
    }
}

int PhysicsDispatcher::subscribeBallCollision(const std::function<BallCollisionFunc> & func) {
    _ballCollisionHandlers.push_back(BallCollisionHandler(_handlerID, func));
    return _handlerID++;
}

int PhysicsDispatcher::subscribeCushionCollision(const std::function<CushionCollisionFunc> & func) {
    _cushionCollisionHandlers.push_back(CushionCollisionHandler(_handlerID, func));
    return _handlerID++;
}

int PhysicsDispatcher::subscribeFloorCollision(const std::function<FloorCollisionFunc> & func) {
    _floorCollisionHandlers.push_back(FloorCollisionHandler(_handlerID, func));
    return _handlerID++;
}

int PhysicsDispatcher::subscribeEdgeCollision(const std::function<EdgeCollisionFunc> & func) {
    _edgeCollisionHandlers.push_back(EdgeCollisionHandler(_handlerID, func));
    return _handlerID++;
}

int PhysicsDispatcher::subscribeBallFallen(const std::function<BallFallenFunc> & func) {
    _ballFallenHandlers.push_back(BallFallenHandler(_handlerID, func));
    return _handlerID++;
}

int PhysicsDispatcher::subscribeWorldPaused(const std::function<WorldPausedFunc> & func) {
    _worldPausedHandlers.push_back(WorldPausedHandler(_handlerID, func));
    return _handlerID++;
}

void PhysicsDispatcher::unsubscribe(int handlerID) {
    for (auto it = _ballCollisionHandlers.begin(); it != _ballCollisionHandlers.end(); ++it) {
        if ((*it).id == handlerID) {
            _ballCollisionHandlers.erase(it);
            return;
        }
    }
    for (auto it = _cushionCollisionHandlers.begin(); it != _cushionCollisionHandlers.end(); ++it) {
        if ((*it).id == handlerID) {
            _cushionCollisionHandlers.erase(it);
            return;
        }
    }
    for (auto it = _floorCollisionHandlers.begin(); it != _floorCollisionHandlers.end(); ++it) {
        if ((*it).id == handlerID) {
            _floorCollisionHandlers.erase(it);
            return;
        }
    }
    for (auto it = _edgeCollisionHandlers.begin(); it != _edgeCollisionHandlers.end(); ++it) {
        if ((*it).id == handlerID) {
            _edgeCollisionHandlers.erase(it);
            return;
        }
    }
    for (auto it = _ballFallenHandlers.begin(); it != _ballFallenHandlers.end(); ++it) {
        if ((*it).id == handlerID) {
            _ballFallenHandlers.erase(it);
            return;
        }
    }
    for (auto it = _worldPausedHandlers.begin(); it != _worldPausedHandlers.end(); ++it) {
        if ((*it).id == handlerID) {
            _worldPausedHandlers.erase(it);
            return;
        }
    }
}
