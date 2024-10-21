#include "..\Public\GameObject.h"
#include "GameInstance.h"
#include "Transform.h"
#include "RenderInstance.h"

const _wstring		CGameObject::m_strTransformTag = TEXT("Com_Transform");

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
	, m_pRenderInstance{ CRenderInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pRenderInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

CGameObject::CGameObject(const CGameObject& Prototype)
	: m_pDevice{ Prototype.m_pDevice }
	, m_pContext{ Prototype.m_pContext }
	, m_pGameInstance{ Prototype.m_pGameInstance }
	, m_pRenderInstance{ Prototype.m_pRenderInstance }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pRenderInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;


	//if (nullptr != pArg)
	//{
	//	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	//	m_iGameObjectData = pDesc->iGameObjectData;
	//	m_pTransformCom->SetUp_TransformDesc(static_cast<CTransform::TRANSFORM_DESC*>(pArg));
	//}



	if (nullptr != pArg)
	{
		FILEDATA* pDesc = static_cast<FILEDATA*>(pArg);

		if (pDesc->isParsing)
		{
			if (pDesc != nullptr)
			{
				m_bIsPasingObject = pDesc->isParsing;
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->position), 1));
				m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVectorSetW(XMLoadFloat3(&pDesc->rightRotation), 0));
				m_pTransformCom->Set_State(CTransform::STATE_UP, XMVectorSetW(XMLoadFloat3(&pDesc->upRotation), 0));
				m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVectorSetW(XMLoadFloat3(&pDesc->lookRotation), 0));
				m_pTransformCom->Set_Scaled(pDesc->scale.x, pDesc->scale.y, pDesc->scale.z);
				m_pTransformCom->SetUp_TransformDesc(static_cast<CTransform::TRANSFORM_DESC*>(pArg));
			}
		}
		else
			m_pTransformCom->SetUp_TransformDesc(static_cast<CTransform::TRANSFORM_DESC*>(pArg));
	}



	if (nullptr != Get_Component(m_strTransformTag))
		return E_FAIL;

	m_Components.emplace(m_strTransformTag, m_pTransformCom);

	Safe_AddRef(m_pTransformCom);

	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render(_float fTimeDelta)
{
	return S_OK;
}

void CGameObject::OnCollisionEnter(CCollider* other, _float fTimeDelta)
{
}

void CGameObject::OnCollisionStay(CCollider* other, _float fTimeDelta)
{
}

void CGameObject::OnCollisionExit(CCollider* other)
{
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	/* 이미 strComponentTag키를 가진 컴포넌트가 있었다. */
	if (nullptr != Get_Component(strComponentTag))
		return E_FAIL;

	/* strComponentTag키를 가진 컴포넌트가 없었다.*/
	CComponent* pComponent = m_pGameInstance->Clone_Component(iPrototypeLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

void CGameObject::Free()
{
	__super::Free();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pRenderInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
