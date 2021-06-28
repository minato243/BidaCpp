#ifndef __PHYSICS_ENTITIES__
#define __PHYSICS_ENTITIES__
#include "ExtensionMath.hpp"
#include "BallBody.hpp"

namespace ps {
	using namespace ExtMath;

	class Pocket {
	public:
		Pocket();

		Polygon2D warnPolygon;
		vector position;
		double radius;
		int id;
	};

	class Pockets {
	public:
		Pocket topLeftPocket;
		Pocket topRightPocket;
		Pocket bottomRightPocket;
		Pocket bottomLeftPocket;
		Pocket topSidePocket;
		Pocket bottomSidePocket;
	};

	class Cushion {
	public:
		Cushion(int id, const segment & seg);
		
		segment seg;
		int id;
	};

	class Collision {
	public:
		double time;
	};

	class CollisionCushion : public Collision {
	public:
		BallBody * ball = nullptr;
		Cushion * cushion = nullptr;
		vector point;
		bool collidePoint;
	};

	class CollisionFloor : public Collision {
	public:
		BallBody * ball = nullptr;
		vector normal;
	};

	class CollisionBall : public Collision {
	public:
		BallBody * ball_1 = nullptr;
		BallBody * ball_2 = nullptr;
	};

	class CueAttribute {
	public:
		double tip_e;
		double tip_coef;
		double mass_ratio;
	};
}

#endif
