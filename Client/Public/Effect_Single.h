#pragma once

#include "Client_Defines.h"
#include "Effect.h"


BEGIN(Client)

class CEffect_Single : public CEffect
{
private:
	CEffect_Single(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Single(const CGameObject& Prototype);
	virtual ~CEffect_Single() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	virtual HRESULT Ready_Components(_wstring* pModelName, _wstring* pMaskTextureName, _wstring* pDiffuseTexturueName) override;
	virtual HRESULT Bind_ShaderResources() override;
	HRESULT Add_Render_Object();

public:
	static CEffect_Single* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END