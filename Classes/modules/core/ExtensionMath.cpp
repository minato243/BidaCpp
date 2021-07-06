#include "ExtensionMath.hpp"
#include <algorithm>
#include <cmath>
#include <cfloat>

using namespace ps;

#define MIN_ZERO_THRESHOLD -1e-12
#define MAX_ZERO_THRESHOLD 1e-12
#define PRECISION_TRIGONOMETRY 10e13
#define EXT_MATH_PI 3.14159265358979323846

const double ExtMath::PI = EXT_MATH_PI;
const double ExtMath::TWO_PI = 2 * ExtMath::PI;

bool ExtMath::isZeroNumber(double num) {
    if (num == 0) return true;
    return MIN_ZERO_THRESHOLD < num && num < MAX_ZERO_THRESHOLD;
}

bool ExtMath::isLessThanZeroNumber(double num) {
    return num <= MIN_ZERO_THRESHOLD;
}

bool ExtMath::isGreatThanZeroNumber(double num) {
    return MAX_ZERO_THRESHOLD <= num;
}

double ExtMath::acos(double value) {
    return std::acos(std::min(1.0, std::max(-1.0, value)));
}

double ExtMath::asin(double value) {
    return std::asin(std::min(1.0, std::max(-1.0, value)));
}

double ExtMath::cos(double value) {
    double ret = std::cos(value);
    return std::round(ret * PRECISION_TRIGONOMETRY) / PRECISION_TRIGONOMETRY;
}

double ExtMath::sin(double value) {
    double ret = std::sin(value);
    return std::round(ret * PRECISION_TRIGONOMETRY) / PRECISION_TRIGONOMETRY;
}

double ExtMath::minifyAngle(double value) {
    if (value < 0) {
        return value + TWO_PI;
    } else if (value > TWO_PI) {
        return value - TWO_PI;
    } else {
        return value;
    }
}

ExtMath::vector::vector() : x(0), y(0), z(0) {}

ExtMath::vector::vector(double x, double y) : x(x), y(y), z(0) {}

ExtMath::vector::vector(double x, double y, double z) : x(x), y(y), z(z) {}

ExtMath::vector::vector(const vector & v) : x(v.x), y(v.y), z(v.z) {}

ExtMath::vector & ExtMath::vector::operator=(const vector & v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

void ExtMath::vector::reverse() {
    x = -x;
    y = -y;
    z = -z;
}

void ExtMath::vector::fixZeroVectorPrecision() {
    if (isZeroNumber(x)) {
        x = 0;
    }
    if (isZeroNumber(y)) {
        y = 0;
    }
    if (isZeroNumber(z)) {
        z = 0;
    }
}

void ExtMath::vector::rotate2D(double angle) {
    double c = ExtMath::cos(angle);
    double s = ExtMath::sin(angle);
    double nx = c * x - s * y;
    double ny = s * x + c * y;
    x = nx;
    y = ny;
}

void ExtMath::vector::applyQuaternion(const quaternion & q) {
    quaternion r = quaternion::reverse(q);
    quaternion vq(0, x, y, z);
    quaternion result = quaternion::multiply(quaternion::multiply(q, vq), r);
    x = result.x;
    y = result.y;
    z = result.z;
}

void ExtMath::vector::assignZero() {
    x = y = z = 0;
}

ExtMath::plane::plane(const vector & p, const vector & n) : p(p), n(n) {}

ExtMath::quaternion::quaternion() : w(1), x(0), y(0), z(0) {}

ExtMath::quaternion::quaternion(double w, double x, double y, double z) : x(x), y(y), z(z), w(w) {}

ExtMath::quaternion::quaternion(const vector & v) {
    double length = vector::length(v);
    if (isZeroNumber(length)) {
        w = 1;
        x = y = z = 0;
    } else {
        double sinHalfThetaPerLength = sin(0.5 * length) / length;
        w = ExtMath::cos(0.5 * length);
        x = v.x * sinHalfThetaPerLength;
        y = v.y * sinHalfThetaPerLength;
        z = v.z * sinHalfThetaPerLength;
    }
}

void ExtMath::quaternion::reverse() {
    double square = w * w + x * x + y * y + z * z;
    w = w / square;
    x = -x / square;
    y = -y / square;
    z = -z / square;
}

ExtMath::segment::segment(const vector & s, const vector & e) : s(s), e(e) {}

bool ExtMath::AABB::checkPointInside(const vector & p) {
    return minX <= p.x && p.x <= maxX
        && minY <= p.y && p.y <= maxY;
}

ExtMath::Polygon2D::Polygon2D() {}

const ExtMath::vector ExtMath::Polygon2D::OUTSIDE_ORIGIN(0, 1e10);

std::vector<ExtMath::vector> & ExtMath::Polygon2D::polygon() {
    return points;
}

bool ExtMath::Polygon2D::checkPointInside(const vector & p) {
    int sides = points.size() - 1;
    // const origin = { x: 0, y: p.y };
    bool result = false;
    for (int i = 0; i < sides; i++) {
        vector & s1 = points[i];
        vector & s2 = points[i + 1];
        // origin.x = Math.min(origin.x, Math.min(s1.x, s2.x) - 1);
        if (intersection(OUTSIDE_ORIGIN, p, s1, s2)) {
            result = !result;
        }
    }

	vector & s1 = points[sides];
	vector & s2 = points[0];
    if (intersection(OUTSIDE_ORIGIN, p, s1, s2)) {
        result = !result;
    }

    return result;
}

bool ExtMath::Polygon2D::ccw(const vector & a, const vector & b, const vector & c) {
    return (c.y - a.y) * (b.x - a.x) >= (b.y - a.y) * (c.x - a.x);
}

bool ExtMath::Polygon2D::intersection(const vector & a, const vector & b, const vector & c, const vector & d) {
    return ccw(a, c, d) != ccw(b, c, d) && ccw(a, b, c) != ccw(a, b, d);
}

// Vector operators
bool ExtMath::vector::equals(const vector & u, const vector & v) {
    return isZeroNumber(u.x - v.x) && isZeroNumber(u.y - v.y) && isZeroNumber(u.z - v.z);
}

bool ExtMath::vector::equalsZero(const vector & u) {
    return isZeroNumber(u.x) && isZeroNumber(u.y) && isZeroNumber(u.z);
}

double ExtMath::vector::dot(const vector & u, const vector & v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

ExtMath::vector ExtMath::vector::cross(const vector & u, const vector & v) {
    return vector(
        u.y * v.z - v.y * u.z,
        v.x * u.z - u.x * v.z,
        u.x * v.y - v.x * u.y
    );
}

double ExtMath::vector::crossZ(const vector & u, const vector & v) {
    return u.x * v.y - v.x * u.y;
}

ExtMath::vector ExtMath::vector::add(const vector & u, const vector & v) {
    return vector(u.x + v.x, u.y + v.y, u.z + v.z);
}

ExtMath::vector ExtMath::vector::add(const vector & u, const vector & v, const vector & w) {
    return vector(u.x + v.x + w.x, u.y + v.y + w.y, u.z + v.z + w.z);
}

ExtMath::vector ExtMath::vector::sub(const vector & u, const vector & v) {
    return vector(u.x - v.x, u.y - v.y, u.z - v.z);
}

ExtMath::vector ExtMath::vector::multiply(double n, const vector & u) {
    return vector(n * u.x, n * u.y, n * u.z);
}

ExtMath::vector ExtMath::vector::reverse(const vector & u) {
    return vector(-u.x, -u.y, -u.z);
}

ExtMath::vector ExtMath::vector::project(const vector & u, const vector & v) {
    return multiply(dot(u, v) / dot(v, v), v);
}

double ExtMath::vector::length(const vector & v) {
    return std::sqrt(dot(v, v));
}

double ExtMath::vector::lengthSquare(const vector & v) {
    return dot(v, v);
}

ExtMath::vector ExtMath::vector::unit(const vector & v) {
    if (equalsZero(v)) {
        return vector(); // Zero vector
    }
    return multiply(1 / length(v), v);
}

ExtMath::vector ExtMath::vector::fromSegment(const segment & seg) {
    return sub(seg.e, seg.s);
}

ExtMath::vector ExtMath::vector::reflect(const vector & dir, const vector & nor) {
    return sub(
        multiply(2, project(dir, nor)),
        dir
    );
}

double ExtMath::vector::distance(const vector & u, const vector & v) {
    return length(sub(u, v));
}

double ExtMath::vector::distanceSquare(const vector & u, const vector & v) {
    return lengthSquare(sub(u, v));
}

ExtMath::vector ExtMath::vector::applyQuaternion(const vector & v, const quaternion & q) {
    quaternion r = quaternion::reverse(q);
    quaternion vq = quaternion(0, v.x, v.y, v.z);
    quaternion result = quaternion::multiply(quaternion::multiply(q, vq), r);
    return vector(result.x, result.y, result.z);
}

ExtMath::vector ps::ExtMath::vector::mix(const vector& u, const vector& v, double range)
{
    return vector(u.x + (v.x - u.x) * range,
        u.y + (v.y - u.y) * range,
        u.z + (v.z - u.z) * range);
    
}

ExtMath::vector ExtMath::plane::projectPoint(const vector & q, const plane & pl) {
    vector qp = vector::sub(pl.p, q);
    double t = vector::dot(qp, pl.n) / vector::dot(pl.n, pl.n);
    return vector(pl.n.x * t + q.x, pl.n.y * t + q.y, pl.n.z * t + q.z);
}

ExtMath::vector ExtMath::plane::projectVector(const vector & v, const plane & pl) {
    vector q = vector::add(pl.p, v);
    vector q_project = projectPoint(q, pl);
    return vector::sub(q_project, pl.p);
}

ExtMath::quaternion ExtMath::quaternion::multiply(const quaternion & p, const quaternion & q) {
    return quaternion(
        p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z,
        p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y,
        p.w * q.y - p.x * q.z + p.y * q.w + p.z * q.x,
        p.w * q.z + p.x * q.y - p.y * q.x + p.z * q.w
    );
}

ExtMath::quaternion ExtMath::quaternion::reverse(const quaternion & q) {
    double square = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
    return quaternion(q.w / square, -q.x / square, -q.y / square, -q.z / square);
}

bool ExtMath::quaternion::equals(const quaternion & p, const quaternion & q) {
    return p.x == q.x && p.y == q.y && p.z == q.z && p.w == q.w;
}

bool ExtMath::segment::projectPoint(const vector & p, const segment & seg, vector & output) {
    vector res = projectPointOnLine(p, seg);
    // Validate z is in seg
    vector z1 = vector::sub(seg.s, res);
    vector z2 = vector::sub(seg.e, res);
    if (vector::dot(z1, z2) <= 0) {
        output = res;
        return true;
    }
    return false;
}

ExtMath::vector ExtMath::segment::projectPointOnLine(const vector & p, const segment & seg) {
    vector segVec = vector::sub(seg.e, seg.s);
    vector v = vector::sub(p, seg.s);
    vector u = vector::project(v, segVec);
    return vector::add(seg.s, u);
}

double ExtMath::segment::distance(const vector & p, const segment & seg) {
    vector out;
    bool valid = segment::projectPoint(p, seg, out);
    if (!valid) return POS_INFINITY;
    return vector::distance(p, out);
}

double ExtMath::segment::distanceSquare(const vector & p, const segment & seg) {
    vector out;
    bool valid = segment::projectPoint(p, seg, out);
    if (!valid) return POS_INFINITY;
    return vector::distanceSquare(p, out);
}

bool ExtMath::segment::isCoPlanar(const segment & s1, const segment & s2) {
    vector v1 = vector::sub(s1.s, s1.e);
    vector v2 = vector::sub(s1.s, s2.s);
    vector v3 = vector::sub(s1.s, s2.e);
    double dot = vector::dot(v1, vector::cross(v2, v3));
    return isZeroNumber(dot);
}

bool ExtMath::segment::differentSide(const segment & seg, const vector & p1, const vector & p2) {
    vector v1 = vector::sub(seg.s, p1);
    vector v2 = vector::sub(seg.e, p1);
    vector v3 = vector::sub(seg.s, p2);
    vector v4 = vector::sub(seg.e, p2);

    double dot = vector::dot(vector::cross(v1, v2), vector::cross(v3, v4));
    return dot <= 0;
}

bool ExtMath::segment::intersect(const segment & s1, const segment & s2) {
    return segment::isCoPlanar(s1, s2)
        && segment::differentSide(s1, s2.s, s2.e)
        && segment::differentSide(s2, s1.s, s1.e);
}
