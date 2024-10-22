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

class CMelee_Effect final : public CGameObject
{
public:
	enum class PLAYERID
	{
		PLAYER_1P, PLAYER_2P
	};

private:
	CMelee_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMelee_Effect(const CMelee_Effect& Prototype);
	virtual ~CMelee_Effect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	HRESULT Ready_Components();

private:
	CCollider* m_pColliderCom = { nullptr };

	_float					m_fX = 0.f;
	_float					m_fY = 0.f;
	PLAYERID				m_playerID = {};

public:
	static CMelee_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END