#include "jnypch.h"
#include "DropDownList.h"
#include <imgui.h>

namespace jny
{

DropDownList::DropDownList(std::vector<std::string>& items, const std::string& label, u32& selectedIndex)
	: m_items(items), m_label(label), m_selectedIndex(selectedIndex)
{
}

bool DropDownList::draw()
{
	bool isSelected = false;
	std::string name = m_selectedIndex != C_INVALID_INDEX ? m_items[m_selectedIndex] : "None";
	if (ImGui::BeginCombo(m_label.c_str(), name.c_str()))
	{
		for (u32 i = 0; i < m_items.size(); ++i)
		{
			if (ImGui::Selectable(m_items[i].c_str(), isSelected))
			{
				m_selectedIndex = i;
				isSelected = true;
			}
			if (i == m_selectedIndex)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	return isSelected;
}

}