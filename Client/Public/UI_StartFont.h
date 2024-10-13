#pragma once

#include "UI_GameStart.h"
#include <queue>

BEGIN(Client)

class CUI_StartFont final :public CUI_GameStart
{
private:
	CUI_StartFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_StartFont(const CUI_StartFont& Prototype);
	virtual ~CUI_StartFont() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Bind_ShaderResources();

private:
	_float m_fMaskTimer = { 0.f };
	_bool m_bState = { FALSE };

	CTexture* m_pMaskTexture = { nullptr };

public:
	static CUI_StartFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END