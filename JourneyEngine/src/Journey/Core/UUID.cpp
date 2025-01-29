#include "jnypch.h"

#include "UUID.h"

namespace
{

u64 generateRandom()
{
	thread_local std::random_device					s_randomDevice = {};
	thread_local std::mt19937_64					s_generator(s_randomDevice());
	thread_local std::uniform_int_distribution<u64>	s_distribution(0, std::numeric_limits<u64>::max());

	return s_distribution(s_generator);
}

} //-- unnamed

namespace jny
{

void UUID::generateUUID()
{
	m_uuid = generateRandom();
}

} //-- jny