#pragma once

#include "UIObject.h"
#include "Monster.h"

BEGIN(Engine)

class CTexture;

END

BEGIN(Client)

class CUI_HpGauge final :public CUIObject
{
private:
	CUI_HpGauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_HpGauge(const CUI_HpGauge& Prototype);
	virtual ~CUI_HpGauge() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components();

private:
	_float m_iCharaCurrHp = { 0.f };
	_float m_fHpRadio = { 0.f };

	_float m_fTemp = { 0.f };

public:
	static CUI_HpGauge* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END