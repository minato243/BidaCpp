#ifndef __BALL_BODY__
#define __BALL_BODY__
#include "ExtensionMath.hpp"

namespace ps {
	using namespace ExtMath;

	class PhysicsWorld;

	class BallBody {
	public:
		BallBody();

		void setId(int id);

		int id();

		void setWorld(PhysicsWorld * world);

		void fallToPocket();

		void reset();

		void resetShootingState();

		void setPosition(const vector & position);

		void backupPosition();

		void restorePosition();
		
		bool step(double dt);

		void setPureRolling(bool pureRolling);

		bool checkDisabled();

		void updateFlyingVelocity(double dt);

		void updatePureRollingVelocity(double dt);

		void updateNonPureRollingVelocity(double dt);

		void updateSpinAngular(double dt);

		void updateVelocityWithGravity(double dt);

		void updateVelocity(double dt);

		void updateQuaternion(double dt);

		bool isMotionless();

		void bounceFloor(const vector & normalAcceleration);

		void bounceEdge(const vector & normalAcceleration);

		bool isPureRolling();

		void finalizeStep(double dt);

		void bounceSegment(const segment & seg);

		void bounceIntersectSegmentPoint(const vector & p);

		void bounceSegmentDirection(const vector & segmentDirection, const vector & collisionPoint);

		void bounceSegmentDirectionOnFloor(const vector & linearVelocity, const vector & segDir);

		void bounceSegmentDirectionOnSky(const vector & linearVelocity, const vector & rVec);

		double calcPenetrationSegmentTimeBack(const segment & seg);

		double calcPenetrationPointTimeBack(const vector & point);

		void syncPureRollingAngularVelocity();

		bool isDisabled();

		vector & position();

		vector & linearVelocity();

		vector & angularVelocity();

		quaternion & quaternionRotation();

		void setLinearVelocity(const vector & value);

		void setAngularVelocity(const vector & value);

		bool isFallingInPocket();

		void enable();

		void disable();

		bool isInHole();
	private:
		int _id;
		PhysicsWorld * _world;
		vector _linearVelocity;
		vector _angularVelocity;
		vector _position;
		quaternion _quaternion;
		bool _disabled;
		bool _fallingInPocket;
		vector _positionBackup;
		vector _frictionAccel;
		bool _hasFrictionAccel;
		bool _pureRolling;
		// LocNB, TODO: Refactor inhole and totally in hole
		bool _inHole;
		bool _totallyInHole;
	};
}
#endif
