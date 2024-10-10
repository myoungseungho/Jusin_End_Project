#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CEffect :	public CGameObject
{
public:
	typedef struct
	{
		_wstring ModelName;
		_wstring MaskTextureName;
		_wstring DiffuseTextureName;
	}EFFECT_DESC;

protected:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CGameObject& Prototype);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;


protected:
	EFFECT_TYPE			m_eEffect_Type = { EFFECT_END };

	CShader* m_pShaderCom = { nullptr };
	CModel*	 m_pModelCom = { nullptr };
	CTexture* m_pDiffuseTextureCom = { nullptr };
	CTexture* m_pMaskTextureCom = { nullptr };

	_float		m_fDuration = { 0.f };
	_float		m_fAlpha = { 0.f };

	_float		m_fCurPosition = { 0.f };

	vector<EFFECT_KEYFRAME>	m_EffectKeyFrames;

public:
	_wstring m_ModelName;
	_wstring m_MaskTextureName;
	_wstring m_DiffuseTextureName;

protected:
	virtual HRESULT Ready_Components(_wstring* pModelName, _wstring* pMaskTextureName, _wstring* pDiffuseTexturueName);
	virtual HRESULT Bind_ShaderResources();

public:
	static CEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END