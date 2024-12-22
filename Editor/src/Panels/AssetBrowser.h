#pragma once

#pragma once

#include "Journey/Scene/Scene.h"
#include "Context.h"
#include "Panel.h"
#include "Journey/Core/fs/VirtualFileSystem.h"

namespace jny
{

class AssetBrowser final : public IPanel
{
public:
	AssetBrowser(const Ref<EditorContext>& ctx);
	~AssetBrowser() = default;

	virtual void	update(f32 dt) override;
	virtual void	updateUI() override;

private:
	void			drawToolbar();
	void			drawContent();

	fs_path	m_currPath = {};
};

}