#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CVirtual_Camera abstract : public CGameObject
{
public:
	struct VIRTUAL_CAMERA_DESC : public CGameObject::GAMEOBJECT_DESC
	{
		_float3					vEye{}, vAt{};
		_float					fFovy{}, fNear{}, fFar{};

		_float					fSensor{};

		_float					fViewportWidth{};
		_float					fViewportHeight{};
	};

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

	VIRTUAL_CAMERA_DESC m_Desc = {};
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END