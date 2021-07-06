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

	static Ball* createBall();
	virtual bool init();
	void initBall();
	void setNumber(int number);
	void initShadow();
	void initHighlight();

	virtual void update(float dt);
	// DEBUG
	void initDebugIndicator();

	ps::BallBody* body();
	CREATE_FUNC(Ball);
};
#endif // !BALL_H_
