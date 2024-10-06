#include "RenderInstance.h"
#include "Target_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CRenderInstance)

CRenderInstance::CRenderInstance()
{

}

HRESULT CRenderInstance::Initialize_Engine(HWND hWnd, _bool isWindowed, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext, CGameInstance* gameInstance)
{
	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext, gameInstance);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderInstance::Render_Engine(_float fTimeDelta)
{
	/* 엔진에서 관리하는 객체들 중, 반복적인 렌더가 필요한 객체들이 있다면. */
	/* 여기에서 렌더를 수행해준다. */

	if (FAILED(m_pRenderer->Draw(fTimeDelta)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderInstance::Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderObject(eRenderGroup, pRenderObject);
}

HRESULT CRenderInstance::Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, _fvector vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iWidth, iHeight, ePixelFormat, vClearColor);
}

HRESULT CRenderInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}

HRESULT CRenderInstance::Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag, pDSV);
}

HRESULT CRenderInstance::End_MRT()
{
	return m_pTarget_Manager->End_MRT();
}

HRESULT CRenderInstance::Copy_RenderTarget(const _wstring& strTargetTag, ID3D11Texture2D* pTexture2D)
{
	return m_pTarget_Manager->Copy_RenderTarget(strTargetTag, pTexture2D);
}

HRESULT CRenderInstance::Bind_RT_ShaderResource(CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Bind_ShaderResource(pShader, pConstantName, strTargetTag);

}

HRESULT CRenderInstance::Ready_RT_Debug(const _wstring& strTargetTag, _float fCenterX, _float fCenterY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_Debug(strTargetTag, fCenterX, fCenterY, fSizeX, fSizeY);
}

HRESULT CRenderInstance::Render_RT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render_Debug(strMRTTag, pShader, pVIBuffer);
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
