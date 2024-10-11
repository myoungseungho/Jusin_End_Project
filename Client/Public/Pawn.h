#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CPawn abstract : public CGameObject
{
public:
	enum PLAYER_SLOT { LPLAYER1, LPLAYER2, RPLAYER1, RPLAYER2, SLOT_END };

public:
	typedef struct
	{
		_bool		bStun = { FALSE };
		_bool		bHit = { 0 };

		_int		iHp = { 0 };
		_uint		iComboCount = { 0 };

		_int		iSKillPoint = { 0 };
		_int		iSKillCount = { 0 };
	}PAWN_DESC;

protected:
	CPawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPawn(const CPawn& Prototype);
	virtual ~CPawn() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

public:
	PAWN_DESC Get_PawnDesc() { return m_tPawnDesc; }

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

protected:

	//UI에 보내야하는 정보
	_uint					m_iComboCount = { 0 };
	_int					m_iHp = { 0 };
	_int					m_iSKillPoint = { 0 };
	_int					m_iSKillCount = { 0 };

	_bool					m_bStun = { FALSE };
	_bool					m_bHit = { FALSE };

	PAWN_DESC				 m_tPawnDesc = {};

protected:
	_float					m_fStunTImer = { 0.f };

	class CUI_Manager* m_pUI_Manager = { nullptr };


public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END