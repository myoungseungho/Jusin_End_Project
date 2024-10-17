#pragma once

#include "Client_Defines.h"
#include "AnimationEvent_Defines.h"
#include "FrameEvent_Manager.h"
#include "Character.h"

#include "Goku_MeleeAttack.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CPlay_Goku final : public CCharacter
{
public:
	typedef struct: CGameObject::GAMEOBJECT_DESC
	{
		_wstring strModelName;
	}Play_Goku_DESC;
	enum AnimationIndex
	{
		ANIME_ATTACK_LIGHT1 = 43, ANIME_ATTACK_LIGHT2 = 44, ANIME_ATTACK_LIGHT3 = 47,
		ANIME_ATTACK_MEDIUM = 46, ANIME_ATTACK_HEAVY = 45,
		ANIME_ATTACK_SPECIAL = 48,

		ANIME_ATTACK_AIR1 = 52, ANIME_ATTACK_AIR2 = 53, ANIME_ATTACK_AIR3 = 54, ANIME_ATTACK_SPECIAL_AIR = 57,

		ANIME_ATTACK_CROUCH_LIGHT = 49, ANIME_ATTACK_CROUCH_MEDUIM = 50, ANIME_ATTACK_CROUCH_HEAVY = 51, ANIME_ATTACK_UPPER_AIR = 55,


		ANIME_ATTACK_236 = 66, ANIME_ATTACK_236_AIR = 67,
		ANIME_ATTACK_214 = 68,

		ANIME_ATTACK_236_SPECIAL = 62, ANIME_ATTACK_236_SPECIAL_UPSIDE = 63,
		ANIME_ATTACK_236_SPECIAL_AIR = 64, ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE = 65,

		ANIME_FINAL_START = 69,

		ANIME_IDLE = 0,
		ANIME_FORWARD_WALK = 9, ANIME_BACK_WALK = 10,
		ANIME_FORWARD_DASH = 11, ANIME_BACK_DASH = 12,
		ANIME_FORWARD_DASH_END = 14, ANIME_BACK_DASH_END = 15,


		ANIME_CROUCH_START = 3, ANIME_CROUCHING = 4,
		ANIME_JUMP_UP = 6, ANIME_JUMP_DOWN = 7,

		ANIME_CHASE = 13

	};
	enum AnimationCount
	{
		COUNT_ATTACK_MEDIUM =0,
		COUNT_ATTACK_CROUCH_MEDUIM,
		COUNT_ATTACK_SPECIAL,
		COUNT_END
	};

private:
	CPlay_Goku(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlay_Goku(const CPlay_Goku& Prototype);
	virtual ~CPlay_Goku() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

	//virtual void NextMoveCheck() override;
	virtual void AttackNextMoveCheck() override;
	virtual void AnimeEndNextMoveCheck() override;

	//virtual void Test_InputCommand();
	//virtual void Set_Animation(_uint iAnimationIndex) override;
	virtual void Set_Animation(_uint iAnimationIndex, _bool bloof = false);

	void KeyTest();


	virtual _bool Check_bCurAnimationisGroundMove(_uint iAnimation = 1000) override;
	virtual _bool Check_bCurAnimationisAttack(_uint iAnimation = 1000) override;
	virtual _bool Check_bCurAnimationisAirAttack(_uint iAnimation = 1000)override;
	
	_bool* Get_pbAttackCount() { return m_bAttackCount; };
	_ushort* Get_piSpecialCount() { return &m_iCountGroundSpecial; };
	virtual void Reset_AttackCount() override;

	virtual void Gravity(_float fTimeDelta) override;

	virtual void AttackEvent(_int iAttackEventEnum, _int AddEvent = 0)override;


private:
	CModel* m_pModelCom_Opening = { nullptr };
	CModel* m_pModelCom_Skill = { nullptr };

	CGoku_MeleeAttack m_tAttackMap;

	_bool m_bAttackCount[COUNT_END] = { true };
	_ushort m_iCountGroundSpecial = 0;


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	
public:
	static CPlay_Goku* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END