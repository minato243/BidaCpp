#ifndef THICCDRAWNODE3D_H_
#define THICCDRAWNODE3D_H_


#include <cocos2d.h>


class ThiccDrawNode3D : public cocos2d::Node
{
public:
	ThiccDrawNode3D();
	virtual bool init();
	cocos2d::Sprite3D* _drawLine(cocos2d::Vec3 start, cocos2d::Vec3 end, cocos2d::Color3B color = cocos2d::Color3B::WHITE, double thiccness = ThiccDrawNode3D::DEFAULT_THICCNESS);
	cocos2d::Vec3 debugMove();
	void drawLine(cocos2d::Vec3 start, cocos2d::Vec3 end, cocos2d::Color3B color = cocos2d::Color3B::WHITE, double thiccness = ThiccDrawNode3D::DEFAULT_THICCNESS);
	void drawLine(cocos2d::Vec3 start, cocos2d::Vec3 end, cocos2d::Color4B color, double thiccness = ThiccDrawNode3D::DEFAULT_THICCNESS);
	void drawPoly(std::vector<cocos2d::Vec3> points, cocos2d::Color3B color = cocos2d::Color3B::WHITE, double thiccness = ThiccDrawNode3D::DEFAULT_THICCNESS);
	void drawCircle(cocos2d::Vec3 center, double radius, cocos2d::Vec3 normal, int segments = 50, cocos2d::Color3B color = cocos2d::Color3B::WHITE);
	void clear();
	void moveAllChildToPool();
	cocos2d::Sprite3D* getChildFromPool();
	CREATE_FUNC(ThiccDrawNode3D);
private:
	std::vector<std::vector<cocos2d::Sprite3D*>> _sprites;
	std::vector<cocos2d::Sprite3D*> _pool;

public:
	static const double DEFAULT_THICCNESS;
};

#endif // !THICCDRAWNODE3D_H_


