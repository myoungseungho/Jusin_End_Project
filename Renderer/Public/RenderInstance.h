#pragma once

#include "Renderer.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Renderer)

class RENDERER_DLL CRenderInstance final : public CBase
{
	DECLARE_SINGLETON(CRenderInstance)
private:
	CRenderInstance();
	virtual ~CRenderInstance() = default;

public:
	/* 엔진을 초기화한다. */
	HRESULT Initialize_Engine(HWND hWnd, _bool isWindowed, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext, class CGameInstance* gameInstance);
	HRESULT Render_Engine(_float fTimeDelta);

public: /* For.Renderer */
	HRESULT Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	void SetActive_RenderTarget(_bool isOn);

public: /* For.Target_Manager */
	HRESULT Add_RenderTarget(const _wstring& strTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, _fvector vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	HRESULT Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr);
	HRESULT End_MRT();
	HRESULT Copy_RenderTarget(const _wstring& strTargetTag, ID3D11Texture2D* pTexture2D);
	HRESULT Bind_RT_ShaderResource(class CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag);

#ifdef _DEBUG
public:
	HRESULT Ready_RT_Debug(const _wstring& strTargetTag, _float fCenterX, _float fCenterY, _float fSizeX, _float fSizeY);
	HRESULT Render_RT_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif

public:/*For.Light_Manager*/
	const LIGHT_DESC* Get_LightDesc(_uint iLightIndex) const;
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
private:
	class CRenderer* m_pRenderer = { nullptr };
	class CTarget_Manager* m_pTarget_Manager = { nullptr };
	class CLight_Manager* m_pLight_Manager = { nullptr };

public:
	void Release_Engine();

	virtual void Free() override;
};

END

