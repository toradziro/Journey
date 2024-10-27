#pragma once

namespace jny
{

class DropDownList
{
public:
	inline static u32 C_INVALID_INDEX = std::numeric_limits<u32>::max();

	DropDownList(std::vector<std::string>& items, const std::string& label, u32& selectedIndex);

	bool draw();

private:
	std::vector<std::string>&	m_items;
	std::string					m_label;
	u32&						m_selectedIndex;
};

}