#include "PhysicsRandom.hpp"
#include <cmath>

#define RD_MODULUS 2147483647
#define RD_MULTIPLIER 48271
#define RD_INCREMENT 0
#define RD_DEFAULT_SEED 1603872017946L

using namespace ps;

PhysicsRandom::PhysicsRandom() : _seed(RD_DEFAULT_SEED), _initSeed(RD_DEFAULT_SEED) {}

PhysicsRandom::PhysicsRandom(double seed) : _seed(seed), _initSeed(seed) {}

double PhysicsRandom::getSeed() {
    return _initSeed;
}

void PhysicsRandom::setSeed(double seed) {
    _initSeed = seed;
    _seed = seed;
}

double PhysicsRandom::randCore() {
    _seed = ((long long) std::floor(_seed * RD_MULTIPLIER + RD_INCREMENT)) % RD_MODULUS;
    return _seed;
}

double PhysicsRandom::rand() {
    return 1.0 * randCore() / RD_MODULUS;
}

void PhysicsRandom::reset() {
    _seed = _initSeed;
}
