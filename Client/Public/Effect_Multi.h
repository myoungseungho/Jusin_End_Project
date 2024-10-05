#pragma once

#include "Client_Defines.h"
#include "Effect.h"


BEGIN(Client)

class CEffect_Multi : public CEffect
{
private:
	CEffect_Multi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Multi(const CGameObject& Prototype);
	virtual ~CEffect_Multi() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CEffect_Multi* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END