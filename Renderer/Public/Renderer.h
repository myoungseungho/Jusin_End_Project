#pragma once

#include "Base.h"
#include "Renderer_Defines.h"

/* 1. ȭ�鿡 �׷����� ��ü���� �׸��� ������� �����Ѵ�. */
/* 2. �����ϰ� �ִ� ��ü���� �����Լ��� ȣ���Ѵ�.(������) */

BEGIN(Engine)
class CGameObject;
END

BEGIN(Renderer)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_NONBLEND, RG_SHADOWOBJ, RG_NONLIGHT, RG_BLEND, RG_UI, RG_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw(_float fTimeDelta);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	list<class CGameObject*>			m_RenderObjects[RG_END];

private:
	HRESULT Render_Priority(_float fTimeDelta);
	HRESULT Render_NonBlend(_float fTimeDelta);
	HRESULT Render_Lights();
	HRESULT Render_Deferred();
	HRESULT Render_NonLight();
	HRESULT Render_Blend(_float fTimeDelta);
	HRESULT Render_UI(_float fTimeDelta);

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END