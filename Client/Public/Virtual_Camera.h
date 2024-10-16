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
		CAMERA_CINEMATIC_MODE,
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

	void Add_Point(_float duration, InterpolationType type, const _float4x4* pModelFloat4x4, _float damping, _bool hasWorldFloat4x4);
	void Remove_Point(_int currentIndex);
	void Move_Point(_int index);
	void Modify_Transform(_int index);
	_float AdjustT_Damping(_float t, _float damping);
	void Delete_Points();

	void ApplyCameraShake(_float fTimeDelta);
	void StartCameraShake(_float fDuration, _float fMagnitude);
	void StopCameraShake();

public:
	const _char* GetTabName() const { return m_Name; };

private:
	void Free_Camera(_float fTimeDelta);
	void Play(_float fTimeDelta);
	void Prev_Stop();

public:
	CAMERA_MODE m_currentMode = { CAMERA_FREE_MODE };
	_char* m_Name;

	_int m_currentPointIndex = { 0 };
	_float m_elapsedTime = { 0.f };

	_float					m_fMouseSensor = {};
	_float					m_fMoveSpeed = {};

	//쉐이킹
	_bool				m_bIsShaking = { false };       // 흔들림 활성화 여부
	_float				m_fShakeDuration = { 0.f };
	_float				m_fElapsedShakeTime = 0.0f; // 흔들림 경과 시간
	_float				m_fShakeMagnitude = { 0.5f };
	_vector				m_vShakeOffset = XMVectorZero();
	_vector				m_vBaseCameraPosition = {};


public:
	static CVirtual_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END