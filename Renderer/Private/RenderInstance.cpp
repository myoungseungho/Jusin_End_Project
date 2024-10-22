#include "RenderInstance.h"
#include "Target_Manager.h"
#include "GameInstance.h"
#include "Light_Manager.h"
#include "Picking.h"

IMPLEMENT_SINGLETON(CRenderInstance)

CRenderInstance::CRenderInstance()
{

}

HRESULT CRenderInstance::Initialize_Engine(HWND hWnd, _bool isWindowed, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	m_pLight_Manager = CLight_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pTarget_Manager = CTarget_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pPicking = CPicking::Create(*ppDevice, *ppContext, hWnd);
	if (nullptr == m_pPicking)
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

HRESULT CRenderInstance::Add_DebugComponent(CComponent* pDebugComponent)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_DebugComponent(pDebugComponent);
}

void CRenderInstance::SetActive_RenderTarget(_bool isOn)
{
	m_pRenderer->SetActive_RenderTarget(isOn);
}

void CRenderInstance::SetActive_Debug_Component(_bool isOn)
{
	m_pRenderer->SetActive_Debug_Component(isOn);
}

void CRenderInstance::Show_OutLine()
{
	m_pRenderer->Show_OutLine();
}

void CRenderInstance::Show_Layer_View()
{
	m_pRenderer->Show_Layer_View();
}

_bool CRenderInstance::Get_isLayerView()
{
	return m_pRenderer->Get_isLayerView();
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

HRESULT CRenderInstance::Begin_MRT_DoNotClear(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV)
{
	return m_pTarget_Manager->Begin_MRT_DoNotClear(strMRTTag, pDSV);
}

HRESULT CRenderInstance::End_MRT()
{
	return m_pTarget_Manager->End_MRT();
}

HRESULT CRenderInstance::Copy_RenderTarget(const _wstring& strTargetTag, ID3D11Texture2D* pTexture2D)
{
	return m_pTarget_Manager->Copy_RenderTarget(strTargetTag, pTexture2D);
}

ID3D11ShaderResourceView* CRenderInstance::Copy_RenderTarget_SRV(const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Copy_RenderTarget_SRV(strTargetTag);
}

HRESULT CRenderInstance::Bind_RT_ShaderResource(CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Bind_ShaderResource(pShader, pConstantName, strTargetTag);

}

HRESULT CRenderInstance::Add_ClientRenderTarget(const _wstring& strMRTTag, const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, _fvector vClearColor)
{
	return m_pTarget_Manager->Add_ClientRenderTarget(strMRTTag, strTargetTag, iWidth, iHeight, ePixelFormat, vClearColor);
}

HRESULT CRenderInstance::Add_ClientRenderTargetToMRT(const _wstring& strMRTTag, const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, _fvector vClearColor)
{
	return m_pTarget_Manager->Add_ClientRenderTargetToMRT(strMRTTag, strTargetTag, iWidth, iHeight, ePixelFormat, vClearColor);
}

#ifdef _DEBUG
HRESULT CRenderInstance::Ready_RT_Debug(const _wstring& strTargetTag, _float fCenterX, _float fCenterY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_Debug(strTargetTag, fCenterX, fCenterY, fSizeX, fSizeY);
}

HRESULT CRenderInstance::Render_RT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render_Debug(strMRTTag, pShader, pVIBuffer);
}
#endif

const LIGHT_DESC* CRenderInstance::Get_LightDesc(_uint iLightIndex) const
{
	return m_pLight_Manager->Get_LightDesc(iLightIndex);
}

HRESULT CRenderInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
	return m_pLight_Manager->Add_Light(LightDesc);
}

HRESULT CRenderInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pLight_Manager->Render_Lights(pShader, pVIBuffer);
}

_float4 CRenderInstance::Picked_Position(_bool* pPicked)
{
	return m_pPicking->Picked_Position(pPicked);
}

_int CRenderInstance::Picked_Effect_Index()
{
	return m_pPicking->Picked_Effect_Index();
}

void CRenderInstance::Release_Engine()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pPicking);

	CRenderInstance::Get_Instance()->Destroy_Instance();
}

void CRenderInstance::Free()
{
	__super::Free();
}
