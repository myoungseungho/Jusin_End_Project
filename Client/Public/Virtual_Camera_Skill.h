#pragma once

#include "Client_Defines.h"
#include "Virtual_Camera.h"

BEGIN(Client)

class CVirtual_Camera_Skill final : public CVirtual_Camera
{
protected:
	CVirtual_Camera_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVirtual_Camera_Skill(const CVirtual_Camera_Skill& Prototype);
	virtual ~CVirtual_Camera_Skill() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	static CVirtual_Camera_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END