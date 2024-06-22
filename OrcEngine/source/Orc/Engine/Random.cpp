#include "OrcPch.hpp"

#include "Engine/Random.hpp"

namespace orc {

std::mt19937 Random::m_engine = std::mt19937(std::random_device{}());
std::uniform_int_distribution<int> Random::m_distributionInt = std::uniform_int_distribution<int>(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
std::uniform_real_distribution<float> Random::m_distributionFloat = std::uniform_real_distribution<float>(0.0f, 1.0f);
std::uniform_real_distribution<double> Random::m_distributionDouble = std::uniform_real_distribution<double>(0.0, 1.0);

void Random::setSeed(uint32_t seed)
{
	m_engine.seed(seed);
}

uint32_t Random::getSeed()
{
	return m_engine();
}

int32_t Random::nextInt()
{
	return m_distributionInt(m_engine);
}

int32_t Random::nextInt(int32_t min, int32_t max)
{
	return std::uniform_int_distribution<int>(min, max)(m_engine);
}

float Random::nextFloat()
{
	return m_distributionFloat(m_engine);
}

float Random::nextFloat(float min, float max)
{
	return std::uniform_real_distribution<float>(min, max)(m_engine);
}

double Random::nextDouble()
{
	return m_distributionDouble(m_engine);
}

double Random::nextDouble(double min, double max)
{
	return std::uniform_real_distribution<double>(min, max)(m_engine);
}

}
