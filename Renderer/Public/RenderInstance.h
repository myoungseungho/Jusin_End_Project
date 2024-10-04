#pragma once

#include "Renderer.h"

BEGIN(Engine)

class ENGINE_DLL CRenderInstance final : public CBase
{
	DECLARE_SINGLETON(CRenderInstance)
private:
	CRenderInstance();
	virtual ~CRenderInstance() = default;

public:
	/* 엔진을 초기화한다. */
	HRESULT Initialize_Engine(HWND hWnd, _bool isWindowed, _uint iNumLevels, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void Update_Engine(_float fTimeDelta);
	HRESULT Render_Engine();

public: /* For.Renderer */
	HRESULT Add_RenderObject(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);

private:
	class CRenderer*					m_pRenderer = { nullptr };

public:
	void Release_Engine();
	
	virtual void Free() override;

};

END

