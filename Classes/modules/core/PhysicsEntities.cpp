#include "PhysicsEntities.hpp"

using namespace ps;

Pocket::Pocket() : id(0), radius(0.0) {}

Cushion::Cushion(int id, const segment & seg) : id(id), seg(seg) {}
