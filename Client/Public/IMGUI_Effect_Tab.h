#pragma once
#include "IMGUI_Tab.h"


BEGIN(Client)

class CIMGUI_Effect_Tab : public CIMGUI_Tab
{

protected:
	CIMGUI_Effect_Tab();
	virtual ~CIMGUI_Effect_Tab() = default;

public:
	HRESULT Initialize() override;
	void Render(_float fTimeDelta) override;

	virtual const _char* GetTabName() const { return "Effect"; };

private:
	class CEffect_Manager* m_pEffect_Manager = { nullptr };

public:
	static CIMGUI_Effect_Tab* Create();
	virtual void Free() override;
};

END
