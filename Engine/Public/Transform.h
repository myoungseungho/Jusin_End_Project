#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct
	{
		_float			fSpeedPerSec;
		_float			fRotationPerSec;
	} TRANSFORM_DESC;

public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTransform() = default;

public:
	/* 저장용 데이터 -> 연산용 데이터 XMLoad */
	/* 연산용 데이터 -> 저장용 데이터 XMStore */

	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_float3 Get_Scaled() const {
		_matrix		WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		return _float3(XMVectorGetX(XMVector3Length(WorldMatrix.r[STATE_RIGHT])),
			XMVectorGetX(XMVector3Length(WorldMatrix.r[STATE_UP])),
			XMVectorGetX(XMVector3Length(WorldMatrix.r[STATE_LOOK])));
	}
	

	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState][0], vState);
	}

public:
	HRESULT Initialize();
	void SetUp_TransformDesc(const TRANSFORM_DESC* pTransformDesc);

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Turn(_fvector vAxis, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);
	void LookAt(_fvector vAt);


private:
	_float4x4				m_WorldMatrix = {};
	_float					m_fSpeedPerSec = {};
	_float					m_fRotationPerSec = {};

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
