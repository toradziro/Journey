#pragma once

namespace jny
{

class Texture : public ReferenceCounter
{
public:
	virtual ~Texture() = default;

	virtual uint32_t width() const = 0;
	virtual uint32_t height() const = 0;

	virtual void bind(uint32_t slot = 0) const = 0;
};

class Texture2D : public Texture
{
public:
	virtual ~Texture2D() = default;

	static Ref<Texture2D> create(const std::string& texturePath);
};

} //--jny
