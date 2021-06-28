#include "BallGenerator.hpp"
#include "PhysicsConstants.hpp"
#include <cmath>

#define TOTAL_LAYER_DEFAULT 5

using namespace ps;

std::vector<vector> BallGenerator::generateDefault() {
	double InitVertBallDistance = PhysicsConstants::BALL_RADIUS + 0.05;
	double SQRT_3_InitVertBallDistance = std::sqrt(3) * InitVertBallDistance;

	int totalLayer = TOTAL_LAYER_DEFAULT;

	std::vector<vector> result;
	
	for (int layer = 0; layer < totalLayer; layer++) {
		for (int index = layer; index >= -layer; index -= 2) {
			result.push_back(vector(
				layer * SQRT_3_InitVertBallDistance + 60,
				index * InitVertBallDistance
			));
		}
	}

	return result;
}
