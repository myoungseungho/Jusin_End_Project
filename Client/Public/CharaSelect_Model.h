#pragma once

#include "Client_Defines.h"
#include "UI_Define.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CModel;
class CTexture;

END

BEGIN(Client)

class CCharaSelect_Model final : public CGameObject
{
public:
	struct typedef
	{
		CUI_Define::PLAYER_ID ePlayerID = {};
	}SELECT_MODEL;

private:
	CCharaSelect_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharaSelect_Model(const CCharaSelect_Model& Prototype);
	virtual ~CCharaSelect_Model() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CTexture* m_pLimTexture = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CCharaSelect_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END