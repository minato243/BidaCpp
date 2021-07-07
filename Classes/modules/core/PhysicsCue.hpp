#ifndef __PHYSICS_CUE__
#define __PHYSICS_CUE__
#include "ExtensionMath.hpp"

namespace ps {
	using namespace ExtMath;

	class PhysicsCue {
	public:
		class ResultVelocities {
		public:
			vector linearVelocity;
			vector angularVelocity;
		};

		class PhysicsCueAttributes {
		public:
			double tip_e;
			double tip_coef;
			double mass_ratio;
		};

	public:
		PhysicsCue();
		
		void initDefaultPhysicsCue();

		void setCueAttributes(const PhysicsCueAttributes & attrs);

		void setDirection(const vector & direction);

		void updateDirection(const vector& direction);
		
		virtual void setCueDirection(const vector& direction) {};
		
		vector getDirection();

		void setOffset(const vector & offset);

		ResultVelocities calcCueBallVelocities(double force);

		vector calcCollisionLocalPoint(const vector & offset, const vector & direction);
	private:
		vector _direction;
		vector _offset;
		PhysicsCueAttributes _attrs;
	};
}

#endif
