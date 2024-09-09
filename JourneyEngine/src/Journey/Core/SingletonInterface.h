#pragma once

#include "jnypch.h"

namespace jny
{

#define JNY_SINGLETON_TYPE(x) static std::string type() { return #x; } \
							private: \
							friend SingletonHolder; \

class SingletonHolder;

class ISingleton
{
public:
	ISingleton() = default;
	virtual ~ISingleton() = default;

private:
	friend SingletonHolder;
};

class SingletonHolder
{
public:
	~SingletonHolder()
	{
		std::ranges::for_each(m_singletones, [](auto& s)
			{
				delete s.second;
			});
	}

	template<typename T, typename... Args>
	void add(Args&&... args)
	{
		static_assert(std::is_base_of<ISingleton, T>::value, "To register your singleton inherit it from ISingleton");
		m_singletones[T::type()] = new T(std::forward<Args>(args)...);
	}

	template<typename T>
	T& st()
	{
		static_assert(std::is_base_of<ISingleton, T>::value, "Unknown singleton");
		return static_cast<T&>(*m_singletones[T::type()]);
	}

private:
	std::unordered_map<std::string, ISingleton*> m_singletones;
};

} //-- jny