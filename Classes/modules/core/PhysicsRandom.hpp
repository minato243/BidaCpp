#ifndef __PHYSICS_RANDOM__
#define __PHYSICS_RANDOM__

namespace ps {
	class PhysicsRandom {
	public:
		PhysicsRandom();
		PhysicsRandom(double seed);

		void setSeed(double seed);
		double getSeed();

		double rand();
		void reset();
	private:
		double randCore();
	private:
		double _seed;
		double _initSeed;
	};
}

#endif
