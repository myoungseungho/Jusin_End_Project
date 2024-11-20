#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCharaSelectCamera final : public CCamera
{
public:
	typedef struct : public CCamera::CAMERA_DESC
	{
		_float		fSensor = {};
	}CAMERA_FREE_DESC;

private:
	CCharaSelectCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharaSelectCamera(const CCharaSelectCamera& Prototype);
	virtual ~CCharaSelectCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	_float				m_fMouseSensor = {};
	CTransform* m_pLobby_Goku_Transform = { nullptr };

public:
	static CCharaSelectCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END