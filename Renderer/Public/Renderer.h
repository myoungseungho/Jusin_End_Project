#pragma once

#include "Base.h"
#include "Renderer_Defines.h"

/* 1. ȭ�鿡 �׷����� ��ü���� �׸��� ������� �����Ѵ�. */
/* 2. �����ϰ� �ִ� ��ü���� �����Լ��� ȣ���Ѵ�.(������) */

BEGIN(Engine)
class CGameObject;
class CShader;
class CVIBuffer_Rect;
class CGameInstance;
class CComponent;
END

BEGIN(Renderer)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_NONBLEND_TEST, RG_NONBLEND_LAYER, RG_SHADOWOBJ, RG_NONLIGHT, RG_GLOW, RG_BLEND, RG_UI, RG_NODE, RG_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT Draw(_float fTimeDelta);
	void SetActive_RenderTarget(_bool isOn) { m_bShow_RenderTarget = isOn; };

public:
	void Show_OutLine() { m_isOutLine = !m_isOutLine; }
	void Show_Layer_View() { m_isLayerView = !m_isLayerView; }
private:
	_bool m_isOutLine = { false };

	_bool m_isLayerView = { false };
private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	list<class CGameObject*>			m_RenderObjects[RG_END];
	list<class CComponent*>		m_DebugComponent;

	class CRenderInstance* m_pRenderInstance = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };

	class CShader* m_pShader = { nullptr };
	class CVIBuffer_Rect* m_pVIBuffer = { nullptr };
	ID3D11DepthStencilView* m_pShadowDSV = { nullptr };


	_float4x4					m_WorldMatrix = {};
	_float4x4					m_ViewMatrix = {};
	_float4x4					m_ProjMatrix = {};

	_bool m_bShow_RenderTarget = { false };

private:
	HRESULT Render_Priority(_float fTimeDelta);
	HRESULT Render_ShadowObj(_float fTimeDelta);
	HRESULT Render_NonBlend_Test(_float fTimeDelta);
	HRESULT Render_NonBlend_Layer(_float fTimeDelta);
	HRESULT Render_Lights(_float fTimeDelta);
	HRESULT Render_Deferred(_float fTimeDelta);
	HRESULT Render_NonLight(_float fTimeDelta);
	HRESULT Render_Glow(_float fTimeDelta);
	HRESULT Render_Blend(_float fTimeDelta);
	HRESULT Render_UI(_float fTimeDelta);
	HRESULT Render_Node(_float fTimeDelta);

#ifdef _DEBUG
private:
	HRESULT Render_Debug(_float fTimeDelta);
#endif

	HRESULT Draw_Glow(_float fTimeDelta);

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END