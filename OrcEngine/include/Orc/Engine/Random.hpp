#pragma once

#include "Engine/Core.hpp"

#include <random>

namespace orc {

class Random
{
public:
	static void setSeed(uint32_t seed);
	static uint32_t getSeed();

	static int32_t nextInt();
	static int32_t nextInt(int32_t min, int32_t max);

	static float nextFloat();
	static float nextFloat(float min, float max);

	static double nextDouble();
	static double nextDouble(double min, double max);

private:
	static std::mt19937 m_engine;
	static std::uniform_int_distribution<int> m_distributionInt;
	static std::uniform_real_distribution<float> m_distributionFloat;
	static std::uniform_real_distribution<double> m_distributionDouble;
};

}
