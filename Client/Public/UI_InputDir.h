#pragma once

#include "UI_Input.h"

BEGIN(Client)

class CUI_InputDir final :public CUI_Input
{
private:
	CUI_InputDir(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_InputDir(const CUI_InputDir& Prototype);
	virtual ~CUI_InputDir() = default;

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
	static CUI_InputDir* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END