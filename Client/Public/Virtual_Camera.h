#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CVirtual_Camera final : public CCamera
{
public:
	enum CAMERA_MODE
	{
		CAMERA_FREE_MODE,
		CAMERA_DEFAULT_MODE,
		CAMERA_MODE_END
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


private:
	void Free_Camera(_float fTimeDelta);
	void Default_Camera(_float fTimeDelta);

private:
	CAMERA_MODE m_currentMode = { CAMERA_FREE_MODE };

public:
	static CVirtual_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END