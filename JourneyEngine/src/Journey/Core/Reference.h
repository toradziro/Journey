#pragma once

namespace jny
{

class ReferenceCounter
{
public:
	virtual ~ReferenceCounter() = default;

	void incRefCounter()
	{
		++m_counter;
	}

	void decRefCounter()
	{
		--m_counter;
	}

	uint32_t refCount() const
	{
		return m_counter.load();
	}

private:
	std::atomic<uint32_t> m_counter = 0;
};

template <typename T>
class Ref
{
public:
	Ref() : m_instance(nullptr) {}

	Ref(T* instance) : m_instance(instance)
	{
		increment();
	}

	Ref(const Ref<T>& other) : m_instance(other.m_instance)
	{
		increment();
	}

	~Ref()
	{
		decrement();
	}

	Ref& operator=(const Ref<T>& other)
	{
		if (this == &other)
		{
			return *this;
		}

		other.increment();
		decrement();

		m_instance = other.m_instance;
		return *this;
	}

	Ref& operator=(std::nullptr_t)
	{
		decrement();
		m_instance = nullptr;
		return *this;
	}

	operator bool() { return m_instance != nullptr; }
	operator bool() const { return m_instance != nullptr; }

	T* operator->() { return m_instance; }
	const T* operator->() const { return m_instance; }

	T& operator*() { return *m_instance; }
	const T& operator*() const { return *m_instance; }

	T* raw() { return m_instance; }
	const T* raw() const { return m_instance; }

	void reset(T* instance = nullptr)
	{
		decrement();
		m_instance = instance;
	}

	template<typename... Args>
	static Ref<T> create(Args&&... args)
	{
		static_assert(std::is_base_of<ReferenceCounter, T>::value, "Must be a child of ReferenceCounter");
		return Ref<T>(new T(std::forward<Args>(args)...));
	}

private:
	void decrement() const
	{
		if (m_instance)
		{
			m_instance->decRefCounter();
			if (m_instance->refCount() == 0)
			{
				delete m_instance;
			}
		}
	}

	void increment() const
	{
		if (m_instance)
		{
			m_instance->incRefCounter();
		}
	}

	mutable T* m_instance;
};

} //-- jny