#pragma once

#include "SingletonInterface.h"

namespace jny
{

struct Random : ISingleton
{
	JNY_SINGLETON_TYPE(Random)
public:
	Random();
	float genFloat();

private:
	std::mt19937												m_randomEngine;
	std::uniform_int_distribution<std::mt19937::result_type>	m_distribution;
};

}