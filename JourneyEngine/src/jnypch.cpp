#include "jnypch.h"

void* operator new[](size_t size, const char*, int, unsigned, const char*, int)
{
	return ::malloc(size);
}

void* operator new[](size_t size, size_t, size_t, const char*, int, unsigned, const char*, int)
{
	return ::malloc(size);
}

void operator delete[](void* p, const char*, int, unsigned, const char*, int) noexcept
{
	::free(p);
}

void operator delete[](void* p, size_t, size_t, const char*, int, unsigned, const char*, int) noexcept
{
	::free(p);
}

namespace eastl
{

void AssertionFailure(const char* pExpression)
{
	std::cerr << "Assertion failed in eastl: " << pExpression << std::endl;
	std::abort();
}

}