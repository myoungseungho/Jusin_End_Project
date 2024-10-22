#pragma once

#include "Client_Defines.h"
#include "AnimationEvent_Defines.h"
#include "FrameEvent_Manager.h"
#include "Character.h"

#include "S21_MeleeAttack.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CPlay_21 final : public CCharacter
{
public:
	typedef struct: CGameObject::GAMEOBJECT_DESC
	{
		_wstring strModelName;
	}Play_21_DESC;
	enum AnimationIndex 
	{
		ANIME_ATTACK_LIGHT1 = 43, ANIME_ATTACK_LIGHT2 = 44, ANIME_ATTACK_LIGHT3 = 45,
		ANIME_ATTACK_MEDIUM = 46, ANIME_ATTACK_HEAVY = 47,
		ANIME_ATTACK_SPECIAL =48,

		ANIME_ATTACK_AIR1 = 53, ANIME_ATTACK_AIR2 = 54, ANIME_ATTACK_AIR3 = 55, //,ANIME_ATTACK_SPECIAL_AIR = 52, 52애니메이션을 16position부터

		ANIME_ATTACK_CROUCH_LIGHT = 49, ANIME_ATTACK_CROUCH_MEDUIM = 50, ANIME_ATTACK_CROUCH_HEAVY = 51, ANIME_ATTACK_UPPER_AIR = 55,
		ANIME_ATTACK_CROUCH_SPECIAL = 52,

		ANIME_ATTACK_236 = 64,  //ANIME_ATTACK_236_AIR = 67,
		ANIME_ATTACK_214 = 61,

		ANIME_ATTACK_236_SPECIAL =63, // ANIME_ATTACK_236_SPECIAL_UPSIDE = 63,

		ANIME_ATTACK_236_SPECIAL_SUCCES = 62,
		//ANIME_ATTACK_214_SPECIAL = 62, // ANIME_ATTACK_236_SPECIAL_UPSIDE = 63,

		ANIME_ATTACK_236_ULTIMATE = 71,  //ANIME_ATTACK_236_SPECIAL_AIR_DOWNSIDE = 65,

		ANIME_FINAL_START = 69,

		ANIME_IDLE = 2, 
		ANIME_FORWARD_WALK = 10, ANIME_BACK_WALK = 11,  
		ANIME_FORWARD_DASH = 12, ANIME_BACK_DASH = 13,
		ANIME_FORWARD_DASH_END = 15, ANIME_BACK_DASH_END = 16,


		ANIME_CROUCH_START = 4, ANIME_CROUCHING = 5,
		ANIME_JUMP_UP = 7, ANIME_JUMP_DOWN = 8


	};
	enum AnimationCount
	{
		COUNT_ATTACK_MEDIUM =0,
		COUNT_ATTACK_CROUCH_MEDUIM,
		COUNT_ATTACK_SPECIAL,
		COUNT_END
	};

private:
	CPlay_21(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlay_21(const CPlay_21& Prototype);
	virtual ~CPlay_21() = default;

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

	virtual void Test_InputCommand();
	//virtual void Set_Animation(_uint iAnimationIndex) override;
	virtual void Set_Animation(_uint iAnimationIndex, _bool bloof = false);

	void KeyTest();


	virtual _bool Check_bCurAnimationisGroundMove(_uint iAnimation = 1000) override;
	virtual _bool Check_bCurAnimationisAttack(_uint iAnimation = 1000) override;

	
	_bool* Get_pbAttackCount() { return m_bAttackCount; };
	_ushort* Get_piSpecialCount() { return &m_iCountGroundSpecial; };
	void Reset_AttackCount();

private:
	CModel* m_pModelCom_Opening = { nullptr };
	CModel* m_pModelCom_Skill = { nullptr };

	CS21_MeleeAttack m_tAttackMap;

	_bool m_bAttackCount[COUNT_END] = { true };
	_ushort m_iCountGroundSpecial = 0;


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


	void Check_Ground();
	
public:
	static CPlay_21* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END