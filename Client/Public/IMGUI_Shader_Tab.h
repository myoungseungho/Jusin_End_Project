#pragma once
#include "IMGUI_Tab.h"


BEGIN(Client)

class CIMGUI_Shader_Tab : public CIMGUI_Tab
{

protected:
	CIMGUI_Shader_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMGUI_Shader_Tab() = default;

public:
	HRESULT Initialize() override;
	void Render(_float fTimeDelta) override;

	virtual const _char* GetTabName() const { return "Shader"; };

	
public:
	static CIMGUI_Shader_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
