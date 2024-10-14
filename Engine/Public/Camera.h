#pragma once

#include "GameObject.h"
#include "Transform.h"

/* Ŭ���̾�Ʈ �����ڰ� ������ ī�޶���� ��ӹ޾ƾ��� �θ�Ŭ����. */
/* ��� ī�޶� �ʿ��� �����Ϳ� ����� �����Ѵ�. */

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

	// ī�޶� ����Ʈ ����ü
	struct CameraPoint {
		//������
		_float3 position = {};

		//ȸ�� ���ʹϾ�
		_float4 rotation = {};

		_float duration; // ���� ����Ʈ���� �̵� �ð�
		InterpolationType interpolationType; //���� Ÿ��
		
		const _float4x4* pWorldFloat4x4;
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
	void Prev_Play(CCamera* camera, _float fTimeDelta);
	void Prev_Stop();
	void Add_Point(_float duration, InterpolationType type, const _float4x4* pModelFloat4x4);
	void Remove_Point(_int currentIndex);
	void Move_Point(_int index);
	void Modify_Transform(_int index);

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