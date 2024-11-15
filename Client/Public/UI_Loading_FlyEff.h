#pragma once

#include "UIObject.h"

BEGIN(Engine)

class CVIBuffer_Trail_Rect;

END

BEGIN(Client)

class CUI_Loading_FlyEff final :public CUIObject
{
private:
	CUI_Loading_FlyEff(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Loading_FlyEff(const CUI_Loading_FlyEff& Prototype);
	virtual ~CUI_Loading_FlyEff() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Camera_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Bind_ShaderResources();

private:
	void Go_Target();

private:
	_float m_fAnimFream = { 0.f };
	_bool m_bAnimStop = { FALSE };

	CTransform* m_pHoleTransform = { nullptr };

public:
	static CUI_Loading_FlyEff* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END