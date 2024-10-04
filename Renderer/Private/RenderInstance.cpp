#include "RenderInstance.h"

IMPLEMENT_SINGLETON(CRenderInstance)

CRenderInstance::CRenderInstance()	
{

}

HRESULT CRenderInstance::Initialize_Engine(HWND hWnd, _bool isWindowed, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderInstance::Render_Engine()
{
	/* 엔진에서 관리하는 객체들 중, 반복적인 렌더가 필요한 객체들이 있다면. */
	/* 여기에서 렌더를 수행해준다. */

	if (FAILED(m_pRenderer->Draw()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderInstance::Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, CGameObject * pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObject(eRenderGroup, pRenderObject);	
}


void CRenderInstance::Release_Engine()
{

	Safe_Release(m_pRenderer);

	CRenderInstance::Get_Instance()->Destroy_Instance();
}

void CRenderInstance::Free()
{
	__super::Free();

	
}
