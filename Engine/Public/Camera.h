#pragma once

#include "GameObject.h"
#include "Transform.h"

/* 클라이언트 개발자가 제작한 카메라들이 상속받아야할 부모클래스. */
/* 모든 카메라에 필요한 데이터와 기능을 구현한다. */

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{
public:
	typedef struct : public CTransform::TRANSFORM_DESC
	{
		_float3		vEye, vAt;
		_float		fFovy, fNear, fFar, fSensor;
		_char* cName;
	}CAMERA_DESC;

public:
	struct CameraSaveData {
		vector<CCamera*>& vecVirtualCamera;
		unordered_map<pair<CAMERA_MODELID, CAMERA_SKILLID>, _uint, pair_hash>& cameraIndexMap;
	};

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& Prototype);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

	void Update_Camera(CCamera* camera, _bool isPrevPlay, _float fTimeDelta);
	void Play(CCamera* camera, _float fTimeDelta);
	void Prev_Stop();
	void Add_Point(_float duration, InterpolationType type, const _float4x4* pModelFloat4x4, _float damping);
	void Remove_Point(_int currentIndex);
	void Move_Point(_int index);
	void Modify_Transform(_int index);
	_float AdjustT_Damping(_float t, _float damping);
	void Delete_Points();

public:
	const _char* GetTabName() const { return m_Name; };

public:
	_float3					m_vEye{}, m_vAt{};
	_float					m_fFovy{}, m_fNear{}, m_fFar{};

	_float					m_fViewportWidth{}, m_fViewportHeight{};
	_float					m_fMouseSensor = {};
	_float					m_fMoveSpeed = {};

	_char* m_Name;

	vector<CameraPoint> m_vecPoints;
	_int m_currentPointIndex = { 0 };
	_float m_elapsedTime = { 0.f };
	_bool m_bPrevPlayMode = { false };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END