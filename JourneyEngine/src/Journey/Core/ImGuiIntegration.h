#pragma once

#include <imgui.h>
#include <cmath>

//inline bool operator==(const ImVec2& lhs, const ImVec2& rhs)
//{
//	return (std::fabs(lhs.x - rhs.x) < 1e-6f) && (std::fabs(lhs.y - rhs.y) < 1e-6f);
//}
//
//inline bool operator!=(const ImVec2& lhs, const ImVec2& rhs)
//{
//	return !(lhs == rhs);
//}
//
//inline bool operator<(const ImVec2& lhs, const ImVec2& rhs)
//{
//	if (lhs.x < rhs.x)
//	{
//		return true;
//	}
//	if (lhs.x > rhs.x)
//	{
//		return false;
//	}
//	return lhs.y < rhs.y;
//}
//
//inline bool operator<=(const ImVec2& lhs, const ImVec2& rhs)
//{
//	return (lhs < rhs) || (lhs == rhs);
//}
//
//inline bool operator>(const ImVec2& lhs, const ImVec2& rhs)
//{
//	return rhs < lhs;
//}
//
//inline bool operator>=(const ImVec2& lhs, const ImVec2& rhs)
//{
//	return (rhs < lhs) || (lhs == rhs);
//}