#pragma once

#include "Client_Defines.h"
#include "Main_Camera.h"

BEGIN(Client)

class CVirtual_Camera final : public CCamera
{
public:
	enum CAMERA_MODE
	{
		CAMERA_NORMAL_MODE,
		CAMERA_FREE_MODE,
		CAMERA_CINEMATIC_MODE,
		CAMERA_MODE_END
	};

	enum CAMERA_PLAY_MODE {
		Playing,
		Paused,
		Stopped
	};

	enum InterpolationType {
		INTERPOLATION_LINEAR_MODE,
		INTERPOLATION_DAMPING_MODE,
		INTERPOLATION_SKIP_MODE,
		INTERPOLATION_END
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

	void Add_Point(_float duration, _int type, const _float4x4* pModelFloat4x4, _float damping, _bool hasWorldFloat4x4, _int animationIndex);
	void Remove_Point(_int currentIndex, _int animationIndex);
	void Move_Point(_int index, _int animationIndex);
	void Modify_Transform(_int index, _int animationIndex);
	_float AdjustT_Damping(_float t, _float damping);
	void Delete_Points(_int animationIndex);

	void ApplyCameraShake(_float fTimeDelta);
	void StartCameraShake(_float fDuration, _float fMagnitude);
	void StopCameraShake();

	void Set_CameraMode(CMain_Camera::VIRTUAL_CAMERA cameraMode);
public:
	const _char* GetTabName() const { return m_Name; };
	void Set_Player(CMain_Camera::PLAYER_STATE state, CGameObject* pPlayer);

	void Start_Play(_int animationIndex);
	void Pause();
	void Stop();
	void Button_Stop();

	_bool Get_IsPlay()
	{
		return m_currentPlayMode == CAMERA_PLAY_MODE::Playing;
	};

private:
	void Free_Camera(_float fTimeDelta);
	void Default_Camera(_float fTimeDelta);
	void Play(_float fTimeDelta);
	_float ComputeDistanceX(_gvector pos1, _gvector pos2);
	void Set_Camera_Position(_float averageX, _float distanceX, _float higherY, _gvector pos1, _gvector pos2);
	void Set_Camera_Direction(_float averageX, _gvector pos1, _gvector pos2);

public:
	CAMERA_MODE m_currentMode = { CAMERA_NORMAL_MODE };
	CAMERA_PLAY_MODE m_currentPlayMode = CAMERA_PLAY_MODE::Stopped;

	_char* m_Name;

	_int m_currentPointIndex = { 0 };
	_float m_elapsedTime = { 0.f };

	_float					m_fMouseSensor = {};
	_float					m_fMoveSpeed = {};

	//쉐이킹
	_bool				m_bIsShaking = { false };       // 흔들림 활성화 여부
	_float				m_fShakeDuration = { 3.f };
	_float				m_fElapsedShakeTime = 0.0f; // 흔들림 경과 시간
	_float				m_fShakeMagnitude = { 0.2f };
	_vector				m_vShakeOffset = XMVectorZero();
	_vector				m_vBaseCameraPosition = {};

	class CGameObject* m_p1pPlayer = { nullptr };
	class CGameObject* m_p2pPlayer = { nullptr };

	_float m_previousFOV = {};

	// 카메라 위치 스무딩을 위한 변수
	_float3 m_previousPosition = _float3(0.f, 17.f, -30.f);

	unordered_map<_int, vector<CameraPoint>> m_mapPoints;
	_int m_AnimationIndex = -1;

	class CGameObject* m_pCutsceneCharacter = { nullptr };

public:
	static CVirtual_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END