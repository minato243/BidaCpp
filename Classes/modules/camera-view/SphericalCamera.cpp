#include "SphericalCamera.h"
#include <modules/core/ExtensionMath.hpp>
#include <cmath>
#include <modules/camera-view/utils/SphericalCoordinate.h>

USING_NS_CC;
using namespace ps;

SphericalCamera::SphericalCamera(CameraProp props) {
	
}

SphericalCamera::SphericalCamera() {
	
}

bool SphericalCamera::init() {
	if (!Node::create()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto width = visibleSize.width;
	auto height = visibleSize.height;

	auto fovx = 35 * ExtMath::PI / 180;
	auto fovy = 2 * atan(
		tan(fovx / 2) / (width / height)
	);

	this->_cameraProps = CameraProp{
			35,
			(width / height),
			1,
			10000
	};
	this->_camera = Camera::createPerspective(
		35,
		this->_cameraProps.ratio,
		this->_cameraProps.nearP,
		this->_cameraProps.farP
	);
	this->_camera->setCameraFlag(CameraFlag::USER1);
	this->addChild(this->_camera);
	this->lookAt(Vec3(0, 0, 0));
	this->_subscribers.clear();
	this->_focusRatio = 1;
	return true;
}

void SphericalCamera::updateFoV(float dt) {
		// this->_cameraProps.ratio += dt;
		this->_cameraProps.fov += 200 * dt;
		// if (this->_cameraProps.ratio >= 2.5) {
		// 	this->unschedule(this->updateFoV);
		// }
		if (this->_cameraProps.fov >= 60) {
			this->_cameraProps.fov = 60;
			this->unschedule(CC_SCHEDULE_SELECTOR(SphericalCamera::updateFoV));
			return;
		}
		
		this->_camera->initPerspective(
			this->_cameraProps.fov,
			this->_cameraProps.ratio,
			this->_cameraProps.nearP,
			this->_cameraProps.farP
		);
		this->focus(-6000 * dt);
	}

void SphericalCamera::changeFoV(double fovx) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto width = visibleSize.width;
	auto height = visibleSize.height;
		auto fovy = 2 * atan(
			tan(fovx / 2) / (width / height)
		);
		this->_camera->initPerspective(
			fovy * 180 / ExtMath::PI,
			this->_cameraProps.ratio,
			this->_cameraProps.nearP,
			this->_cameraProps.farP
		);
	}

void SphericalCamera::updateUnFov(float dt) {
		this->_cameraProps.fov -= 200 * dt;
		// if (this->_cameraProps.ratio >= 2.5) {
		// 	this->unschedule(this->updateFoV);
		// }
		if (this->_cameraProps.fov <= 30) {
			this->_cameraProps.fov = 30;
			this->unschedule(CC_SCHEDULE_SELECTOR(SphericalCamera::updateUnFov));
			return;
		}
		
		this->_camera->initPerspective(
			this->_cameraProps.fov,
			this->_cameraProps.ratio,
			this->_cameraProps.nearP,
			this->_cameraProps.farP
		);
		this->focus(6000 * dt);
	}

void SphericalCamera::focusFov() {
		this->schedule(CC_SCHEDULE_SELECTOR(SphericalCamera::updateFoV));
	}

void SphericalCamera::unfocusFov(){
		this->schedule(CC_SCHEDULE_SELECTOR(SphericalCamera::updateUnFov));
	}

void SphericalCamera::lookAt(cocos2d::Vec3 position, cocos2d::Vec3 up) {
		this->_lookAt = CameraLookAtData{position, up };
		this->_camera->lookAt(position, up);
	}

void SphericalCamera::setPosition3D(cocos2d::Vec3 position) {
		this->_camera->setPosition3D(position);
		this->lookAt(this->_lookAt.position, this->_lookAt.up);
		this->dispatchChangePosition(position);
	}

void SphericalCamera::dispatchChangePosition(cocos2d::Vec3 position) {
		/*for (auto i = 0; i < this->_subscribers.size(); i++) { //todo update later
			this->_subscribers[i].call(null, {
				x: position.x,
				y: position.y,
				z: position.z
			});
		}*/
	}

void SphericalCamera::subscribe(CUSTOM_CALLBACK handler) {
		this->_subscribers.push_back(handler);
	}

void SphericalCamera::unsubscribe(CUSTOM_CALLBACK handler) {
	for (int i = 0; i < this->_subscribers.size(); i++) {
		if (getAddress<void>(this->_subscribers.at(i)) == getAddress<void>(handler)) {
			this->_subscribers.erase(this->_subscribers.begin() +i);
			return;
		}
	}
	
}

int SphericalCamera::getCameraMask() {
		return (int)this->_camera->getCameraFlag();
	}

void SphericalCamera::setPositionSpherical(Rpt rpt) {
		auto position = SphericalCoord::sphericalToDecartes(rpt); 
		this->setPosition3D(position);
	}

cocos2d::Vec3 SphericalCamera::getPosition3D() {
		return this->_camera->getPosition3D();
	}

void SphericalCamera::takeSnapshot(){
	this->_snapshot = CameraSnapShot{
		this->_camera->getPosition3D(),
		this->_lookAt
	};
}

void SphericalCamera::loadSnapshot(){
		this->_lookAt = this->_snapshot.lookAt;
		this->setPosition3D(this->_snapshot.position);
	}

	// TODO: Cache sphericalPosition values

	// move: function(horizontal = 0, vertical = 0, high = 0) {
	// 	// Calculate vertical direction
	// 	auto camPosition = this->_camera->getPosition3D();
	// 	auto lookAtPosition = this->_lookAt.position;

	// 	auto vDirection = Vec3Sub(lookAtPosition, camPosition);
	// 	vDirection.y = 0;

	// 	auto normVDir = Vec3Normalize(vDirection);
	// 	auto normHDir = Vec3(-normVDir.z, 0, normVDir.x);

	// 	auto dir = Vec3(
	// 		normVDir.x * vertical + normHDir.x * horizontal,
	// 		high,
	// 		normVDir.z * vertical + normHDir.z * horizontal
	// 	);

	// 	auto newCamPosition = Vec3Add(camPosition, dir);
	// 	auto newLookAtPosition = Vec3Add(lookAtPosition, dir);

	// 	this->_camera->setPosition3D(newCamPosition);
	// 	this->_lookAt.position = newLookAtPosition;
	// },

void SphericalCamera::move(double dx, double dy, double dz) {
		auto position = this->_camera->getPosition3D();
		this->_camera->setPosition3D(Vec3(
			position.x + dx,
			position.y + dy,
			position.z + dz
		));
		this->_lookAt.position.x += dx;
		this->_lookAt.position.y += dy;
		this->_lookAt.position.z += dz;
		this->dispatchChangePosition(this->_camera->getPosition3D());
	}

void SphericalCamera::follow(cocos2d::Vec3 position) {
		auto cameraPosition = this->_camera->getPosition3D();
		auto lookAtPosition = this->_lookAt.position;
		this->_lookAt.position = position;
		auto delta = position - lookAtPosition;
		auto newPosition = cameraPosition + delta;
		this->setPosition3D(newPosition);
	}

	// TODO: Merge focus to moveSpherical
void SphericalCamera::focus(float delta) {
		auto cameraPosition = this->_camera->getPosition3D();
		auto lookAtPosition = this->_lookAt.position;
		auto backToRootPosition = cameraPosition - lookAtPosition;

		auto sphericalPosition = SphericalCoord::descartesToSpherical(backToRootPosition);
		// sphericalPosition.r = min(SphericalCamera.MAX_R, max(SphericalCamera.MIN_R,
		// 		sphericalPosition.r + sphericalPosition.r * delta / SphericalCamera.MAX_R
		// ));

		sphericalPosition.r = sphericalPosition.r + sphericalPosition.r * delta * 0.001;

		auto newCameraPosition = SphericalCoord::sphericalToDecartes(sphericalPosition);
		auto restoreToLookAtPosition = newCameraPosition + lookAtPosition;
		this->setPosition3D(restoreToLookAtPosition);
	}

float SphericalCamera::r() {
		auto cameraPosition = this->_camera->getPosition3D();
		auto lookAtPosition = this->_lookAt.position;
		auto d =cameraPosition - lookAtPosition;
		return sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
	}

void SphericalCamera::moveSpherical(float deltaPhi, float deltaTheta) {
		auto cameraPosition = this->_camera->getPosition3D();
		auto lookAtPosition = this->_lookAt.position;
		auto backToRootPosition = cameraPosition - lookAtPosition;

		auto sphericalPosition = SphericalCoord::descartesToSpherical(backToRootPosition);
		auto beforeP = sphericalPosition.p;
		sphericalPosition.p += deltaPhi;
		sphericalPosition.t += deltaTheta;
		sphericalPosition.t = std::min(SphericalCamera::MAX_P, std::max(SphericalCamera::MIN_P, sphericalPosition.t));

		auto newCameraPosition = SphericalCoord::sphericalToDecartes(sphericalPosition);
		auto restoreToLookAtPosition = newCameraPosition + lookAtPosition;
		this->setPosition3D(restoreToLookAtPosition);
	}

Vec3 SphericalCamera::unproject(cocos2d::Vec3 src) {
		return this->_camera->unproject(src);
	}

ps::ExtMath::vector SphericalCamera::windowLocationToFloorPosition(cocos2d::Vec3 location) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto nearPoint = this->_camera->getPosition3D();
	auto farPoint = this->_camera->unproject(Vec3(
			location.x, visibleSize.height - location.y, 0));

	auto direction = farPoint - nearPoint;
	direction.normalize();
	auto ray = Ray(nearPoint, direction);
	auto t = -ray._origin.y / ray._direction.y;
	return ps::ExtMath::vector(
		ray._origin.x + t * ray._direction.x,
		-ray._origin.z - t * ray._direction.z,
		0
	);
}

Vec2 SphericalCamera::worldPositionToWindowLocation(ps::ExtMath::vector position) {
		return this->_camera->project(this->transformVecToCocosVec(position));
	}

Vec3 SphericalCamera::transformVecToCocosVec(ps::ExtMath::vector vec) {
		return Vec3(vec.x, vec.z, -vec.y);
	}


const double SphericalCamera::MIN_R = 10;
const double SphericalCamera::MAX_R = 1000;
const double SphericalCamera::MIN_P = 0.1 * ExtMath::PI / 180;
const double SphericalCamera::MAX_P = 89.99 * ExtMath::PI / 180;
