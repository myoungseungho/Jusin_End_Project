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

public:
	typedef struct : public CCamera::CAMERA_DESC
	{
		_float		fSensor = {};
	}CAMERA_FREE_DESC;

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
	void FreeCamera(_float fTimeDelta);
	void DefaultCamera(_float fTimeDelta);
private:
	_float				m_fMouseSensor = {};
	list<class CVirtual_Camera*> m_listVirtualCamera;
	class CVirtual_Camera* m_current_Virtual_Camara = { nullptr };
	CAMERA_MODE m_currentMode = { CAMERA_FREE_MODE };

public:
	static CMain_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END