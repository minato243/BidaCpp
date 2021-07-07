#ifndef 	_SphericalCoord_H_
#define 	_SphericalCoord_H_


#include <cocos2d.h>
#include <data/DataStruct.h>

class SphericalCoord {
public:
	SphericalCoord();
	static double degreeToRadian (double degree);
	static double radianToDegree (double radian);
	static Rpt descartesToSpherical(double x, double y, double z);
	static Rpt descartesToSpherical(cocos2d::Vec3 v);
	static cocos2d::Vec3 sphericalToDecartes(double r, double p, double t);
	static cocos2d::Vec3 sphericalToDecartes(Rpt rpt);

private:
};
#endif