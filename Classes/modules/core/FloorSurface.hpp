#ifndef __FLOOR_SURFACE__
#define __FLOOR_SURFACE__
#include "ExtensionMath.hpp"
#include "PhysicsEntities.hpp"

namespace ps {
	using namespace ExtMath;

	class FloorSurface {
	public:
		AABB safeZone;
		AABB * limits = nullptr;
		Pockets * pockets = nullptr;
		AABB tableEdges;

		bool isOnTableEdges(const vector & p);

		bool isInSafeZone(const vector & p);

		bool isOutSideTable(const vector& p);

		Pocket * findExtractPocket(const vector & p);

		Pocket * findRegionPocket(const vector & p);

		vector normalAcceleration(const vector & p);

		double minimumZOfPosition(const vector & p);
		
	};
}

#endif
