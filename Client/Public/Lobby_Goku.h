#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
END

BEGIN(Client)

class CLobby_Goku final : public CGameObject
{
private:
	CLobby_Goku(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLobby_Goku(const CLobby_Goku& Prototype);
	virtual ~CLobby_Goku() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	void RotateTowardsTarget(const _float3& vTargetDir, _float fTimeDelta);
	void MoveForward(_float fTimeDelta);
public:
	static CLobby_Goku* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END