#include "sandboxpch.h"

#include "jnypch.h"

void* operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return ::malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return ::malloc(size);
}

void operator delete[](void* p, const char* name, int flags, unsigned debugFlags, const char* file, int line) noexcept
{
	::free(p);
}

void operator delete[](void* p, size_t alignment, size_t alignmentOffset, const char* name, int flags, unsigned debugFlags, const char* file, int line) noexcept
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