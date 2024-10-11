#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Renderer)
class CRenderer;
END
BEGIN(Client)

class CEffect :	public CGameObject
{
public:
	typedef struct
	{
		_float3 vPosition;
		_float3 vScaled;
		_float3 vRotation;

		_wstring EffectName;

		_wstring ModelName;
		_wstring MaskTextureName;
		_wstring DiffuseTextureName;

		void* SRV_Ptr = { nullptr };
		_int iRenderIndex;

		_int iUnique_Index = -1;

		//CRenderer::RENDERGROUP eRenderType = CRenderer::RG_NONBLEND_TEST;
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

public:
	void Add_KeyFrame(EFFECT_KEYFRAME NewKeyFrame);
	void Set_Effect_Scaled(_float3 ChangeScaled);
	void Set_Effect_Position(_float3 ChangePosition);
	void Set_Effect_Rotation(_float3 ChangeRoation);
	_float3 Get_Effect_Scaled();
	_float3 Get_Effect_Position();
	_float3 Get_Effect_Rotation();


protected:
	EFFECT_TYPE			m_eEffect_Type = { EFFECT_END };

	CShader* m_pShaderCom = { nullptr };
	CModel*	 m_pModelCom = { nullptr };
	CTexture* m_pDiffuseTextureCom = { nullptr };
	CTexture* m_pMaskTextureCom = { nullptr };

	_float		m_fAlpha = { 0.f };

	_float		m_fCurPosition = { 0.f };

	vector<EFFECT_KEYFRAME>	m_EffectKeyFrames;

	_int m_iRenderIndex = { 0 };

public:
	_int m_iUnique_Index = { -1 };
	//class CRenderer::RENDERGROUP m_eRenderType = CRenderer::RG_NONBLEND_TEST;
public:
	_wstring m_EffectName;
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