#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_Logo final :public CUIObject
{
private:
	CUI_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Logo(const CUI_Logo& Prototype);
	virtual ~CUI_Logo() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components();

public:
	static CUI_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END