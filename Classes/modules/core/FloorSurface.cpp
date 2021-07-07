#include "FloorSurface.hpp"
#include "PhysicsConstants.hpp"
#include "ExtensionMath.hpp"
#include <cmath>
#include <cfloat>

using namespace ps;

bool FloorSurface::isOnTableEdges(const vector & p) {
    Pocket * regionPocket = findExtractPocket(p);
    if (regionPocket) {
        return false;
    }
    if (isInSafeZone(p)) return false;

    double r = PhysicsConstants::BALL_RADIUS;
    //        if(offset){
    //            if((tableEdges.minX <= p.x && p.x <= limits->minX && tableEdges.minY <= p.y && p.y <= tableEdges.maxY) ||
    //                    (tableEdges.maxX >= p.x && p.x >= limits->maxX && tableEdges.minY <= p.y && p.y <= tableEdges.maxY) ||
    //                    (tableEdges.minY <= p.y && p.y <= limits->minY && tableEdges.minX <= p.x && p.x <= tableEdges.maxX) ||
    //                    (tableEdges.maxY >= p.y && p.y >= limits->maxY && tableEdges.minX <= p.x && p.x <= tableEdges.maxX)){
    //                return true;
    //            }
    //        }
    //        else{
    if ((tableEdges.minX - r <= p.x && p.x <= limits->minX && tableEdges.minY - r <= p.y && p.y <= tableEdges.maxY + r) ||
        (tableEdges.maxX + r >= p.x && p.x >= limits->maxX && tableEdges.minY - r <= p.y && p.y <= tableEdges.maxY + r) ||
        (tableEdges.minY - r <= p.y && p.y <= limits->minY && tableEdges.minX - r <= p.x && p.x <= tableEdges.maxX + r) ||
        (tableEdges.maxY + r >= p.y && p.y >= limits->maxY && tableEdges.minX - r <= p.x && p.x <= tableEdges.maxX + r)) {
        return true;
    }
    //        }

    return false;
}

bool FloorSurface::isInSafeZone(const vector & p) {
    return (limits->minX <= p.x && p.x <= limits->maxX && safeZone.minY <= p.y && p.y <= safeZone.maxY) ||
        (safeZone.minX <= p.x && p.x <= safeZone.maxX && limits->minY <= p.y && p.y <= limits->maxY);
}

bool ps::FloorSurface::isOutSideTable(const vector& p)
{
    auto edges = this->tableEdges;
    if ((edges.minX > p.x) ||
        (edges.maxX < p.x) ||
        (edges.minY > p.y) ||
        (edges.maxY < p.y)) {
        return true;
    }

    return false;
}

Pocket * FloorSurface::findExtractPocket(const vector & p) {
    if (vector::distance(pockets->bottomLeftPocket.position, p) < pockets->bottomLeftPocket.radius) return &pockets->bottomLeftPocket;
    if (vector::distance(pockets->bottomSidePocket.position, p) < pockets->bottomSidePocket.radius) return &pockets->bottomSidePocket;
    if (vector::distance(pockets->bottomRightPocket.position, p) < pockets->bottomRightPocket.radius) return &pockets->bottomRightPocket;
    if (vector::distance(pockets->topLeftPocket.position, p) < pockets->topLeftPocket.radius) return &pockets->topLeftPocket;
    if (vector::distance(pockets->topSidePocket.position, p) < pockets->topSidePocket.radius) return &pockets->topSidePocket;
    if (vector::distance(pockets->topRightPocket.position, p) < pockets->topRightPocket.radius) return &pockets->topRightPocket;
    return nullptr;
}

Pocket * FloorSurface::findRegionPocket(const vector & p) {
    Pocket * regionPocket = nullptr;
    if (p.x < safeZone.minX) {
        // Left
        if (p.y > safeZone.maxY) {
            // Top-left
            regionPocket = &pockets->topLeftPocket;
        } else if (p.y < safeZone.minY) {
            // Bottom-left
            regionPocket = &pockets->bottomLeftPocket;
        }
    } else if (p.x > safeZone.maxX) {
        // Right
        if (p.y > safeZone.maxY) {
            // Top-right
            regionPocket = &pockets->topRightPocket;
        } else if (p.y < safeZone.minY) {
            // Bottom-right
            regionPocket = &pockets->bottomRightPocket;
        }
    } else {
        // Middle
        if (p.y > limits->maxY) {
            regionPocket = &pockets->topSidePocket;
        } else {
            regionPocket = &pockets->bottomSidePocket;
        }
    }
    return regionPocket;
}

vector FloorSurface::normalAcceleration(const vector & p) {
    bool isOnTableEdge = isOnTableEdges(p);
    if (ExtMath::isGreatThanZeroNumber(p.z)) {
        return vector();
    }

    if (isInSafeZone(p)) {
        return PhysicsConstants::REVERSE_GRAVITY;
    }

    if (isOnTableEdge) {
        return PhysicsConstants::REVERSE_GRAVITY;
    }

    Pocket * regionPocket = findRegionPocket(p);
    if (regionPocket == nullptr || !regionPocket->warnPolygon.checkPointInside(p)) {
        return vector();
    }

    vector delta = vector::sub(p, regionPocket->position);
    delta.z = 0;
    double distance = vector::length(delta);
    if (distance > regionPocket->radius) {
        return PhysicsConstants::REVERSE_GRAVITY;
    } else {
        double d = regionPocket->radius - distance;
        if (d >= PhysicsConstants::BALL_RADIUS) {
            return vector();
        } else {
            double minZ = PhysicsConstants::BALL_RADIUS - std::sqrt(PhysicsConstants::BALL_RADIUS_SQUARE - d * d);
            if (ExtMath::isGreatThanZeroNumber(p.z - minZ)) {
                return vector();
            }
            vector A = vector::add(regionPocket->position, vector::multiply(regionPocket->radius / distance, delta));
            vector C = vector(p.x, p.y, p.z + PhysicsConstants::BALL_RADIUS);
            vector normalDirection = vector::sub(C, A);
            vector reverseNormalProject = vector::project(PhysicsConstants::GRAVITY, normalDirection);
            return vector::reverse(reverseNormalProject);
        }
    }
}

double FloorSurface::minimumZOfPosition(const vector & p) {
    if (isInSafeZone(p)) {
        return 0;
    }

    if (isOnTableEdges(p)) {
        return PhysicsConstants::CUSHION_HEIGHT;
    }

    Pocket * regionPocket = findRegionPocket(p);
    if (regionPocket == nullptr || !regionPocket->warnPolygon.checkPointInside(p)) {
        return NEG_INFINITY;
    }

    vector delta = vector::sub(p, regionPocket->position);
    delta.z = 0;
    double distance = vector::length(delta);
    if (distance > regionPocket->radius) {
        return 0;
    } else {
        double d = regionPocket->radius - distance;
        if (d >= PhysicsConstants::BALL_RADIUS) {
            return NEG_INFINITY;
        } else {
            double minZ = PhysicsConstants::BALL_RADIUS - std::sqrt(PhysicsConstants::BALL_RADIUS_SQUARE - d * d);
            return -minZ;
        }
    }
}
