#pragma once

#include "UI_Start.h"
#include <queue>

BEGIN(Client)

class CUI_FightFont final :public CUI_Start
{
private:
	CUI_FightFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_FightFont(const CUI_FightFont& Prototype);
	virtual ~CUI_FightFont() = default;

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
	_float m_fAlphaTimer = { 0.f };

	CTexture* m_pMaskTexture = { nullptr };

public:
	static CUI_FightFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END