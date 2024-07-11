#include "Application.h"

namespace Journey
{

__declspec(dllimport) void print();

}

int main()
{
	Journey::print();
}
