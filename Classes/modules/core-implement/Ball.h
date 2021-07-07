#ifndef BALL_H_
#define BALL_H_

#include <cocos2d.h>
#include <modules/core/ExtensionMath.hpp>
#include <modules/core/BallBody.hpp>

class Ball : public cocos2d::Node	{

private:
public:
	int _number;
	cocos2d::Sprite3D* _ball;
	cocos2d::Sprite3D* _shadow;
	ps::BallBody* _body;

	double _specular;
	double metal;
	double rough;
	cocos2d::Material* _mat;
	bool _updateRotate;
	bool _extrapolatePosition;
	double _updateDelay;
	ps::ExtMath::vector _oldPos;
	ps::ExtMath::vector _newPos;
	cocos2d::DrawNode* _draw;

	static Ball* createBall();
	static Ball* createBall(int id);
	virtual bool init();
	void initBall();
	void initPhysicsBody();
	void setNumber(int number);
	void initShadow();
	void initHighlight();

	virtual void update(float dt);
	void onBodyChange();
	void setBallPosition(ps::ExtMath::vector position);
	void setBallQuaternion(ps::ExtMath::quaternion quaternion);
	// DEBUG
	void initDebugIndicator();

	ps::BallBody* body();
	CREATE_FUNC(Ball);
};
#endif // !BALL_H_

