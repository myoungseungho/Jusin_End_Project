#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct
	{
		_float			fSpeedPerSec = 1.f;
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

	_matrix Get_WorldMatrix() const {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	const _float4x4* Get_WorldMatrixPtr() const {
		return &m_WorldMatrix;
	}

	_matrix Get_WorldMatrix_Inverse() const {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

public:

	void Set_Scaled(_float fScaleX, _float fScaleY, _float fScaleZ) {
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[STATE_RIGHT][0], XMVector3Normalize(Get_State(STATE_RIGHT)) * fScaleX);
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[STATE_UP][0], XMVector3Normalize(Get_State(STATE_UP)) * fScaleY);
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[STATE_LOOK][0], XMVector3Normalize(Get_State(STATE_LOOK)) * fScaleZ);
	}


	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState][0], vState);
	}

	_float3 GetEulerAnglesFromRotationMatrix(const _matrix& rotationMatrix) {
		_float3 eulerAngles;

		// 행렬의 요소를 추출
		float r11 = XMVectorGetX(rotationMatrix.r[0]);
		float r12 = XMVectorGetY(rotationMatrix.r[0]);
		float r13 = XMVectorGetZ(rotationMatrix.r[0]);

		float r21 = XMVectorGetX(rotationMatrix.r[1]);
		float r22 = XMVectorGetY(rotationMatrix.r[1]);
		float r23 = XMVectorGetZ(rotationMatrix.r[1]);

		float r31 = XMVectorGetX(rotationMatrix.r[2]);
		float r32 = XMVectorGetY(rotationMatrix.r[2]);
		float r33 = XMVectorGetZ(rotationMatrix.r[2]);

		// 요(Yaw) 각도 계산 (rotation around Z axis)
		eulerAngles.y = atan2(r13, r33);

		// 피치(Pitch) 각도 계산 (rotation around Y axis)
		float c = sqrt(r11 * r11 + r31 * r31);
		eulerAngles.x = atan2(-r23, c);

		// 롤(Roll) 각도 계산 (rotation around X axis)
		eulerAngles.z = atan2(r21, r22);

		return eulerAngles;
	}

public:
	HRESULT Initialize();
	void SetUp_TransformDesc(const TRANSFORM_DESC* pTransformDesc);
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Turn(_fvector vAxis, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);
	void Rotation(_float3 vRotation);
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
