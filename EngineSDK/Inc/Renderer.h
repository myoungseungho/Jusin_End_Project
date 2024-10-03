#pragma once

#include "Base.h"

/* 1. 화면에 그려야할 객체들을 그리는 순서대로 보관한다. */
/* 2. 보관하고 있는 객체들의 렌더함수를 호출한다.(렌더콜) */

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_NONBLEND, RG_BLEND, RG_UI, RG_END };
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	list<class CGameObject*>			m_RenderObjects[RG_END];

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END