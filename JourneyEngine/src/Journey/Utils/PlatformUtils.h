#pragma once

namespace jny
{

namespace fs
{

//-- std::string{} on cancel
std::string openFileDialog(const char* filter);
//-- std::string{} on cancel
std::string saveFileDialog(const char* filter);

}

}