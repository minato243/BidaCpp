#include "PhysicsCue.hpp"
#include "PhysicsConstants.hpp"
#include <algorithm>
#include <cmath>

using namespace ps;

PhysicsCue::PhysicsCue() : _direction(1, 0, 0), _offset(0, 0) {
	initDefaultPhysicsCue();
}

void PhysicsCue::initDefaultPhysicsCue() {
	_attrs.tip_e = PhysicsConstants::TIP_E;
	_attrs.tip_coef = PhysicsConstants::TIP_COEF;
	_attrs.mass_ratio = PhysicsConstants::BALL_CUE_MASS_RATIO;
}

void PhysicsCue::setCueAttributes(const PhysicsCueAttributes & attrs) {
	_attrs.tip_e = attrs.tip_e;
	_attrs.tip_coef = attrs.tip_coef;
	_attrs.mass_ratio = attrs.mass_ratio;
}

void PhysicsCue::setDirection(const vector & direction) {
	_direction = direction;
}

void PhysicsCue::setOffset(const vector & offset) {
	_offset = offset;
}

PhysicsCue::ResultVelocities PhysicsCue::calcCueBallVelocities(double force) {
    vector localPoint = calcCollisionLocalPoint(_offset, _direction);

    vector vR = vector::reverse(localPoint);

    vector velocity = vector::multiply(force, _direction);
    vector vy = vector::project(velocity, vR);
    vector vx = vector::sub(velocity, vy);
    vector Vy = vy;
    vector Vx = vx;
    // vector Vy = vector::multiply(
    //     _attrs.mass_ratio * (1 + _attrs.tip_e),
    //     vy
    // );

    // vector Vx_dir = vector::unit(vector::sub(velocity, Vy));

    // double value = Math.abs(
    //     vector::dot(vy, velocity) /
    //     (vector::length(vy) * vector::length(velocity))
    // );
    // double angle = ExtensionMath.acos(value);

    // double angleDegree = 180 * angle / Math.PI;
    // double friction = _attrs.tip_coef * angleDegree * angleDegree + angleDegree / 62;
    // vector Vx = vector::multiply(friction * vector::length(Vy), Vx_dir);

    vector W_dir = vector::unit(vector::cross(Vx, Vy));
    vector W = vector::multiply(2.5 * vector::length(Vx) * _attrs.tip_coef / PhysicsConstants::BALL_RADIUS, W_dir);

    vector linearVelocity = vector::add(Vx, Vy);
    if (_direction.z < 0) {
        // Calculate bouncing
        linearVelocity = vector::reflect(
            vector::reverse(linearVelocity),
            vector(0, 0, 1)
        );
        double zVal = linearVelocity.z * (linearVelocity.z - PhysicsConstants::BOUNCE_FLOOR_REFLECT_COEF);
        linearVelocity.z = sqrt(std::max(0.0, zVal));
        // const pitch = mathlib.asin(Math.abs(direction.z));
        // if (pitch > Math.PI / 4) {

        // }
        vector velProj = vector::project(linearVelocity, _direction);
        if (vector::dot(velProj, _direction) < 0) {
            linearVelocity = vector::sub(linearVelocity, velProj);
        }
    }

    ResultVelocities result;
    result.linearVelocity = linearVelocity;
    result.angularVelocity = W;

    return result;
}

vector PhysicsCue::calcCollisionLocalPoint(const vector & offset, const vector & direction) {
    if (ExtMath::isZeroNumber(offset.x) && ExtMath::isZeroNumber(offset.y)) {
        vector point = vector::multiply(-PhysicsConstants::BALL_RADIUS, direction);
        return point;
    }

    vector upWorld = vector(0, 0, 1);
    vector right = vector::unit(vector::cross(direction, upWorld));

    if (vector::equalsZero(right)) {
        right = vector(0, -1, 0);
    }
    vector up = vector::unit(vector::cross(right, direction));
    vector offsetPoint = vector::add(
        vector::multiply(offset.x, right),
        vector::multiply(offset.y, up)
    );

    double uv = vector::dot(offsetPoint, direction);
    double uu = vector::dot(offsetPoint, offsetPoint);
    double vv = vector::dot(direction, direction);

    double delta = uv * uv - vv * (uu - PhysicsConstants::BALL_RADIUS_SQUARE);
    double t = (-uv - std::sqrt(delta)) / vv;

    vector point = vector::add(offsetPoint, vector::multiply(t, direction));
    return point;
}
