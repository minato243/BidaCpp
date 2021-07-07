#ifndef _DataStruct_h_
#define _DataStruct_h_

#include <cocos2d.h>
#include<functional>


struct CameraProp {
	double fov;
	double ratio;
	double nearP;
	double farP;
};

struct CameraLookAtData {
	cocos2d::Vec3 position;
	cocos2d::Vec3 up;
};

struct CameraSnapShot {
	cocos2d::Vec3 position;
	CameraLookAtData lookAt;
};

template<typename T>
size_t getAddress(std::function<T()> f) {
	typedef T(fnType)();
	fnType** fnPointer = f.template target<fnType*>();
	return (size_t)*fnPointer;
}

struct Rpt {
	double r;
	double t;
	double p;
	Rpt();
	Rpt(double r, double p, double t);
};
#endif // !_DataStruct_h_
