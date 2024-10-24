#pragma once

#include "Base.h"
#include "Renderer_Defines.h"

/* 1. 화면에 그려야할 객체들을 그리는 순서대로 보관한다. */
/* 2. 보관하고 있는 객체들의 렌더함수를 호출한다.(렌더콜) */

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
						/* 맵은 프리올리티로 바로 그릴것 디퍼드(빛연산)이 필요한 애들은 논블렌드 */
	enum RENDERGROUP { RG_PRIORITY, RG_NONBLEND_TEST, RG_NONBLEND_LAYER, RG_GLOW_PRI, RG_BLEND_PRI, RG_GLOW_STAR,
		RG_NONBLEND, RG_PLAYER, RG_SHADOWOBJ,
		RG_NONLIGHT, RG_GLOW, RG_BLEND, RG_UI, RG_NODE, RG_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject, string strName = "");
	HRESULT Add_DebugComponent(class CComponent* pDebugComponent);
	HRESULT Draw(_float fTimeDelta);
	void SetActive_RenderTarget(_bool isOn) { m_bShow_RenderTarget = isOn; };
	void SetActive_Debug_Component(_bool isOn) { m_bShow_Debug_Component = isOn; };

public:
	void Show_OutLine() { m_isOutLine = !m_isOutLine; }
	void Show_Layer_View() { m_isLayerView = !m_isLayerView; }
	_bool Get_isLayerView() { return m_isLayerView; }

private:
	_bool m_isOutLine = { false };

	_bool m_isLayerView = { false };
private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	list<class CGameObject*>			m_RenderObjects[RG_END];
	vector<string>				m_PlayerStrNames;
	list<class CComponent*>		m_DebugComponent;

	class CRenderInstance* m_pRenderInstance = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };

	class CShader* m_pShader = { nullptr };
	class CShader* m_pGlowShader = { nullptr };
	class CVIBuffer_Rect* m_pVIBuffer = { nullptr };

	ID3D11DepthStencilView* m_pShadowDSV = { nullptr };

	_float4x4					m_DownWorldMatrix = {};
	_float4x4					m_DownWorldMatrix_Second = {};
	_float4x4					m_WorldMatrix = {};
	_float4x4					m_ViewMatrix = {};
	_float4x4					m_ProjMatrix = {};

	_bool m_bShow_RenderTarget = { false };
	_bool m_bShow_Debug_Component = { false };

private:
	HRESULT Render_Priority(_float fTimeDelta);
	HRESULT Render_ShadowObj(_float fTimeDelta);
	HRESULT Render_Glow_Priority(_float fTimeDelta);
	HRESULT Render_Blend_Priority(_float fTimeDelta);
	HRESULT Render_NonBlend(_float fTimeDelta);
	HRESULT Render_Player(_float fTimeDelta);
	HRESULT Render_PlayerLight(_float fTimeDelta, _int iCount);
	HRESULT Render_PlayerDeferred(_float fTimeDelta);
	HRESULT Render_NonBlend_Test(_float fTimeDelta); // 디버깅용 피킹
	HRESULT Render_NonBlend_Layer(_float fTimeDelta);// 디버깅용 피킹
	HRESULT Render_Lights(_float fTimeDelta);
	HRESULT Render_Deferred(_float fTimeDelta);
	HRESULT Render_NonLight(_float fTimeDelta);
	HRESULT Render_Glow(_float fTimeDelta);
	HRESULT Render_Blend(_float fTimeDelta);
	HRESULT Render_UI(_float fTimeDelta);
	HRESULT Render_Node(_float fTimeDelta);


private:
	HRESULT Render_Debug(_float fTimeDelta);


	HRESULT Draw_Glow(_float fTimeDelta, _int iPassIndex = -1);

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END