#pragma once
#include "IMGUI_Tab.h"
#include "Effect_Rect.h"

BEGIN(Client)

class CIMGUI_Shader_Tab : public CIMGUI_Tab
{

protected:
	CIMGUI_Shader_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMGUI_Shader_Tab() = default;

public:
	HRESULT Initialize() override;
	void Render(_float fTimeDelta) override;

	void Set_EffectRect(CEffect_Rect* pEffectRect) { m_Effect_Rect = pEffectRect; }
	virtual const _char* GetTabName() const { return "Shader"; };

private:
	CEffect_Rect* m_Effect_Rect = { nullptr };
	
public:
	static CIMGUI_Shader_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
