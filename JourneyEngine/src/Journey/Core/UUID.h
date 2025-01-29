#pragma once

#include <unordered_map>

namespace jny
{

struct UUID
{
	UUID() { generateUUID(); }
	UUID(u64 uuid) : m_uuid(uuid) {}
	UUID(const UUID&) = default;

	void generateUUID();

	operator u64() const
	{
		return m_uuid;
	}

	bool operator==(const UUID& other) const
	{
		return m_uuid == other.m_uuid;
	}

	u64	m_uuid = 0;
};

}

namespace std
{

template <>
struct hash<jny::UUID>
{
	size_t operator()(const jny::UUID& obj) const
	{
		return std::hash<u64>{}(obj.m_uuid);
	}
};

} //-- jny