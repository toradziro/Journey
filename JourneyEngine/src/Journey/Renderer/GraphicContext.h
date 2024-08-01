#pragma once

namespace jny
{

class GraphicContext
{
public:
	virtual ~GraphicContext() {}

	virtual void swapBuffers() = 0;
};

}