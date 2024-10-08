#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "GameInstance.h"
#include "UI_Manager.h"

BEGIN(Engine)

class CVIBuffer_Rect;
class CTexture;
class CShader;

END

BEGIN(Client)

class CUIObject abstract : public CGameObject
{
protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

protected:
	virtual HRESULT Bind_ShaderResources();

	virtual HRESULT Ready_Components();

	virtual void Set_UI_Setting(_float fSizeX, _float fSizeY, _float fPosX, _float fPosY, _float fDepth = 0.9f);

	_bool ClickRange();

protected:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

protected:
	_float m_fSizeX = { 0.f };
	_float m_fSizeY = { 0.f };
	_float m_fPosX = { 0.f };
	_float m_fPosY = { 0.f };

	_float4x4 m_ViewMatrix = {};
	_float4x4 m_ProjMatrix = {};

	CUI_Manager* m_pUI_Manager = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END

