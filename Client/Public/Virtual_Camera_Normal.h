#pragma once

#include "Client_Defines.h"
#include "Virtual_Camera.h"

BEGIN(Client)

class CVirtual_Camera_Normal final : public CVirtual_Camera
{
protected:
	CVirtual_Camera_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVirtual_Camera_Normal(const CVirtual_Camera_Normal& Prototype);
	virtual ~CVirtual_Camera_Normal() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	static CVirtual_Camera_Normal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END