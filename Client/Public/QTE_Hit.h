#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CQTE_Hit final : public CGameObject
{
private:
	CQTE_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQTE_Hit(const CQTE_Hit& Prototype);
	virtual ~CQTE_Hit() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	static CQTE_Hit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END