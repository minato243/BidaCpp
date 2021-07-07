#ifndef 	_SPHERICALCAMERA_H_
#define 	_SPHERICALCAMERA_H_


#include <cocos2d.h>
#include <data/DataStruct.h>
#include <data/GlobalVar.h>
#include <modules/core/ExtensionMath.hpp>

class SphericalCamera : public cocos2d::Node {
public:
	static const double SphericalCamera::MIN_R;
	static const double SphericalCamera::MAX_R;
	static const double SphericalCamera::MIN_P;
	static const double SphericalCamera::MAX_P;

	SphericalCamera(CameraProp cameraProp);
	SphericalCamera();
	virtual bool init();
	void updateFoV(float dt);
	void changeFoV(double fovx);
	void updateUnFov(float dt);
	void focusFov();
	void unfocusFov();
	void lookAt(cocos2d::Vec3 position, cocos2d::Vec3 up = cocos2d::Vec3(0, 1, 0));
	void setPosition3D(cocos2d::Vec3 position);
	void dispatchChangePosition(cocos2d::Vec3 position);
	void subscribe(CUSTOM_CALLBACK handler);
	void unsubscribe(CUSTOM_CALLBACK handler);
	int getCameraMask();
	void setPositionSpherical(Rpt rpt);
	cocos2d::Vec3 getPosition3D();
	void takeSnapshot();
	void loadSnapshot();
	void move(double dx = 0, double dy = 0, double dz = 0);
	void follow(cocos2d::Vec3 position);
	void focus(float delta);
	float r();
	void moveSpherical(float deltaPhi, float deltaTheta);
	cocos2d::Vec3 unproject(cocos2d::Vec3 src);
	ps::ExtMath::vector windowLocationToFloorPosition(cocos2d::Vec3 location);
	cocos2d::Vec2 worldPositionToWindowLocation(ps::ExtMath::vector position);
	cocos2d::Vec3 transformVecToCocosVec(ps::ExtMath::vector vec);
	CREATE_FUNC(SphericalCamera);

private:
	CameraProp _cameraProps;
	cocos2d::Camera* _camera;
	std::vector<CUSTOM_CALLBACK> _subscribers;
	double _focusRatio;
	CameraLookAtData _lookAt;
	CameraSnapShot _snapshot;
};
#endif