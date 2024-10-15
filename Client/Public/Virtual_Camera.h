#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CVirtual_Camera final : public CCamera
{
protected:
	CVirtual_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVirtual_Camera(const CVirtual_Camera& Prototype);
	virtual ~CVirtual_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	static CVirtual_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END