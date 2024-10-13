#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CMain_Camera final : public CCamera
{
public:
	enum CAMERA_MODE
	{
		CAMERA_FREE_MODE,
		CAMERA_DEFAULT_MODE,
		CAMERA_MODE_END
	};

private:
	CMain_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMain_Camera(const CMain_Camera& Prototype);
	virtual ~CMain_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	void Free_Camera(_float fTimeDelta);
	void Default_Camera(_float fTimeDelta);

public:
	list<CCamera*> m_listVirtualCamera;

private:
	CCamera* m_current_Virtual_Camara = { nullptr };
	CAMERA_MODE m_currentMode = { CAMERA_FREE_MODE };

public:
	static CMain_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END