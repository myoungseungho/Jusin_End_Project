#include "Renderer.h"
#include "GameObject.h"
#include "RenderInstance.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"
#include "GameInstance.h"
#include "Component.h"
_uint		g_iSizeX = 8192;
_uint		g_iSizeY = 4608;

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pRenderInstance{ CRenderInstance::Get_Instance() }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pRenderInstance);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CRenderer::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	D3D11_VIEWPORT			ViewportDesc{};
	_uint					iNumViewport = { 1 };
	m_pContext->RSGetViewports(&iNumViewport, &ViewportDesc);

	/* 렌더타겟들을 추가한다. */
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_Diffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_Normal"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_Depth"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, XMVectorSet(0.f, 0.f, -1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_Player_Diffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_Player_Normal"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_Player_Depth"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, XMVectorSet(0.f, 0.f, -1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_PickDepth"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, XMVectorSet(0.f, 0.f, -1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_Shade"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, XMVectorSet(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_Specular"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, XMVectorSet(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;



	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_LightDepth"),
		g_iSizeX, g_iSizeY,
		DXGI_FORMAT_R32G32B32A32_FLOAT, XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* 효과를 줄 이펙트를 여기에 먼저 그린다*/
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_GlowDiffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_GlowAlpha"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_GlowDiffuse"), TEXT("Target_GlowDiffuse"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_GlowDiffuse"), TEXT("Target_GlowAlpha"))))
		return E_FAIL;

	/* 다운 처리*/
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_DownTarget"),ViewportDesc.Width / 2, ViewportDesc.Height/2,DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_DownTarget_Second"), ViewportDesc.Width / 4, ViewportDesc.Height / 4, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_Down"), TEXT("Target_DownTarget"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_DownSecond"), TEXT("Target_DownTarget_Second"))))
		return E_FAIL;

	/* 블러 처리 */
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_Blur_X"), ViewportDesc.Width / 4, ViewportDesc.Height / 4, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_Blur_Y"), ViewportDesc.Width / 4, ViewportDesc.Height / 4, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_Blur_X"), TEXT("Target_Blur_X"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_Blur_Y"), TEXT("Target_Blur_Y"))))
		return E_FAIL;

	/* 업 처리*/
	if (FAILED(m_pRenderInstance->Add_RenderTarget(TEXT("Target_UpTarget_Second"), ViewportDesc.Width / 2, ViewportDesc.Height / 2, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_UpSecond"), TEXT("Target_UpTarget_Second"))))
		return E_FAIL;


	/* 멀티렌더타겟에 타겟들을 모아놓는다. */
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;

	/* 멀티렌더타겟에 타겟들을 모아놓는다. */
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_Player"), TEXT("Target_Player_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_Player"), TEXT("Target_Player_Normal"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_Player"), TEXT("Target_Player_Depth"))))
		return E_FAIL;


	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_EffectToolPick"), TEXT("Target_PickDepth"))))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Add_MRT(TEXT("MRT_ShadowObjects"), TEXT("Target_LightDepth"))))
		return E_FAIL;

	XMStoreFloat4x4(&m_DownWorldMatrix, XMMatrixScaling(ViewportDesc.Width / 2, ViewportDesc.Height / 2, 1.f));
	m_DownWorldMatrix._41 = -((ViewportDesc.Width / 2) / 2);
	m_DownWorldMatrix._42 = ((ViewportDesc.Height / 2) / 2);

	XMStoreFloat4x4(&m_DownWorldMatrix_Second, XMMatrixScaling(ViewportDesc.Width / 4, ViewportDesc.Height / 4, 1.f));
	m_DownWorldMatrix_Second._41 = -((ViewportDesc.Width / 2) / 2) * 1.5;
	m_DownWorldMatrix_Second._42 = ((ViewportDesc.Height / 2) / 2) * 1.5;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;


	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pGlowShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred_Glow.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pGlowShader)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = g_iSizeX;
	TextureDesc.Height = g_iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pShadowDSV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	_float offsetY = 18.f;
#ifdef _DEBUG
	if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_DownTarget"), 100.f, 100.f + offsetY, 200.0f, 200.0f)))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_DownTarget_Second"), 100.f, 300.f, 200.0f, 200.0f)))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_UpTarget_Second"), 100.f, 500.f, 200.0f, 200.0f)))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_Blur_X"), 600.f, 100.f, 200.0f, 200.0f)))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_Blur_Y"), 350.f, 150.f, 300.f, 300.f)))
		return E_FAIL;

	//if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_Diffuse"), 100.f, 100.f, 200.0f, 200.0f)))
	//	return E_FAIL;
	//if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_Normal"), 100.f, 300.f, 200.0f, 200.0f)))
	//	return E_FAIL;
	//if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_Depth"), 100.f, 500.f, 200.0f, 200.0f)))
	//	return E_FAIL;
	//if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_PickDepth"), 600.f, 100.f, 200.0f, 200.0f)))
	//	return E_FAIL;
	//if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_Shade"), 350.f, 150.f, 300.f, 300.f)))
	//	return E_FAIL;
	//if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_Specular"), 350.f, 450.f, 300.f, 300.f)))
	//	return E_FAIL;

	//if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_PickDepth"), 350.f, 450.f, 300.f, 300.f)))
	//	return E_FAIL;
	//if (FAILED(m_pRenderInstance->Ready_RT_Debug(TEXT("Target_LightDepth"), 1920.0f - 150.0f, 150.f, 300.f, 300.f)))
	//	return E_FAIL;
#endif

	return S_OK;
}

HRESULT CRenderer::Add_RenderObject(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	if (eRenderGroup >= RG_END)
		return E_FAIL;

	m_RenderObjects[eRenderGroup].emplace_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

HRESULT CRenderer::Add_DebugComponent(CComponent* pDebugComponent)
{
	m_DebugComponent.emplace_back(pDebugComponent);

	Safe_AddRef(pDebugComponent);

	return S_OK;
}

HRESULT CRenderer::Draw(_float fTimeDelta)
{
	if (FAILED(Render_Priority(fTimeDelta)))
		return E_FAIL;
	if (FAILED(Render_Glow_Priority(fTimeDelta)))
		return E_FAIL;
	if (FAILED(Render_Blend_Priority(fTimeDelta)))
		return E_FAIL;
	if (FAILED(Render_NonBlend(fTimeDelta)))
		return E_FAIL;

	if (FAILED(Render_ShadowObj(fTimeDelta)))
		return E_FAIL;
	if (FAILED(Render_NonBlend_Test(fTimeDelta)))
		return E_FAIL;
	if (FAILED(Render_NonBlend_Layer(fTimeDelta)))
		return E_FAIL;

	if (FAILED(Render_Lights(fTimeDelta)))
		return E_FAIL;
	if (FAILED(Render_Deferred(fTimeDelta)))
		return E_FAIL;
	if (FAILED(Render_NonLight(fTimeDelta)))
		return E_FAIL;

	if (FAILED(Render_Blend(fTimeDelta)))
		return E_FAIL;
	if (FAILED(Render_Glow(fTimeDelta)))
		return E_FAIL;
	if (FAILED(Render_UI(fTimeDelta)))
		return E_FAIL;
	if (FAILED(Render_Node(fTimeDelta)))
		return E_FAIL;
	
#ifdef _DEBUG
	if (FAILED(Render_Debug(fTimeDelta)))
		return E_FAIL;
#endif


	return S_OK;
}

HRESULT CRenderer::Render_Priority(_float fTimeDelta)
{
	for (auto& pRenderObject : m_RenderObjects[RG_PRIORITY])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_ShadowObj(_float fTimeDelta)
{
	/* Target_LightDepth */
	if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_ShadowObjects"), m_pShadowDSV)))
		return E_FAIL;

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)g_iSizeX;
	ViewPortDesc.Height = (_float)g_iSizeY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);


	for (auto& pRenderObject : m_RenderObjects[RG_SHADOWOBJ])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_SHADOWOBJ].clear();

	if (FAILED(m_pRenderInstance->End_MRT()))
		return E_FAIL;

	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = 1920.f;
	ViewPortDesc.Height = 1080.0f;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);

	return S_OK;
}

HRESULT CRenderer::Render_Glow_Priority(_float fTimeDelta)
{
	for (auto& pRenderObject : m_RenderObjects[RG_GLOW_PRI])
	{
		if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_GlowDiffuse"))))
			return E_FAIL;

		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		if (FAILED(m_pRenderInstance->End_MRT()))
			return E_FAIL;

		if (FAILED(Draw_Glow(-1.f, pRenderObject->Get_GameObjectData())))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_GLOW_PRI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend_Priority(_float fTimeDelta)
{
	for (auto& pRenderObject : m_RenderObjects[RG_BLEND_PRI])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_BLEND_PRI].clear();

	for (auto& pRenderObject : m_RenderObjects[RG_GLOW_STAR])
	{
		if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_GlowDiffuse"))))
			return E_FAIL;

		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		if (FAILED(m_pRenderInstance->End_MRT()))
			return E_FAIL;

		if (FAILED(Draw_Glow(-1.f, pRenderObject->Get_GameObjectData())))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_GLOW_STAR].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend(_float fTimeDelta)
{
	if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_GameObjects"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONBLEND].clear();

	if (FAILED(m_pRenderInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Player(_float fTimeDelta)
{
	if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_Player"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONBLEND].clear();

	if (FAILED(m_pRenderInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend_Test(_float fTimeDelta)
{
	if (m_isLayerView == true)
	{
		/* Diffuse + Normal */
		//if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_GameObjects"))))
		//	return E_FAIL;

		for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND_TEST])
		{
			Safe_Release(pRenderObject);
		}

		m_RenderObjects[RG_NONBLEND_TEST].clear();

		//if (FAILED(m_pRenderInstance->End_MRT()))
		//	return E_FAIL;

		return S_OK;
	}

	/* Diffuse + Normal */
	if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_EffectToolPick"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND_TEST])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render(-1);

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONBLEND_TEST].clear();

	if (FAILED(m_pRenderInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend_Layer(_float fTimeDelta)
{
	if (m_isLayerView == false)
	{
		for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND_LAYER])
		{
			Safe_Release(pRenderObject);
		}

		m_RenderObjects[RG_NONBLEND_LAYER].clear();

		return S_OK;
	}

	/* Diffuse + Normal */
	if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_EffectToolPick"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND_LAYER])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render(-1);

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONBLEND_LAYER].clear();

	if (FAILED(m_pRenderInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Lights(_float fTimeDelta)
{
	/* 빛 연산한 결과를 렌더타겟에 기록해준다. (명암 + ) */
	/* Bind : Shade */
	if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float4x4 viewMatrixInv = m_pGameInstance->Get_Transform_Inverse_Float4x4(CPipeLine::D3DTS_VIEW);
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", &viewMatrixInv)))
		return E_FAIL;

	_float4x4 projMatrixInv = m_pGameInstance->Get_Transform_Inverse_Float4x4(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", &projMatrixInv)))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pShader, "g_NormalTexture", TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pShader, "g_DepthTexture", TEXT("Target_Depth"))))
		return E_FAIL;

	_float4 camPosition = m_pGameInstance->Get_CamPosition_Float4();
	if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", &camPosition, sizeof(_float4))))
		return E_FAIL;

	m_pVIBuffer->Bind_Buffers();

	m_pRenderInstance->Render_Lights(m_pShader, m_pVIBuffer);

	if (FAILED(m_pRenderInstance->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Render_Deferred(_float fTimeDelta)
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_RawValue("isOutLine", &m_isOutLine,sizeof(bool))))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pShader, "g_DiffuseTexture", TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pShader, "g_ShadeTexture", TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pShader, "g_SpecularTexture", TEXT("Target_Specular"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pShader, "g_LightDepthTexture", TEXT("Target_LightDepth"))))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pShader, "g_DepthTexture", TEXT("Target_Depth"))))
		return E_FAIL;



	_float4x4			LightViewMatrix, LightProjMatrix;

	XMStoreFloat4x4(&LightViewMatrix, XMMatrixLookAtLH(XMVectorSet(0.f, 10.f, 0.f, 1.f), XMVectorSet(1.f, -1.f, 1.f, 0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	XMStoreFloat4x4(&LightProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(120.0f), (_float)1920.f / 1080.f, 0.1f, 1000.f));

	if (FAILED(m_pShader->Bind_Matrix("g_LightViewMatrix", &LightViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_LightProjMatrix", &LightProjMatrix)))
		return E_FAIL;

	m_pShader->Begin(3);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();


	return S_OK;
}

HRESULT CRenderer::Render_NonLight(_float fTimeDelta)
{
	for (auto& pRenderObject : m_RenderObjects[RG_NONLIGHT])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Glow(_float fTimeDelta)
{
	for (auto& pRenderObject : m_RenderObjects[RG_GLOW])
	{
		if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_GlowDiffuse"))))
			return E_FAIL;

		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		Safe_Release(pRenderObject);

		if (FAILED(m_pRenderInstance->End_MRT()))
			return E_FAIL;

		if (FAILED(Draw_Glow(fTimeDelta)))
			return E_FAIL;

	}

	m_RenderObjects[RG_GLOW].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend(_float fTimeDelta)
{
	for (auto& pRenderObject : m_RenderObjects[RG_BLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI(_float fTimeDelta)
{
	for (auto& pRenderObject : m_RenderObjects[RG_UI])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Node(_float fTimeDelta)
{
	for (auto& pRenderObject : m_RenderObjects[RG_NODE])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render(fTimeDelta);

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NODE].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Debug(_float fTimeDelta)
{
	// Debug Component 관련 처리
	if (m_bShow_Debug_Component)
	{
		for (auto& pComponent : m_DebugComponent)
		{
			pComponent->Render(fTimeDelta);
			Safe_Release(pComponent);
		}

		m_DebugComponent.clear();
	}

	// Render Target 관련 처리
	if (m_bShow_RenderTarget)
	{
		// Matrix 바인딩
		if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
			return E_FAIL;
		if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
			return E_FAIL;

		// Render Target 디버그 렌더링
		if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
		if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_ShadowObjects"), m_pShader, m_pVIBuffer)))
			return E_FAIL;
	}
	if (!m_bShow_RenderTarget)
		return S_OK;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_Down"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_DownSecond"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_UpSecond"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_Blur_X"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_Blur_Y"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	//if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_EffectToolPick"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pRenderInstance->Render_RT_Debug(TEXT("MRT_ShadowObjects"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Draw_Glow(_float fTimeDelta, _int iPassIndex)
{
	if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_Down"))))
		return E_FAIL;

	if (FAILED(m_pGlowShader->Bind_Matrix("g_WorldMatrix", &m_DownWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pGlowShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pGlowShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float2 fTexSize = { 1920.f, 1080.f };
	if (FAILED(m_pGlowShader->Bind_RawValue("g_DownTexSize", &fTexSize, sizeof(_float2))))
		return E_FAIL;

	_float2 fSamplingSize = { 4.f,4.f };
	if (FAILED(m_pGlowShader->Bind_RawValue("g_DownSamplingSize", &fSamplingSize, sizeof(_float2))))
		return E_FAIL;

	// g_DownTexSize, g_DownSamplingSize
	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pGlowShader, "g_Texture", TEXT("Target_GlowDiffuse"))))
		return E_FAIL;

	m_pGlowShader->Begin(3);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pRenderInstance->End_MRT()))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_DownSecond"))))
		return E_FAIL;

	if (FAILED(m_pGlowShader->Bind_Matrix("g_WorldMatrix", &m_DownWorldMatrix_Second)))
		return E_FAIL;

	if (FAILED(m_pGlowShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pGlowShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	fTexSize = { 1920.f * 0.5f, 1080.f * 0.5f };
	if (FAILED(m_pGlowShader->Bind_RawValue("g_DownTexSize", &fTexSize, sizeof(_float2))))
		return E_FAIL;

	fSamplingSize = { 6.f,6.f };
	if (FAILED(m_pGlowShader->Bind_RawValue("g_DownSamplingSize", &fSamplingSize, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pGlowShader, "g_Texture", TEXT("Target_DownTarget"))))
		return E_FAIL;

	m_pGlowShader->Begin(3);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pRenderInstance->End_MRT()))
		return E_FAIL;
	//-------------------------------------------
	//return S_OK;
	//-------------------------------------------
	if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_Blur_X"))))
		return E_FAIL;

	if (FAILED(m_pGlowShader->Bind_Matrix("g_WorldMatrix", &m_DownWorldMatrix_Second)))
		return E_FAIL;
	if (FAILED(m_pGlowShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pGlowShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pGlowShader, "g_Texture", TEXT("Target_DownTarget_Second"))))
		return E_FAIL;

	m_pGlowShader->Begin(0);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pRenderInstance->End_MRT()))
		return E_FAIL;
	//****************************************
	if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_Blur_Y"))))
		return E_FAIL;

	if (FAILED(m_pGlowShader->Bind_Matrix("g_WorldMatrix", &m_DownWorldMatrix_Second)))
		return E_FAIL;
	if (FAILED(m_pGlowShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pGlowShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pGlowShader, "g_Texture", TEXT("Target_Blur_X"))))
		return E_FAIL;

	m_pGlowShader->Begin(1);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	if (FAILED(m_pRenderInstance->End_MRT()))
		return E_FAIL;

	//***************************************************
	//if (FAILED(m_pRenderInstance->Begin_MRT(TEXT("MRT_UpSecond"))))
	//	return E_FAIL;

	//if (FAILED(m_pGlowShader->Bind_Matrix("g_WorldMatrix", &m_DownWorldMatrix)))
	//	return E_FAIL;
	//if (FAILED(m_pGlowShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
	//	return E_FAIL;
	//if (FAILED(m_pGlowShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
	//	return E_FAIL;

	//if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pGlowShader, "g_Texture", TEXT("Target_DownTarget_Second"))))
	//	return E_FAIL;
	//if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pGlowShader, "g_BlurTexture", TEXT("Target_Blur_Y"))))
	//	return E_FAIL; 

	//m_pGlowShader->Begin(4);
	//m_pVIBuffer->Bind_Buffers();
	//m_pVIBuffer->Render();

	//if (FAILED(m_pRenderInstance->End_MRT()))
	//	return E_FAIL;
	
	
	if (FAILED(m_pGlowShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pGlowShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pGlowShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pGlowShader, "g_Texture", TEXT("Target_GlowDiffuse"))))
		return E_FAIL;
	//if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pGlowShader, "g_BlurTexture", TEXT("Target_UpTarget_Second"))))
	//	return E_FAIL;
	if (FAILED(m_pRenderInstance->Bind_RT_ShaderResource(m_pGlowShader, "g_BlurTexture", TEXT("Target_Blur_Y"))))
		return E_FAIL;

	if (fTimeDelta < 0)
	{
		if(iPassIndex == 0)
			m_pGlowShader->Begin(5);
		else
			m_pGlowShader->Begin(iPassIndex);
	}
	else
	{

		m_pGlowShader->Begin(2);
	}

	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	return S_OK;
}

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pDevice, pContext)))
	{
		MSG_BOX(TEXT("Failed to Created : CRenderer"));
		Safe_Release(pInstance);
	}

	return pInstance;
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
	for (auto& pRenderObject : m_DebugComponent)
		Safe_Release(pRenderObject);
	m_DebugComponent.clear();
	

	for (auto& pComponent : m_DebugComponent)
		Safe_Release(pComponent);

	Safe_Release(m_pShadowDSV);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pRenderInstance);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pGlowShader);
}
