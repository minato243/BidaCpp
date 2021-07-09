#include "ThiccDrawNode3D.h"
#include "math.h"

USING_NS_CC;
using namespace std;


const double ThiccDrawNode3D::DEFAULT_THICCNESS = 0.3;

ThiccDrawNode3D::ThiccDrawNode3D()
{
}

bool ThiccDrawNode3D::init()
{
	if (!Node::init()) {
		return false;
	}
	return true;
}

Sprite3D* ThiccDrawNode3D::_drawLine(cocos2d::Vec3 start, cocos2d::Vec3 end, cocos2d::Color3B color, double thiccness)
{
    if ((start - end).length() == 0) return nullptr;
    auto cylinder = this->getChildFromPool();
    cylinder->setPosition3D(start);
    cylinder->setScaleX(thiccness / 12);
    cylinder->setScaleY(thiccness / 12);

    // this->addChild(cylinder);

    auto u = end - start;
    auto u3n = u.getNormalized();
    Vec2 u2 = Vec2(u.x, u.z);
    auto u2len = sqrt(u2.x * u2.x + u2.y * u2.y);

    auto pitch = asin(u3n.y) * 180 / std::_Pi;
    double yaw;
    if (u2len == 0) {
        yaw = 0;
    }
    else {
        auto u2n = Vec2(u2.x / u2len, u2.y / u2len);
        yaw = acos(u2n.y) * 180 / std::_Pi;
        if (u2n.x < 0) yaw = -yaw;
    }
    cylinder->setScaleZ(0.04 * u.length());
    cylinder->setRotation3D(Vec3(pitch, -yaw, 180));
    cylinder->setColor(color);
    //
    // auto program = new cc.GLProgram('res/shaders/basic_3d.vert', 'res/shaders/cylinder_thicc_node.frag');
    // program.link();
    // program.updateUniforms();
    // program.retain();
    // auto state = cc.GLProgramState.create(program);
    // state.setUniformVec4('u_line_color', cc.math.vec4(1, 1, 1, 1));
    // state.setUniformFloat('u_opacity', 1);
    // this->state = state;
    return cylinder;
}

cocos2d::Vec3 ThiccDrawNode3D::debugMove()
{
    // array
    //if (before.length) {
    //    const ans = [];
    //    for (let i = 0; i < before.length; i++) {
    //        ans.push_back(Vec3Add(before[i], dist));
    //    }
    //    return ans;
    //}
    return cocos2d::Vec3();
}

void ThiccDrawNode3D::drawLine(cocos2d::Vec3 start, cocos2d::Vec3 end, cocos2d::Color3B color, double thiccness)
{
    Sprite3D* sprite = this->_drawLine(start, end, color, thiccness);
    if (sprite == nullptr) return;
    vector<cocos2d::Sprite3D*> sprites;
    sprites.push_back(sprite);
    this->_sprites.push_back(sprites);
}

void ThiccDrawNode3D::drawLine(cocos2d::Vec3 start, cocos2d::Vec3 end, cocos2d::Color4B color, double thiccness)
{
    Color3B cColor = Color3B(color.r, color.g, color.b);
    this->drawLine(start, end, cColor, thiccness);
}

void ThiccDrawNode3D::drawPoly(std::vector<cocos2d::Vec3> points, cocos2d::Color3B color, double thiccness)
{
    vector<Sprite3D*> sprites;
    for (auto i = 0; i < points.size() - 1; i++) {
        auto sprite = this->_drawLine(points[i], points[i + 1], color, thiccness);
        if (sprite == nullptr) continue;
        sprites.push_back(sprite);
    }
    //this->lines->push_back(points);
    this->_sprites.push_back(sprites);
}

void ThiccDrawNode3D::drawCircle(cocos2d::Vec3 center, double radius, cocos2d::Vec3 normal, int segments, cocos2d::Color3B color)
{
    auto length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    auto zAxis = Vec3(normal.x / length, normal.y / length, normal.z / length);
    auto xAxis = Vec3(zAxis.z, 0, -zAxis.x);
    if (zAxis.x == 0 && zAxis.z == 0) {
        xAxis = Vec3(1, 0, 0);
    }
    xAxis.normalize();
    Vec3 yAxis;
    Vec3::cross(xAxis, zAxis, &yAxis);
    vector<Vec3> points;
    for (auto i = 0; i < segments; i++) {
        auto cos = std::cos(2 * i * M_PI / segments);
        auto sin = std::sin(2 * i * M_PI / segments);
        auto point = Vec3(center.x + radius * cos * xAxis.x + radius * sin * yAxis.x,
            center.y + radius * cos * xAxis.y + radius * sin * yAxis.y,
            center.z + radius * cos * xAxis.z + radius * sin * yAxis.z);
        points.push_back(point);
    }
    vector<Sprite3D*> sprites;
    for (auto i = 0; i < segments; i++) {
        auto sprite = this->_drawLine(points[i], points[(i + 1) % segments], color);
        if (sprite == nullptr) continue;
        sprites.push_back(sprite);
    }
    //this->lines.push_back({ r: radius, c : center });
    this->_sprites.push_back(sprites);
}

void ThiccDrawNode3D::clear()
{
    this->moveAllChildToPool();
}

void ThiccDrawNode3D::moveAllChildToPool()
{
    for (auto i = 0; i < this->_sprites.size(); i++) {
        auto sprite = this->_sprites[i];
        if (sprite.size() > 0) {
            for (int j = 0; j < sprite.size(); j++) {
                sprite[j]->setVisible(false);
                this->_pool.push_back(sprite[j]);
            }
        }
        
    }
    this->_sprites.clear();
}

cocos2d::Sprite3D* ThiccDrawNode3D::getChildFromPool()
{
    Sprite3D* sprite;
    if (this->_pool.size() > 0) {
        sprite = this->_pool.front();
        sprite->setVisible(true);
        return sprite;
    }

    sprite = Sprite3D::create("res/3d/cylinder/cylinder.c3b");
    this->addChild(sprite);
    //sprite->setGLProgramState(this->state);
    sprite->setCameraMask(this->getCameraMask());
    return sprite;
}
