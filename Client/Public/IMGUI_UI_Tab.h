#pragma once
#include "IMGUI_Tab.h"


BEGIN(Client)

class CIMGUI_UI_Tab : public CIMGUI_Tab
{

protected:
	CIMGUI_UI_Tab();
	virtual ~CIMGUI_UI_Tab() = default;

public:
	HRESULT Initialize() override;
	void Render(_float fTimeDelta) override;

	virtual const _char* GetTabName() const { return "UI"; };

public:
	static CIMGUI_UI_Tab* Create();
	virtual void Free() override;
};

END
