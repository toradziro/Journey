#pragma once

namespace jny
{

class GraphicContext
{
public:
	virtual ~GraphicContext() = default;

	virtual void swapBuffers() = 0;
};

}