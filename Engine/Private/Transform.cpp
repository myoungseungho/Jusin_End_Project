#include "..\Public\Transform.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent { pDevice, pContext }
{
}

HRESULT CTransform::Initialize()
{
	
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

void CTransform::SetUp_TransformDesc(const TRANSFORM_DESC* pTransformDesc)
{
	m_fSpeedPerSec = pTransformDesc->fSpeedPerSec;
	m_fRotationPerSec = pTransformDesc->fRotationPerSec;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);	

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vDirection = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	vPosition += vDirection * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Down(_float fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vDirection = XMVectorSet(0.f, -1.f, 0.f, 0.f);
	vPosition += vDirection * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_float3		vScale = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}


void CTransform::Rotation(_float3 vRotation)
{
	// 입력된 회전 값을 쿼터니언으로 변환
	_vector newRotationQuaternion = XMQuaternionRotationRollPitchYaw(vRotation.x, vRotation.y, vRotation.z);

	// 쿼터니언을 회전 행렬로 변환
	_matrix rotationMatrix = XMMatrixRotationQuaternion(newRotationQuaternion);

	// 스케일을 유지하기 위해 기존 코드를 사용
	_float3 vScale = Get_Scaled();
	_vector vRight = XMVector3Normalize(rotationMatrix.r[0]) * vScale.x;
	_vector vUp = XMVector3Normalize(rotationMatrix.r[1]) * vScale.y;
	_vector vLook = XMVector3Normalize(rotationMatrix.r[2]) * vScale.z;

	// 상태 설정
	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::LookAt(_fvector vAt)
{
	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vLook = vAt - vPosition;
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	_float3		vScaled = Get_Scaled();

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);

}

HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*		pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	return nullptr;
}


void CTransform::Free()
{
	__super::Free();

}
