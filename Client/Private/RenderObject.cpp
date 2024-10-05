#include "stdafx.h"
#include "RenderObject.h"
#include "RenderInstance.h"

CRenderObject::CRenderObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
	, m_pRenderInstance { CRenderInstance::Get_Instance() }
{
	Safe_AddRef(m_pRenderInstance);
}

CRenderObject::CRenderObject(const CRenderObject& Prototype)
	: CGameObject(Prototype)
	, m_pRenderInstance{ Prototype.m_pRenderInstance }
{
	Safe_AddRef(m_pRenderInstance);
}

CGameObject* CRenderObject::Clone(void* pArg)
{
    return nullptr;
}

void CRenderObject::Free()
{
	Safe_Release(m_pRenderInstance);

	__super::Free();
}
