#pragma once

#include "Journey/Log/Log.h"

template <typename T>
void printTypeSize()
{
	jny::Log::error("'{}' : '{}'", typeid(T).name(), sizeof(T));
}