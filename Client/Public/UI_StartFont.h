#pragma once

#include "UI_Start.h"
#include <queue>

BEGIN(Client)

class CUI_StartFont final :public CUI_Start
{
private:
	CUI_StartFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_StartFont(const CUI_StartFont& Prototype);
	virtual ~CUI_StartFont() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Bind_ShaderResources();

private:
	void Action_StartAnim(_float fTimeDelta);

private:
	_float m_fMaskTimer = { 0.f };
	_float m_fAlphaTimer = { 0.f };

	_bool m_bStart = { FALSE};
	_bool m_bEnd = { FALSE };

	_float m_fStartAnimTimer = { 0.f };
	_float m_fOffSetPosY = {0.f};
	
	CTexture* m_pMaskTexture = { nullptr };

public:
	static CUI_StartFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END