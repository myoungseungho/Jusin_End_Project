#include "..\Public\Renderer.h"

#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{

	return S_OK;
}

HRESULT CRenderer::Add_RenderObject(RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (eRenderGroup >= RG_END)
		return E_FAIL;
	
	m_RenderObjects[eRenderGroup].emplace_back(pRenderObject);

	Safe_AddRef(pRenderObject);
	
	return S_OK;
}

HRESULT CRenderer::Draw()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_NonBlend()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_UI()))
		return E_FAIL;

	



	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pRenderObject : m_RenderObjects[RG_PRIORITY])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONBLEND].clear();

	return S_OK;
}
//
//_bool Compare(CGameObject* pSour, CGameObject* pDest)
//{
//	return ((CBlendObject*)pSour)->Get_ViewZ() > ((CBlendObject*)pDest)->Get_ViewZ();
//}

HRESULT CRenderer::Render_Blend()
{
	//m_RenderObjects[RG_BLEND].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
	//{
	//	return ((CBlendObject*)pSour)->Get_ViewZ() > ((CBlendObject*)pDest)->Get_ViewZ();
	//});

	for (auto& pRenderObject : m_RenderObjects[RG_BLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	

	for (auto& pRenderObject : m_RenderObjects[RG_UI])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_UI].clear();

	return S_OK;
}

CRenderer * CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CRenderer(pDevice, pContext);
}

void CRenderer::Free()
{
	__super::Free();

	for (size_t i = 0; i < RG_END; i++)
	{
		for (auto& pRenderObject : m_RenderObjects[i])
			Safe_Release(pRenderObject);
		m_RenderObjects[i].clear();
	}

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
