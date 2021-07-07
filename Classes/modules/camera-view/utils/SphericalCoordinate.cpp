#include "SphericalCoordinate.h"
#include <modules/core/ExtensionMath.hpp>
#include <data/DataStruct.h>

using namespace ps;
USING_NS_CC;


const double DEGREE_PER_RADIAN = 180 / ExtMath::PI;
const double RADIAN_PER_DEGREE = ExtMath::PI / 180;

double SphericalCoord::degreeToRadian (double degree) {
	return degree * RADIAN_PER_DEGREE;
}

double SphericalCoord::radianToDegree (double radian) {
	return radian * DEGREE_PER_RADIAN;
}

Rpt SphericalCoord::descartesToSpherical(double x, double y, double z) {
	auto r = sqrt(x * x + y * y + z * z);
	auto r0 = sqrt(x * x + z * z);
	double p;
	if (z < 0) {
		p = 2 * ExtMath::PI - ExtMath::acos(x / r0);
	} else {
		p = acos(x / r0);
	}
	return Rpt{ r,
		p,
		ExtMath::asin(y / r)
	};

}

Rpt SphericalCoord::descartesToSpherical(cocos2d::Vec3 v)
{
	return descartesToSpherical(v.x, v.y, v.z);
}

cocos2d::Vec3 SphericalCoord::sphericalToDecartes(Rpt rpt)
{
	return sphericalToDecartes(rpt.r, rpt.p, rpt.t);
}

cocos2d::Vec3 SphericalCoord::sphericalToDecartes (double r, double p, double t) {
	auto RcosT = r * cos(t);
	return Vec3(RcosT * cos(p),
		r * sin(t),
		RcosT * sin(p)
	);

}

// cc.log(SphericalCoord.descartesToSpherical(1, 1, 1).p);
// cc.log(SphericalCoord.descartesToSpherical(-1, 1, 1).p);
// cc.log(SphericalCoord.descartesToSpherical(-1, 1, -1).p);
// cc.log(SphericalCoord.descartesToSpherical(1, 1, -1).p);
