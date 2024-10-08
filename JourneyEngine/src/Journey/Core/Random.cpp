#include "jnypch.h"
#include "Random.h"

namespace jny
{

Random::Random()
{
	m_randomEngine.seed(std::random_device()());
}

float Random::genFloat()
{
	constexpr u32 C_MAX_VAL = std::numeric_limits<u32>::max();

	return static_cast<float>(m_distribution(m_randomEngine)) / static_cast<float>(C_MAX_VAL);
}

}