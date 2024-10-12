#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CVirtual_Camera abstract : public CGameObject
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

protected:
	CameraData m_CameraData = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END