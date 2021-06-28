#include "PhysicsWorld.hpp"
#include "BallBody.hpp"
#include "PhysicsConstants.hpp"
#include <cmath>
#include <algorithm>
#include "cocos2d.h"

#define MAX_BALL 16
#define DEFAULT_FRAME_TIME 1.0 / 180
#define MAX_CUE_SLOT 4

#define CUSHION_HEAD_ID 2
#define CUSHION_FOOT_ID 0
#define CUSHION_SIDE_1_ID 1
#define CUSHION_SIDE_2_ID 3

#define LIMIT_LOOP_PER_FRAME 15

using namespace ps;

bool PhysicsWorld::_initedStatic = false;
double PhysicsWorld::_width;
double PhysicsWorld::_height;
AABB PhysicsWorld::_limits;
AABB PhysicsWorld::_nonCushionCollisionZone;
std::vector<Cushion> PhysicsWorld::_cushions;
std::vector<Cushion> PhysicsWorld::_mainCushions;
FloorSurface PhysicsWorld::_floorSurface;
Pockets PhysicsWorld::_pockets;

void PhysicsWorld::initStaticValues() {
	if (_initedStatic) return;
	_initedStatic = true;
    updateLimits(PhysicsConstants::TABLE_WIDTH, PhysicsConstants::TABLE_HEIGHT);
    updateStaticSegmentsAndPockets();
}

void PhysicsWorld::updateLimits(double width, double height) {
    _width = width;
    _height = height;

    _limits.minX = -_width / 2;
    _limits.maxX = _width / 2;
    _limits.minY = -_height / 2;
    _limits.maxY = _height / 2;

    _nonCushionCollisionZone.minX = _limits.minX + PhysicsConstants::BALL_RADIUS;
    _nonCushionCollisionZone.maxX = _limits.maxX - PhysicsConstants::BALL_RADIUS;
    _nonCushionCollisionZone.minY = _limits.minY + PhysicsConstants::BALL_RADIUS;
    _nonCushionCollisionZone.maxY = _limits.maxY - PhysicsConstants::BALL_RADIUS;
}

void PhysicsWorld::updateStaticSegmentsAndPockets() {
    double CORNER_POCKET_WIDTH = PhysicsConstants::CORNER_POCKET_WIDTH;
    double SIDE_POCKET_WIDTH = PhysicsConstants::SIDE_POCKET_WIDTH;

    double CORNER_POCKET_RADIUS = PhysicsConstants::CORNER_POCKET_RADIUS;
    double SIDE_POCKET_RADIUS = PhysicsConstants::SIDE_POCKET_RADIUS;

    double CORNER_POCKET_DEPTH = PhysicsConstants::CORNER_POCKET_DEPTH;
    double SIDE_POCKET_DEPTH = PhysicsConstants::SIDE_POCKET_DEPTH;

    double TABLE_EDGE_THICKNESS_V = PhysicsConstants::TABLE_EDGE_THICKNESS_V;
    double TABLE_EDGE_THICKNESS_H = PhysicsConstants::TABLE_EDGE_THICKNESS_H;

    double CORN_SEG_OFFSET = CORNER_POCKET_WIDTH / std::sqrt(2);
    double MIDDLE_SEG_OFFSET = SIDE_POCKET_WIDTH / 2;

    double cushionZ = PhysicsConstants::CUSHION_HEIGHT - PhysicsConstants::BALL_RADIUS;

    ExtMath::AABB & tableEdgeLimits = _floorSurface.tableEdges;

    tableEdgeLimits.minX = _limits.minX - TABLE_EDGE_THICKNESS_H;
    tableEdgeLimits.maxX = _limits.maxX + TABLE_EDGE_THICKNESS_H;
    tableEdgeLimits.minY = _limits.minY - TABLE_EDGE_THICKNESS_V;
    tableEdgeLimits.maxY = _limits.maxY + TABLE_EDGE_THICKNESS_V;

    segment segLeft = segment(
        vector(_limits.minX, _limits.minY + CORN_SEG_OFFSET, cushionZ),
        vector(_limits.minX, _limits.maxY - CORN_SEG_OFFSET, cushionZ)
    );

    segment segTop_1 = segment(
        vector(_limits.minX + CORN_SEG_OFFSET, _limits.maxY, cushionZ),
        vector(-MIDDLE_SEG_OFFSET, _limits.maxY, cushionZ)
    );

    segment segTop_2 = segment(
        vector(MIDDLE_SEG_OFFSET, _limits.maxY, cushionZ),
        vector(_limits.maxX - CORN_SEG_OFFSET, _limits.maxY, cushionZ)
    );

    segment segRight = segment(
        vector(_limits.maxX, _limits.minY + CORN_SEG_OFFSET, cushionZ),
        vector(_limits.maxX, _limits.maxY - CORN_SEG_OFFSET, cushionZ)
    );

    segment segBottom_1 = segment(
        vector(MIDDLE_SEG_OFFSET, _limits.minY, cushionZ),
        vector(_limits.maxX - CORN_SEG_OFFSET, _limits.minY, cushionZ)
    );

    segment segBottom_2 = segment(
        vector(_limits.minX + CORN_SEG_OFFSET, _limits.minY, cushionZ),
        vector(-MIDDLE_SEG_OFFSET, _limits.minY, cushionZ)
    );

    double CornerDepthSqrt2 = CORNER_POCKET_DEPTH / std::sqrt(2);
    
    Pocket & topLeftPocket = _pockets.topLeftPocket;
    topLeftPocket.position = vector(_limits.minX - CornerDepthSqrt2, _limits.maxY + CornerDepthSqrt2);
    topLeftPocket.radius = CORNER_POCKET_RADIUS;

    Pocket & topRightPocket = _pockets.topRightPocket;
    topRightPocket.position = vector(_limits.maxX + CornerDepthSqrt2, _limits.maxY + CornerDepthSqrt2);
    topRightPocket.radius = CORNER_POCKET_RADIUS;

    Pocket & bottomRightPocket = _pockets.bottomRightPocket;
    bottomRightPocket.position = vector(_limits.maxX + CornerDepthSqrt2, _limits.minY - CornerDepthSqrt2);
    bottomRightPocket.radius = CORNER_POCKET_RADIUS;

    Pocket & bottomLeftPocket = _pockets.bottomLeftPocket;
    bottomLeftPocket.position = vector(_limits.minX - CornerDepthSqrt2, _limits.minY - CornerDepthSqrt2);
    bottomLeftPocket.radius = CORNER_POCKET_RADIUS;

    Pocket & topSidePocket = _pockets.topSidePocket;
    topSidePocket.position = vector(0, _limits.maxY + SIDE_POCKET_DEPTH);
    topSidePocket.radius = SIDE_POCKET_RADIUS;

    Pocket & bottomSidePocket = _pockets.bottomSidePocket;
    bottomSidePocket.position = vector(0, _limits.minY - SIDE_POCKET_DEPTH);
    bottomSidePocket.radius = SIDE_POCKET_RADIUS;

    AABB & safeZone = _floorSurface.safeZone;
    safeZone.minX = segTop_1.s.x;
    safeZone.maxX = segTop_2.e.x;
    safeZone.minY = segLeft.s.y;
    safeZone.maxY = segLeft.e.y;

    // Pocket[] cornerPockets = { topLeftPocket, topRightPocket, bottomRightPocket, bottomLeftPocket };
    // Pocket[] sidePockets = { topSidePocket, bottomSidePocket };

    double CornerRadiusSqrt2 = CORNER_POCKET_RADIUS / std::sqrt(2);
    segment segTopLeftCorner_1 = segment(
        segLeft.e,
        vector::add(topLeftPocket.position, vector(-CornerRadiusSqrt2, -CornerRadiusSqrt2, cushionZ))
    );

    segment segTopLeftCorner_2 = segment(
        segTop_1.s,
        vector::add(topLeftPocket.position, vector(CornerRadiusSqrt2, CornerRadiusSqrt2, cushionZ))
    );

    std::vector<vector> & topLeftPocketPolygon = topLeftPocket.warnPolygon.polygon();
    topLeftPocketPolygon.push_back(segTopLeftCorner_1.e);
    topLeftPocketPolygon.push_back(segTopLeftCorner_2.e);
    topLeftPocketPolygon.push_back(segTopLeftCorner_2.s);
    topLeftPocketPolygon.push_back(vector(safeZone.minX, safeZone.maxY));
    topLeftPocketPolygon.push_back(segTopLeftCorner_1.s);

    segment segTopRightCorner_1 = segment(
        segTop_2.e,
        vector::add(topRightPocket.position, vector(-CornerRadiusSqrt2, CornerRadiusSqrt2, cushionZ))
    );

    segment segTopRightCorner_2 = segment(
        segRight.e,
        vector::add(topRightPocket.position, vector(CornerRadiusSqrt2, -CornerRadiusSqrt2, cushionZ))
    );

    std::vector<vector> & topRightPocketPolygon = topRightPocket.warnPolygon.polygon();

    topRightPocketPolygon.push_back(segTopRightCorner_1.e);
    topRightPocketPolygon.push_back(segTopRightCorner_2.e);
    topRightPocketPolygon.push_back(segTopRightCorner_2.s);
    topRightPocketPolygon.push_back(vector(safeZone.maxX, safeZone.maxY));
    topRightPocketPolygon.push_back(segTopRightCorner_1.s);

    segment segBottomRightCorner_1 = segment(
        segRight.s,
        vector::add(bottomRightPocket.position, vector(CornerRadiusSqrt2, CornerRadiusSqrt2, cushionZ))
    );

    segment segBottomRightCorner_2 = segment(
        segBottom_1.e,
        vector::add(bottomRightPocket.position, vector(-CornerRadiusSqrt2, -CornerRadiusSqrt2, cushionZ))
    );

    std::vector<vector> & bottomRightPocketPolygon = bottomRightPocket.warnPolygon.polygon();

    bottomRightPocketPolygon.push_back(segBottomRightCorner_1.e);
    bottomRightPocketPolygon.push_back(segBottomRightCorner_2.e);
    bottomRightPocketPolygon.push_back(segBottomRightCorner_2.s);
    bottomRightPocketPolygon.push_back(vector(safeZone.maxX, safeZone.minY));
    bottomRightPocketPolygon.push_back(segBottomRightCorner_1.s);

    segment segBottomLeftCorner_1 = segment(
        segBottom_2.s,
        vector::add(bottomLeftPocket.position, vector(CornerRadiusSqrt2, -CornerRadiusSqrt2, cushionZ))
    );

    segment segBottomLeftCorner_2 = segment(
        segLeft.s,
        vector::add(bottomLeftPocket.position, vector(-CornerRadiusSqrt2, CornerRadiusSqrt2, cushionZ))
    );

    std::vector<vector> & bottomLeftPocketPolygon = bottomLeftPocket.warnPolygon.polygon();

    bottomLeftPocketPolygon.push_back(segBottomLeftCorner_1.e);
    bottomLeftPocketPolygon.push_back(segBottomLeftCorner_2.e);
    bottomLeftPocketPolygon.push_back(segBottomLeftCorner_2.s);
    bottomLeftPocketPolygon.push_back(vector(safeZone.minX, safeZone.minY));
    bottomLeftPocketPolygon.push_back(segBottomLeftCorner_1.s);

    segment segMidTop_1 = segment(
        vector::add(topSidePocket.position, vector(-SIDE_POCKET_RADIUS, 0, cushionZ)),
        segTop_1.e
    );

    segment segMidTop_2 = segment(
        segTop_2.s,
        vector::add(topSidePocket.position, vector(SIDE_POCKET_RADIUS, 0, cushionZ))
    );

    std::vector<vector> & topSidePocketPolygon = topSidePocket.warnPolygon.polygon();

    topSidePocketPolygon.push_back(segMidTop_1.s);
    topSidePocketPolygon.push_back(segMidTop_1.e);
    topSidePocketPolygon.push_back(segMidTop_2.s);
    topSidePocketPolygon.push_back(segMidTop_2.e);

    segment segMidBottom_1 = segment(
        vector::add(bottomSidePocket.position, vector(SIDE_POCKET_RADIUS, 0, cushionZ)),
        segBottom_1.s
    );

    segment segMidBottom_2 = segment(
        segBottom_2.e,
        vector::add(bottomSidePocket.position, vector(-SIDE_POCKET_RADIUS, 0, cushionZ))
    );

    std::vector<vector> & bottomSidePocketPolygon = bottomSidePocket.warnPolygon.polygon();
    bottomSidePocketPolygon.push_back(segMidBottom_1.s);
    bottomSidePocketPolygon.push_back(segMidBottom_1.e);
    bottomSidePocketPolygon.push_back(segMidBottom_2.s);
    bottomSidePocketPolygon.push_back(segMidBottom_2.e);

    // Left: Head(2), Right: Foot(0), Bottom-Top: Side (1, 3)

    // segment[] mainStaticSegments = {
    //     segLeft, segTop_1, segTop_2, segRight, segBottom_1, segBottom_2
    // };

    // segment[] cornerSegments = {
    //     segTopLeftCorner_1, segTopLeftCorner_2,
    //     segMidTop_1, segMidTop_2,
    //     segTopRightCorner_1, segTopRightCorner_2,
    //     segBottomRightCorner_1, segBottomRightCorner_2,
    //     segMidBottom_1, segMidBottom_2,
    //     segBottomLeftCorner_1, segBottomLeftCorner_2
    // };

    _mainCushions.push_back(Cushion(CUSHION_HEAD_ID, segLeft));
    _mainCushions.push_back(Cushion(CUSHION_SIDE_1_ID, segTop_1));
    _mainCushions.push_back(Cushion(CUSHION_SIDE_1_ID, segTop_2));
    _mainCushions.push_back(Cushion(CUSHION_FOOT_ID, segRight));
    _mainCushions.push_back(Cushion(CUSHION_SIDE_2_ID, segBottom_1));
    _mainCushions.push_back(Cushion(CUSHION_SIDE_2_ID, segBottom_2));

    // _cushions = new ArrayList<>(mainStaticSegments.length + cornerSegments.length);
    for (auto it = _mainCushions.begin(); it != _mainCushions.end(); ++it) {
        _cushions.push_back(*it);
    }
    
    _cushions.push_back(Cushion(CUSHION_HEAD_ID, segTopLeftCorner_1));
    _cushions.push_back(Cushion(CUSHION_SIDE_1_ID, segTopLeftCorner_2));

    _cushions.push_back(Cushion(CUSHION_SIDE_1_ID, segMidTop_1));
    _cushions.push_back(Cushion(CUSHION_SIDE_1_ID, segMidTop_2));

    _cushions.push_back(Cushion(CUSHION_SIDE_1_ID, segTopRightCorner_1));
    _cushions.push_back(Cushion(CUSHION_FOOT_ID, segTopRightCorner_2));

    _cushions.push_back(Cushion(CUSHION_FOOT_ID, segBottomRightCorner_1));
    _cushions.push_back(Cushion(CUSHION_SIDE_2_ID, segBottomRightCorner_2));

    _cushions.push_back(Cushion(CUSHION_SIDE_2_ID, segMidBottom_1));
    _cushions.push_back(Cushion(CUSHION_SIDE_2_ID, segMidBottom_2));

    _cushions.push_back(Cushion(CUSHION_SIDE_2_ID, segBottomLeftCorner_1));
    _cushions.push_back(Cushion(CUSHION_HEAD_ID, segBottomLeftCorner_2));

    // for (int j = 0; j < mainStaticSegments.length; j++) {
    //     _staticSegments.add(mainStaticSegments[j]);
    // }
    // for (int j = 0; j < cornerSegments.length; j++) {
    //     _staticSegments.add(cornerSegments[j]);
    // }

    // _pockets.topLeftPocket = topLeftPocket;
    // _pockets.topRightPocket = topRightPocket;
    // _pockets.bottomRightPocket = bottomRightPocket;
    // _pockets.bottomLeftPocket = bottomLeftPocket;
    // _pockets.topSidePocket = topSidePocket;
    // _pockets.bottomSidePocket = bottomSidePocket;

    topRightPocket.id = 0;
    topSidePocket.id = 1;
    topLeftPocket.id = 2;
    bottomLeftPocket.id = 3;
    bottomSidePocket.id = 4;
    bottomRightPocket.id = 5;

    _floorSurface.limits = &_limits;
    _floorSurface.pockets = &_pockets;

    // _floorSurface.setLimits(safeZone, _limits, _pockets, tableEdgeLimits);
}

FloorSurface * PhysicsWorld::floorSurface() {
    return &_floorSurface;
}

PhysicsWorld::PhysicsWorld(int id) {
	initStaticValues();
    // _ballMapping = new BallBody[MAX_BALL]; // Alias mapping ID => Ball
    _id = id;
    _frameTime = DEFAULT_FRAME_TIME;
    _cueBall = nullptr;
    _lagBall = nullptr;
    _run = false;
    _simulateResult = new PhysicSimulateResult();
    _lastAccess = 0L;
    _timestamp = 0;
    // _ballOverBallHelper = new BallOverBallHelper();
    _break = false;
    _turnLag = false;
    _random = new PhysicsRandom();
    _cues = new PhysicsCue[MAX_CUE_SLOT];
    _cacheMotionLess = new bool[MAX_BALL];
    _stepCount = 0;
	_mode = PHYSICS_SIMULATE_MODE;
	_remainFrameTime = 0;
	_dispatcher = new PhysicsDispatcher(this);
    _evtPoller = new EventPoller(_dispatcher);
    _evtPoller->setEnabled(_mode == PHYSICS_SIMULATE_MODE);
}

// TODO: Checking BallBody Leak Memory
PhysicsWorld::~PhysicsWorld() {
    removeAllBalls();
    delete _simulateResult;
    // delete _ballOverBallHelper;
    delete _random;
    delete[] _cues;
    delete[] _cacheMotionLess;
    delete _evtPoller;
	delete _dispatcher;
}

void PhysicsWorld::reset(int id) {
	_id = id;
	//removeAllBalls();
	_frameTime = DEFAULT_FRAME_TIME;
	_cueBall = nullptr;
	_lagBall = nullptr;
	_run = false;
	_simulateResult->reset();
	_lastAccess = 0L;
	_timestamp = 0;
	//_ballOverBallHelper.reset();
	_break = false;
	_random->reset();
	/*for (int i = 0; i < _cues.length; i++) {
		_cues[i] = nullptr;
	}*/
	_remainFrameTime = 0;
}

int PhysicsWorld::id() {
    return _id;
}

void PhysicsWorld::setMode(int mode) {
	_mode = mode;
    _evtPoller->setEnabled(_mode == PHYSICS_SIMULATE_MODE);
}

void PhysicsWorld::setRandomSeed(double seed) {
    _random->setSeed(seed);
}

long long PhysicsWorld::getRandomSeed() {
   return _random->getSeed();
}

void PhysicsWorld::setLastAccess(long long time) {
    _lastAccess = time;
}

long long PhysicsWorld::lastAccess() {
    return _lastAccess;
}

bool PhysicsWorld::setCueAttributes(int slot, const PhysicsCue::PhysicsCueAttributes & attrs) {
    if (slot < 0 || slot >= MAX_CUE_SLOT) return false;
    _cues[slot].setCueAttributes(attrs);
    return true;
}

PhysicsCue * PhysicsWorld::getCue(int slot) {
    if (slot < 0 || slot >= MAX_CUE_SLOT) return nullptr;
    return &_cues[slot];
}

BallBody * PhysicsWorld::getBall(int ballId) {
    if (ballId == 0) {
        return _cueBall; // Better performance
    }
    BallBody * ball;
    for (auto it = _balls.begin(); it != _balls.end(); ++it) {
        ball = *it;
        if (ball->id() == ballId) {
            return ball;
        }
    }
    return nullptr;
}

BallBody * PhysicsWorld::addBall(int ballId, const vector & position) {
    BallBody * ball = new BallBody();
    ball->setId(ballId);
    ball->setPosition(position);
    ball->setWorld(this);
    if (ballId == PhysicsConstants::CUE_BALL_ID) {
        _cueBall = ball;
    }
    _balls.push_back(ball);
    return ball;
}

void PhysicsWorld::removeAllBalls() {
    for (auto it = _balls.begin(); it != _balls.end(); ++it) {
        delete (*it);
    }
    _balls.clear();
}

void PhysicsWorld::update(double dt) {
	if (_mode != PHYSICS_SIMULATE_MODE) return;
	int loop = 0;

    _evtPoller->releaseEvents(); // Auto release previous frame's events
	_remainFrameTime += dt;
	
	while (_run && _remainFrameTime >= _frameTime) {
		++loop;
		step(_frameTime);
		_remainFrameTime -= _frameTime;
		if (loop > LIMIT_LOOP_PER_FRAME) {
			_remainFrameTime = 0;
			break;
		}
	}
}

size_t PhysicsWorld::getBallSize() {
	return _balls.size();
}

void PhysicsWorld::updateSimulateStepTime(const vector & initialVelocity) {
    _frameTime = DEFAULT_FRAME_TIME;
}

void PhysicsWorld::step(double frameTime) {
    int i = 0;
    bool motionLess;

    BallBody * ball;
    for (auto it = _balls.begin(); it != _balls.end(); ++it) {
        ball = *it;
        ball->backupPosition();
        motionLess = !ball->step(frameTime);
        if (i < MAX_BALL) {
            _cacheMotionLess[i++] = motionLess;
        }
    }
    
    // TODO: Reduce vector creation
    std::list<CollisionCushion> cushionCollisions;
    std::list<CollisionBall> ballCollisions;
    std::list<CollisionFloor> floorCollisions;
    std::list<CollisionFloor> edgeCollisions;

    checkCollisionsWithCushions(&cushionCollisions, _cacheMotionLess);
    checkBallCollisions(&ballCollisions, _cacheMotionLess);
    checkCollisionsWithFloor(&floorCollisions, _cacheMotionLess);
    checkCollisionsWithEdge(&edgeCollisions, _cacheMotionLess);
    
    size_t totalCollisions = cushionCollisions.size() + ballCollisions.size() +
            floorCollisions.size() + edgeCollisions.size();
    
    double dt = frameTime;

    if (totalCollisions > 0) {
        double minTime = 0;
        double n_frametimeLimit = -2 * frameTime;

        for (auto it = cushionCollisions.begin(); it != cushionCollisions.end(); ++it) {
            if ((*it).time >= n_frametimeLimit) {
                minTime = std::min(minTime, (*it).time);
            }
        }
        
        for (auto it = ballCollisions.begin(); it != ballCollisions.end(); ++it) {
            if ((*it).time >= n_frametimeLimit) {
                minTime = std::min(minTime, (*it).time);
            }
        }

        for (auto it = floorCollisions.begin(); it != floorCollisions.end(); ++it) {
            if ((*it).time >= n_frametimeLimit) {
                minTime = std::min(minTime, (*it).time);
            }
        }

        for (auto it = edgeCollisions.begin(); it != edgeCollisions.end(); ++it) {
            if ((*it).time >= n_frametimeLimit) {
                minTime = std::min(minTime, (*it).time);
            }
        }
        
        if (minTime < 0) {
            dt = frameTime + minTime;
            _remainFrameTime -= minTime;
            for (auto it = _balls.begin(); it != _balls.end(); ++it) {
                ball = *it;
                ball->restorePosition();
                ball->step(dt);
            }

        }
        _timestamp += dt;

        for (auto it = cushionCollisions.begin(); it != cushionCollisions.end(); ++it) {
            CollisionCushion & collision = *it;

            if (collision.time == minTime) {
                if (collision.collidePoint) {
                    collision.ball->bounceIntersectSegmentPoint(collision.point);
                } else {
                    collision.ball->bounceSegment(collision.cushion->seg);
                }
				_dispatcher->dispatchCushionCollision(_timestamp, collision.ball, collision.cushion);
                _simulateResult->cushionCollisions.push_back(
                    PhysicSimulateResult::BallCushionPair(_timestamp, collision.ball->id(), collision.cushion->id)
                );
            }
        }

        for (auto it = ballCollisions.begin(); it != ballCollisions.end(); ++it) {
            CollisionBall & collision = *it;

            if (collision.time == minTime) {
                resolveBallsCollision(collision.ball_1, collision.ball_2);
				_dispatcher->dispatchBallCollision(_timestamp, collision.ball_1, collision.ball_2);
                _simulateResult->ballCollisionIdPairs.push_back(
                    PhysicSimulateResult::BallIDPair(_timestamp, collision.ball_1->id(), collision.ball_2->id()));
            }
        }

        for (auto it = floorCollisions.begin(); it != floorCollisions.end(); ++it) {
            CollisionFloor & collision = *it;

            if (collision.time == minTime) {
                // if (collision.normal.x == 0 && collision.normal.y == 0) {
                    collision.ball->bounceFloor(collision.normal);
					_dispatcher->dispatchFloorCollision(_timestamp, collision.ball);
                    _simulateResult->floorCollisionIds.push_back(collision.ball->id());
                // }
            }
        }
        
        for (auto it = edgeCollisions.begin(); it != edgeCollisions.end(); ++it) {
            CollisionFloor & collision = *it;
            if (collision.time == minTime) {
                collision.ball->bounceEdge(collision.normal);
				_dispatcher->dispatchEdgeCollision(_timestamp, collision.ball);
			}
        }
	} else {
		_timestamp += dt;
	}

    bool allBallMotionLess = true;
    for (auto it = _balls.begin(); it != _balls.end(); ++it) {
        BallBody * ball = *it;
        if (!ball->isMotionless()) {
            ball->updateVelocity(dt);
            ball->updateQuaternion(dt);
            ball->finalizeStep(dt);
            allBallMotionLess = false;
        }
    }

    _stepCount++;
    if (allBallMotionLess) {
        _run = false;
		onWorldPaused();
    } else {
        // checkBallOverBalls(ballCollisions);
    }
}

inline bool PhysicsWorld::checkCacheMotionLess(bool * cacheMotionLess, int index) {
    if (index >= MAX_BALL) {
        return false;
    }
    return cacheMotionLess[index];
}

// void PhysicsWorld::checkBallOverBalls(const std::vector<CollisionBall> & ballCollisions) {
    // TODO: Adding check Ball Over Balls
// }

void PhysicsWorld::checkCollisionsWithCushions(std::list<CollisionCushion> * collisions, bool * cacheMotionLess) {
    int i = 0;
    for (auto it = _balls.begin(); it != _balls.end(); ++it) {
        if (checkCacheMotionLess(cacheMotionLess, i++)) continue;

        BallBody * ball = *it;
        if (ball->isDisabled() || ball->isFallingInPocket()) continue;

        if (!isPosibleToBeColliding(ball)) continue;

        // boolean foundCollision = false;
        vector & position = ball->position();
        
        for (auto itCus = _mainCushions.begin(); itCus != _mainCushions.end(); itCus++) {
            Cushion * cushion = &(*itCus);
            segment & seg = cushion->seg;
            if (ExtMath::isLessThanZeroNumber(
                vector::distanceSquare(position, seg.s) - PhysicsConstants::BALL_RADIUS_SQUARE
            )) {
                CollisionCushion collision;
                collision.ball = ball;
                collision.point = seg.s;
                collision.cushion = cushion;
                collision.time = ball->calcPenetrationPointTimeBack(seg.s);
                collision.collidePoint = true;
                collisions->push_back(collision);
                break;
            }  else if (ExtMath::isLessThanZeroNumber(
                vector::distanceSquare(position, seg.e) - PhysicsConstants::BALL_RADIUS_SQUARE
            )) {
                CollisionCushion collision;
                collision.ball = ball;
                collision.point = seg.e;
                collision.cushion = cushion;
                collision.time = ball->calcPenetrationPointTimeBack(seg.e);
                collision.collidePoint = true;
                collisions->push_back(collision);
                break;
            }
        }
        
        // if (foundCollision) continue; // If Found collision with cushion, do not check with point

        for (auto itCus = _cushions.begin(); itCus != _cushions.end(); itCus++) {
            Cushion * cushion = &(*itCus);
            segment & seg = cushion->seg;
            double distanceSquare = segment::distanceSquare(ball->position(), seg);
            if (ExtMath::isLessThanZeroNumber(
                distanceSquare - PhysicsConstants::BALL_RADIUS_SQUARE
            )) {
                CollisionCushion collision;
                collision.ball = ball;
                collision.cushion = cushion;
                collision.collidePoint = false;
                collision.time = ball->calcPenetrationSegmentTimeBack(seg);
                collisions->push_back(collision);
                // foundCollision = true;
                break;
            }
        }
    }
}

void PhysicsWorld::checkBallCollisions(std::list<CollisionBall> * collisions, bool * cacheMotionLess) {
    // if (!_break) {
    //     balls = _random.shuffle(balls, 16);
    // }

    size_t size = _balls.size();
    for (size_t i = 0; i < size - 1; i++) {
        BallBody * ball_1 = _balls[i];

        if (ball_1->isDisabled() || ball_1->isFallingInPocket()) continue;
        bool motionLess_1 = checkCacheMotionLess(cacheMotionLess, i);

        for (size_t j = i + 1; j < size; j++) {
            if (motionLess_1 && checkCacheMotionLess(cacheMotionLess, j)) continue;
            
            BallBody * ball_2 = _balls[j];
            if (ball_2->isDisabled() || ball_2->isFallingInPocket()) continue;
            
            if (!checkAABBOverlap(ball_1, ball_2)) continue;
            if (!checkCollision(ball_1, ball_2)) continue;
            if (vector::equalsZero(ball_1->linearVelocity()) && vector::equalsZero(ball_2->linearVelocity())) continue;
            CollisionBall collision;
            collision.ball_1 = ball_1;
            collision.ball_2 = ball_2;
            collision.time = calcPenetrationTimeBack(collision.ball_1, collision.ball_2);
            collisions->push_back(collision);
        }
    }
}

void PhysicsWorld::checkCollisionsWithFloor(std::list<CollisionFloor> * collisions, bool * cacheMotionLess) {
    int i = 0;
    for (auto it = _balls.begin(); it != _balls.end(); ++it) {
        if (checkCacheMotionLess(cacheMotionLess, i++)) continue;
        BallBody * ball = *it;

        if (ball->isDisabled() || ball->isFallingInPocket()) continue;
        vector & position = ball->position();
        if (ExtMath::isLessThanZeroNumber(position.z) && position.z > -PhysicsConstants::BALL_RADIUS) {
            vector normal = _floorSurface.normalAcceleration(position);
            if (
                ExtMath::isZeroNumber(normal.x) &&
                ExtMath::isZeroNumber(normal.y) &&
                !ExtMath::isZeroNumber(normal.z)
            ) {
                double time = calcFloorPenetrationTimeBack(ball);
                if (time < 0) {
                    CollisionFloor collision;
                    collision.ball = ball;
                    collision.normal = normal;
                    collision.time = time;
                    collisions->push_back(collision);
                }
            }
        }
    }
}

void PhysicsWorld::checkCollisionsWithEdge(std::list<CollisionFloor> * collisions, bool * cacheMotionLess) {
    int i = 0;
    double DELTA_HEIGHT = PhysicsConstants::CUSHION_HEIGHT - PhysicsConstants::BALL_RADIUS; // TODO: Add to constants
    for (auto it = _balls.begin(); it != _balls.end(); ++it) {
        BallBody * ball = *it;

        if (checkCacheMotionLess(cacheMotionLess, i++)) continue;

        if (ball->isDisabled() || ball->isFallingInPocket()) continue;
        vector & position = ball->position();

        if (!isPosibleToBeColliding(ball)) continue;

        if(!_floorSurface.isOnTableEdges(position)) {
            continue;
        }

        if (ExtMath::isLessThanZeroNumber(position.z - PhysicsConstants::CUSHION_HEIGHT)
            && position.z > DELTA_HEIGHT) {
            vector normal = _floorSurface.normalAcceleration(position);
            if (ExtMath::isZeroNumber(normal.x) &&
                ExtMath::isZeroNumber(normal.y) &&
                !ExtMath::isZeroNumber(normal.z)
            ) {
                double time = calcFloorPenetrationTimeBack(ball);
                if (time < 0) {
                    CollisionFloor collision;
                    collision.ball = ball;
                    collision.normal = normal;
                    collision.time = time;
                    collisions->push_back(collision);
                }
            }
        }
    }
}

bool PhysicsWorld::checkAABBOverlap(BallBody * ball_1, BallBody * ball_2) {
    vector & p1 = ball_1->position();
    vector & p2 = ball_2->position();
    return ExtMath::isGreatThanZeroNumber(p1.x + PhysicsConstants::BALL_DIAMETER - p2.x)
        && ExtMath::isLessThanZeroNumber(p1.x - p2.x - PhysicsConstants::BALL_DIAMETER)

        && ExtMath::isGreatThanZeroNumber(p1.y + PhysicsConstants::BALL_DIAMETER - p2.y)
        && ExtMath::isLessThanZeroNumber(p1.y - p2.y - PhysicsConstants::BALL_DIAMETER)

        && ExtMath::isGreatThanZeroNumber(p1.z + PhysicsConstants::BALL_DIAMETER - p2.z)
        && ExtMath::isLessThanZeroNumber(p1.z - p2.z - PhysicsConstants::BALL_DIAMETER);
}

bool PhysicsWorld::checkAABBOverlapXY(BallBody * ball_1, BallBody * ball_2) {
    vector & p1 = ball_1->position();
    vector & p2 = ball_2->position();
    return ExtMath::isGreatThanZeroNumber(p1.x + PhysicsConstants::BALL_DIAMETER - p2.x)
        && ExtMath::isGreatThanZeroNumber(p2.x + PhysicsConstants::BALL_DIAMETER - p1.x)

        && ExtMath::isGreatThanZeroNumber(p1.y + PhysicsConstants::BALL_DIAMETER - p2.y)
        && ExtMath::isGreatThanZeroNumber(p2.y + PhysicsConstants::BALL_DIAMETER - p1.y);
}

bool PhysicsWorld::checkCollision(BallBody * ball_1, BallBody * ball_2) {
    vector v = vector::sub(ball_2->position(), ball_1->position());
    return ExtMath::isLessThanZeroNumber(
        vector::dot(v, v) - PhysicsConstants::BALL_DIAMETER_SQUARE
    );
}

bool PhysicsWorld::checkCollisionXY(BallBody * ball_1, BallBody * ball_2) {
    vector v = vector::sub(ball_2->position(), ball_1->position());
    v.z = 0;
    return ExtMath::isLessThanZeroNumber(vector::dot(v, v) - PhysicsConstants::BALL_DIAMETER_SQUARE);
}

double PhysicsWorld::randomAngle() {
    double r = _random->rand();
    // const r = Math.random();
    double sign = (r >= 0.5) ? 1 : -1;
    double value = ExtMath::PI * (0.05 + 0.05 * 2 * std::abs(r - 0.5));
    double angle = sign * value;
    // cc.log("Angle: " + angle);
    return angle;

}

void PhysicsWorld::resolveBallsCollision(BallBody * ball_1, BallBody * ball_2) {
        vector VEC_DOWN(0, 0, -1);
        vector & v1 = ball_1->linearVelocity();
        vector & v2 = ball_2->linearVelocity();

        vector r12 = vector::sub(ball_2->position(), ball_1->position());
        if (!_break && ball_1->id() != 0 && ball_2->id() != 0) {
            double angle = randomAngle();
            r12.rotate2D(angle);
        }
        vector r21 = vector::reverse(r12);

        vector v12 = vector::project(v1, r12);
        vector v11 = vector::sub(v1, v12);

        vector v21 = vector::project(v2, r21);
        vector v22 = vector::sub(v2, v21);

        ball_1->setLinearVelocity(vector::add(v11, v21));
        ball_2->setLinearVelocity(vector::add(v12, v22));

        ball_1->setPureRolling(false);
        ball_2->setPureRolling(false);

        if (ExtMath::isZeroNumber(ball_1->position().z) && ExtMath::isZeroNumber(ball_2->position().z)) {
            vector & a1 = ball_1->angularVelocity();
            vector & a2 = ball_2->angularVelocity();

            vector a1r = vector::project(a1, r12);
            vector a1n = vector::sub(a1, a1r);
            double a1z = a1n.z;

            vector a2r = vector::project(a2, r21);
            vector a2n = vector::sub(a2, a2r);
            double a2z = a2n.z;

            a1n.z = 0;
            a2n.z = 0;

            vector vc1n = vector::cross(a1n, VEC_DOWN);
            vector vc2n = vector::cross(a2n, VEC_DOWN);

            if (ExtMath::isLessThanZeroNumber(vector::dot(vc1n, r12))) {
                double a1nLength = vector::length(a1n);
                double a1nDelta = a1nLength - PhysicsConstants::COLLISION_ANGULAR_COEF;
                if (a1nDelta > 0) {
                    a1n = vector::multiply(std::sqrt(a1nDelta / a1nLength), a1n);
                } else {
                    a1n = vector();
                }
                a1n.z = a1z;
                ball_1->setAngularVelocity(vector::add(a1r, a1n));
            }

            if (ExtMath::isLessThanZeroNumber(vector::dot(vc2n, r21))) {
                double a2nLength = vector::length(a2n);
                double a2nDelta = a2nLength - PhysicsConstants::COLLISION_ANGULAR_COEF;
                if (a2nDelta > 0) {
                    a2n = vector::multiply(std::sqrt(a2nDelta / a2nLength), a2n);
                } else {
                    a2n = vector();
                }
                a2n.z = a2z;
                ball_2->setAngularVelocity(vector::add(a2r, a2n));
            }
        }
    }

bool PhysicsWorld::isPosibleToBeColliding(BallBody * ball) {
    vector & p = ball->position();
    return p.x < _nonCushionCollisionZone.minX
        || p.x > _nonCushionCollisionZone.maxX
        || p.y < _nonCushionCollisionZone.minY
        || p.y > _nonCushionCollisionZone.maxY;
}

double PhysicsWorld::calcPenetrationTimeBack(BallBody * ball_1, BallBody * ball_2) {
    vector & p1 = ball_1->position();
    vector & p2 = ball_2->position();

    vector dp = vector::sub(p2, p1);

    double distance = vector::length(dp);
    double penetrationLength = PhysicsConstants::BALL_DIAMETER - distance;

    if (ExtMath::isLessThanZeroNumber(penetrationLength)) return 0;

    vector v1 = ball_1->linearVelocity();
    vector v2 = ball_2->linearVelocity();

    vector dv = vector::sub(v2, v1);
    double dpdv = vector::dot(dp, dv);
    double dvdv = vector::dot(dv, dv);
    double dpdp = vector::dot(dp, dp);
    double delta = dpdv * dpdv - dvdv * (dpdp - PhysicsConstants::BALL_DIAMETER_SQUARE);

    double t = (-dpdv - std::sqrt(delta)) / dvdv;

    return t;
}

double PhysicsWorld::calcFloorPenetrationTimeBack(BallBody * ball) {
    vector & p = ball->position();
    double minZ = _floorSurface.minimumZOfPosition(p);
    if (minZ == NEG_INFINITY) return 0.0;
    double penetration = minZ - p.z;
    double vz = ball->linearVelocity().z;
    double t = std::min(0.0, penetration / vz);
    return t;
}

void PhysicsWorld::onBallFallToHole(BallBody * ball) {
    Pocket * pocket = _floorSurface.findRegionPocket(ball->position());
	_dispatcher->dispatchBallFallen(_timestamp, ball, pocket);
    _simulateResult->fallenBalls.push_back(PhysicSimulateResult::FallenBallPair(_timestamp, ball->id(), pocket->id));
}

void PhysicsWorld::checkOutsideTableBalls() {
    for (auto it = _balls.begin(); it != _balls.end(); ++it) {
        BallBody * ball = *it;
        // if (ball.id() == PhysicsConstants::CUE_BALL_ID) {
        // }
        if (ball->isDisabled() && !ball->isFallingInPocket()) {
            _simulateResult->ousideTableBallIds.push_back(ball->id());
        }
    }
}

void PhysicsWorld::onWorldPaused() {
	if (!_break) {
		_break = true;
	}

	checkOutsideTableBalls();
	_dispatcher->dispatchWorldPaused(_timestamp);
	// _simulateResult->setBallOverBalls(_ballOverBallHelper.result());
}

PhysicSimulateResult * PhysicsWorld::shoot(int ballId, int cueSlot, double force, const vector & direction, const vector & offset) {
    PhysicsCue * cue = getCue(cueSlot);
    BallBody * ball = getBall(ballId);
    return shoot(ball, cue, force, direction, offset);
}

PhysicSimulateResult * PhysicsWorld::shoot(BallBody * ball, PhysicsCue * cue, double force, const vector & direction, const vector & offset) {
    bool valid = (ball != nullptr && cue != nullptr && checkValidShoot(force, direction, offset));
	cocos2d::log("Shoot: %lf, %lf, %lf", ball->position().x, ball->position().y, ball->position().z);
    if (!valid) {
        _simulateResult->valid = valid;
        return _simulateResult;
    }

    for (auto it = _balls.begin(); it != _balls.end(); ++it) {
        (*it)->resetShootingState();
    }

    cue->setOffset(offset);
    cue->setDirection(direction);
    PhysicsCue::ResultVelocities velocities = cue->calcCueBallVelocities(force);
    ball->setLinearVelocity(velocities.linearVelocity);
    ball->setAngularVelocity(velocities.angularVelocity);
    updateSimulateStepTime(velocities.linearVelocity);

    // _ballOverBallHelper->reset();
    _simulateResult->reset();
    _simulateResult->valid = valid;
    _timestamp = 0;
	_remainFrameTime = 0;
    _run = true;

	if (_mode == PHYSICS_SIMULATE_MODE) {
		// Do nothing
		return nullptr;
	}

    while (_run) {
        step(_frameTime);
    }

    // Check out of table balls;

    return _simulateResult;
}

bool PhysicsWorld::checkValidShoot(double force, const vector & direction, const vector & offset) {
    if (force <= 0) return false;
    if (direction.z > 0) return false;
    if (ExtMath::isZeroNumber(vector::lengthSquare(direction))) return false;
    if (ExtMath::isGreatThanZeroNumber(
        vector::lengthSquare(offset) - PhysicsConstants::BALL_RADIUS_SQUARE
    )) return false;
    return true;
}

bool PhysicsWorld::place(int ballId, const vector & position) {
    BallBody * ball = getBall(ballId);
    if (ball == nullptr) return false;

    if (!checkValidBallPosition(ball, position)) return false;

    ball->enable();
    ball->setPosition(position);
    return true;
}

bool PhysicsWorld::checkValidBallPosition(BallBody * ball, const vector & position) {
    for (auto it = _balls.begin(); it != _balls.end(); ++it) {
        BallBody * currentBall = *it;
        if (currentBall == ball || currentBall->isDisabled()) continue;
        double disSqr = vector::distanceSquare(currentBall->position(), position);
        if (disSqr < PhysicsConstants::BALL_RADIUS_SQUARE) return false;
    }
   
    return true;
}

std::vector<BallBody *> * PhysicsWorld::allBalls() {
    return &_balls;
}

Pockets * PhysicsWorld::pockets() {
    return &_pockets;
}

BallBody * PhysicsWorld::cueBall() {
    return _cueBall;
}

PhysicSimulateResult * PhysicsWorld::physicSimulateResult() {
	return _simulateResult;
}

EventPoller * PhysicsWorld::eventPoller() {
    return _evtPoller;
}

bool PhysicsWorld::isRunning() {
    return _run;
}

void PhysicsWorld::setBreak(bool brk) {
	_break = brk;
}
