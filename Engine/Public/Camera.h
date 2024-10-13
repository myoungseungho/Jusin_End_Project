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
	enum InterpolationType {
		INTERPOLATION_LINEAR_MODE,
		INTERPOLATION_SPLINE_MODE,
		INTERPOLATION_SKIP_MODE,
		INTERPOLATION_END
	};

	// 카메라 포인트 구조체
	struct CameraPoint {
		//포지션
		_float3 position = {};

		//월드행렬의 각 1,2,3 행벡터의 단위벡터
		_float3 rotationX = {};
		_float3 rotationY = {};
		_float3 rotationZ = {};

		_float duration; // 다음 포인트까지 이동 시간
		InterpolationType interpolationType;
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

	void Update_Camera(CCamera* camera);
	void Add_Point(_float duration, InterpolationType type);

public:
	const _char* GetTabName() const { return m_Name; };

public:
	_float3					m_vEye{}, m_vAt{};
	_float					m_fFovy{}, m_fNear{}, m_fFar{};

	_float					m_fViewportWidth{}, m_fViewportHeight{};
	_float					m_fMouseSensor = {};
	_float					m_fMoveSpeed = {};

	_char* m_Name;

	list<CameraPoint> m_listPoints;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END