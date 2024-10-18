#pragma once

#include "UI_Input.h"

BEGIN(Client)

class CUI_InputDirEffect final :public CUI_Input
{
private:
	CUI_InputDirEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_InputDirEffect(const CUI_InputDirEffect& Prototype);
	virtual ~CUI_InputDirEffect() = default;

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
	static CUI_InputDirEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END