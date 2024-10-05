#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Free final : public CCamera
{

public:
	enum CAMERA_MODE
	{
		CAMERA_DEFAULT_MODE,
		CAMERA_FREE_MODE,
		CAMERA_CINEMATIC_MODE,
		CAMERA_MODE_END
	};

	enum CAMERA_CINEMATIC_MOVE_MODE
	{
		CAMERA_CINEMATIC_MOVE_LIENAR_MODE,
		CAMERA_CINEMATIC_MOVE_SPLINE_MODE,
		CAMERA_CINEMATIC_MOVE__MODE_END,
	};
public:
	typedef struct : public CCamera::CAMERA_DESC
	{
		_float		fSensor = {};
	}CAMERA_FREE_DESC;

public:
	struct CameraPoint
	{
		_vector localPosition; // 카메라 위치 (로컬 좌표)
		_float duration;       // 이동 시간
	};

private:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Free(const CCamera_Free& Prototype);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

	void StartCameraShake(_float fDuration, _float fMagnitude);
	void StopCameraShake();
	void StartCinematic_Mode(const wstring& cinematicKey, CGameObject* pTargetObject = nullptr);

private:
	void PlayerCamera();
	void FreeCamera(_float fTimeDelta);
	void ApplyCameraShake(_float fTimeDelta);
	void CinematicCamera(_float fTimeDelta);

	void InitializeCameraPoints();

	_vector CatmullRom(_vector p0, _vector p1, _vector p2, _vector p3, _float t);
	_vector CatmullRomSplinePosition(_float t);
		
public:
	CAMERA_MODE m_currentMode = { CAMERA_FREE_MODE };
	CAMERA_CINEMATIC_MOVE_MODE m_currentMoveMode = { CAMERA_CINEMATIC_MOVE_SPLINE_MODE };

private:
	_float				m_fMouseSensor = {};
	_float3				m_vCameraOffset = {};
	_float				m_fElapsedTime = { 0.f };
	_bool				m_bIsShaking = { false };       // 흔들림 활성화 여부
	_float				m_fShakeDuration = { 1.f };
	_float				m_fElapsedShakeTime = 0.0f; // 흔들림 경과 시간
	_float				m_fShakeMagnitude = { 0.5f };
	_vector				m_vShakeOffset = XMVectorZero();
	_vector				m_vBaseCameraPosition = {};


	size_t m_CurrentIndex = 0;              // 현재 카메라 포인트의 인덱스
	CameraPoint m_CurrentCameraPoint;       // 현재 카메라 포인트
	CameraPoint m_NextCameraPoint;          // 다음 카메라 포인트
	_bool m_bCinematicInitialized = false;   // 시네마틱 초기화 여부

	unordered_map<wstring, vector<CameraPoint>> m_Cinematics;
	// 현재 실행 중인 시네마틱의 키와 포인트들
	wstring m_CurrentCinematicKey;
	vector<CameraPoint>* m_pCurrentCinematicPoints = nullptr;

	// 카메라의 대상 객체
	class CGameObject* m_pTargetObject = { nullptr };

public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END