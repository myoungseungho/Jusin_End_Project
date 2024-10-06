#pragma once
#include "IMGUI_Tab.h"


BEGIN(Client)

class CIMGUI_Animation_Tab : public CIMGUI_Tab
{

protected:
	CIMGUI_Animation_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMGUI_Animation_Tab() = default;

public:
	HRESULT Initialize() override;
	void Render(_float fTimeDelta) override;

	virtual const _char* GetTabName() const { return "Animation"; };

public:
	static CIMGUI_Animation_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
