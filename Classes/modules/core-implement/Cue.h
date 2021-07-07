#ifndef CUE_H_
#define CUE_H_

#include <cocos2d.h>
#include <modules/core/PhysicsCue.hpp>
#include <2d/CCActionInstant.h>
#include <modules/core/utils/LerpUtils.h>

struct CueModelData {
	std::string model;
	std::string tex;
	std::string mrao;
};

class Cue : public cocos2d::Node, public ps::PhysicsCue{
public:
	static const double OFFSET_DISTANCE;
	static const double MOVE_IN_DISTANCE;
	static const double MOVE_OUT_DISTANCE;
	Cue();
	~Cue();

	virtual bool init();
	CREATE_FUNC(Cue);

	void initCue();
	void initShader();
	void setupShader();
	void loadModel(CueModelData);
	void cueMoveByForce(double forcePercent);
	void updateCueMoveIn(double x);
	void updateCueMoveOut(double x);
	void setAllCueModelPosition3D(cocos2d::Vec3 pos);

	void setCuePosition(ps::ExtMath::vector position);
	void updateCueRotate(ps::ExtMath::vector direction);
	void setCueDirection(const ps::ExtMath::vector& direction);
	void doShootAnimation(double force, std::function<void()> &callback);
	void chainMoveTo3D(std::vector<float> internals, std::vector<cocos2d::Vec3> positions, std::function<void()> callback);
	void moveTo3D(float interval, cocos2d::Vec3 position, std::function<void()> callback);

	virtual void onEnter();
	void update(float dt);
	void animateVisible();
	void animateInvisible();
	void setVisible(bool visible);
private:
	cocos2d::Sprite3D* _cue;
	cocos2d::Vec3 _moveToStartPos;
	cocos2d::Vec3 _moveTo;
	double _moveToInterval;
	std::function<void()> _moveToCallback;
	double _moveToCurrentInterval;
	NumberLerp* _moveInLerp;
	NumberLerp* _moveOutLerp;
	VectorLerp* _rotateLerp;

};
#endif // !CUE_H_
