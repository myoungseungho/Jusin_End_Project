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
		PLAYER_SLOT ePlayerSlot = {};
	}SLOT_DESC;

public:
	typedef struct
	{
		_bool		bStun = { FALSE };
		_bool		bHit = { FALSE };
		_bool		bAttBuf = { FALSE };

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
	void Action_AttBuf(_ubyte byKeyID, PLAYER_SLOT eSlot,_float fTimeDelta);
	void Action_Hit(_ubyte byKeyID, _float fStunDuration, _float fTimeDelta);

protected:
	void SkillGaugeLimit();
	void StunRecover(_float fTimeDelta);

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

protected:

	//캐릭터 정보
	_uint					m_iComboCount = { 0 };
	_int					m_iHp = { 0 };
	_int					m_iSKillPoint = { 0 };
	_int					m_iSKillCount = { 0 };

	_bool					m_bStun = { FALSE };
	_bool					m_bHit = { FALSE };
	_bool					m_bAttBuf = { FALSE };

	_uint					m_iNumAttBuf = { 1 };

	//UI에 보내야할 정보
	PAWN_DESC				 m_tPawnDesc = {};

protected:
	PLAYER_SLOT				m_ePlayerSlot = {};

	_float					m_fStunTImer = { 0.f };
	_float					m_fAttBufTimer = { 0.f };

	class CUI_Manager*		m_pUI_Manager = { nullptr };


public:
	virtual CGameObject* Clone(void* pArg) override = 0;
	virtual void Free() override;
};

END