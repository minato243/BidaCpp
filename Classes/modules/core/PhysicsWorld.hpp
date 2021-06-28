#ifndef __PHYSICS_WORLD__
#define __PHYSICS_WORLD__
#include <vector>
#include <list>
#include "ExtensionMath.hpp"
#include "PhysicsEntities.hpp"
#include "FloorSurface.hpp"
#include "PhysicSimulateResult.hpp"
#include "BallOverBallHelper.hpp"
#include "PhysicsRandom.hpp"
#include "PhysicsCue.hpp"
#include "PhysicsDispatcher.hpp"
#include "EventPoller.hpp"

// For Client
#define PHYSICS_SIMULATE_MODE 0

// For Server
#define PHYSICS_CALCULATE_MODE 1

namespace ps {
    using namespace ExtMath;

    class BallBody;

	class PhysicsWorld {
	private:
		static bool _initedStatic;
		static double _width;
        static double _height;
        static AABB _limits;
        static AABB _nonCushionCollisionZone;
        static std::vector<Cushion> _cushions;
        static std::vector<Cushion> _mainCushions;
        static FloorSurface _floorSurface;
        static Pockets _pockets;
    public:
        static void initStaticValues();

        static void updateLimits(double width, double height);

        static void updateStaticSegmentsAndPockets();
        
        static FloorSurface * floorSurface();
    public:
        PhysicsWorld(int id = 0);
        ~PhysicsWorld();

        void reset(int id);

        int id();

		void setMode(int mode);

        void setRandomSeed(double seed);

        long long getRandomSeed();

        void setLastAccess(long long time);

        long long lastAccess();

        bool setCueAttributes(int slot, const PhysicsCue::PhysicsCueAttributes & attrs);

        PhysicsCue * getCue(int slot);

        BallBody * getBall(int ballId);

        BallBody * addBall(int ballId, const vector & position);

        void removeAllBalls();

		void update(double dt);

		size_t getBallSize();
    private:
        void updateSimulateStepTime(const vector & initialVelocity);

        void step(double frameTime);

        inline bool checkCacheMotionLess(bool * cacheMotionLess, int index);

        // void checkBallOverBalls(const std::vector<CollisionBall> & ballCollisions);

        void checkCollisionsWithCushions(std::list<CollisionCushion> * cushionCollisions, bool * cacheMotionLess);

        void checkBallCollisions(std::list<CollisionBall> * ballCollisions, bool * cacheMotionLess);

        void checkCollisionsWithFloor(std::list<CollisionFloor> * floorCollisions, bool * cacheMotionLess);

        void checkCollisionsWithEdge(std::list<CollisionFloor> * edgeCollisions, bool * cacheMotionLess);

        bool checkAABBOverlap(BallBody * ball_1, BallBody * ball_2);

        bool checkAABBOverlapXY(BallBody * ball_1, BallBody * ball_2);

        bool checkCollision(BallBody * ball_1, BallBody * ball_2);
       
        bool checkCollisionXY(BallBody * ball_1, BallBody * ball_2);

        double randomAngle();

        void resolveBallsCollision(BallBody * ball_1, BallBody * ball_2);

        bool isPosibleToBeColliding(BallBody * ball);

        double calcPenetrationTimeBack(BallBody * ball_1, BallBody * ball_2);

        double calcFloorPenetrationTimeBack(BallBody * ball);

        void checkOutsideTableBalls();

		void onWorldPaused();
        // std::vector<PhysicSimulateResult::BallOverBall> findBallOverBallIds();
    public:
        void onBallFallToHole(BallBody * ball);

        PhysicSimulateResult * shoot(int ballId, int cueSlot, double force, const vector & direction, const vector & offset);

        PhysicSimulateResult * shoot(BallBody * ball, PhysicsCue * cue, double force, const vector & direction, const vector & offset);

        bool checkValidShoot(double force, const vector & direction, const vector & offset);

        bool place(int ballId, const vector & position);

        bool checkValidBallPosition(BallBody * ball, const vector & position);

        std::vector<BallBody *> * allBalls();

        static Pockets * pockets();

        BallBody * cueBall();

		PhysicSimulateResult * physicSimulateResult();

        EventPoller * eventPoller();

        bool isRunning();

		void setBreak(bool brk);
    private:
        int _id;
        std::vector<BallBody *> _balls;
        BallBody * _cueBall;
        BallBody * _lagBall;
        double _frameTime;
        bool _run;
        PhysicSimulateResult * _simulateResult;
        long long _lastAccess;
        double _timestamp;
        // BallOverBallHelper * _ballOverBallHelper;
        bool _break;
        bool _turnLag;
        PhysicsRandom * _random;
        PhysicsCue * _cues;
		int _stepCount;
		int _mode;

        bool * _cacheMotionLess;
		double _remainFrameTime;

		PhysicsDispatcher * _dispatcher;
        EventPoller * _evtPoller;
	};
}

#endif
