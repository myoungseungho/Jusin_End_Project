#include "stdafx.h"
#include "..\Public\Camera_Free.h"

#include "GameInstance.h"

CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
	: CCamera{ Prototype }
{

}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	m_fMouseSensor = static_cast<CCamera_Free::CAMERA_FREE_DESC*>(pArg)->fSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



	CTransform::TRANSFORM_DESC pDesc{};
	pDesc.fSpeedPerSec = 5.f;
	pDesc.fRotationPerSec = 5.f;


	m_pTransformCom->SetUp_TransformDesc(&pDesc);
	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{
	if(m_pGameInstance->Key_Pressing(DIK_LSHIFT))
	{

		if (GetKeyState('A') & 0x8000)
		{
			m_pTransformCom->Go_Left(fTimeDelta);
		}

		if (GetKeyState('D') & 0x8000)
		{
			m_pTransformCom->Go_Right(fTimeDelta);
		}

		if (GetKeyState('W') & 0x8000)
		{
			m_pTransformCom->Go_Straight(fTimeDelta);
			//_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			//vPos += {0.f, fTimeDelta * 2.f, 0.f, 0.f};
			//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

		}

		if (GetKeyState('S') & 0x8000)
		{
			m_pTransformCom->Go_Backward(fTimeDelta);
			//_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			//vPos -= {0.f, fTimeDelta * 2.f, 0.f, 0.f};
			//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		}

		if (GetKeyState('Q') & 0x8000)
		{
			_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vPos += {0.f, fTimeDelta * 2.f, 0.f, 0.f};
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			//m_pTransformCom->Go_Straight(fTimeDelta);

		}
		if (GetKeyState('E') & 0x8000)
		{
			_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vPos -= {0.f, fTimeDelta * 2.f, 0.f, 0.f};
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			//m_pTransformCom->Go_Backward(fTimeDelta);

		}
		_long		MouseMove = {};

		if (GetKeyState(MK_RBUTTON) & 0x8000)
		{

			if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_X))
			{
				m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMouseSensor * MouseMove * fTimeDelta);
			}

			if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_Y))
			{
				m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), m_fMouseSensor * MouseMove * fTimeDelta);
			}
		}
	}
	__super::Priority_Update(fTimeDelta);
}

void CCamera_Free::Update(_float fTimeDelta)
{
}

void CCamera_Free::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render(_float fTimeDelta)
{
	return S_OK;
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();

}
