#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CGameInstance;

END

BEGIN(Client)

class CUI_Manager final : public CBase
{
	DECLARE_SINGLETON(CUI_Manager)

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

public:
	void UsingAttckBuff(_bool bAttackBuff) { 
		m_bUsingAttckBuff = bAttackBuff; 
		//m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY , TEXT(""))
	}

public:
	//UsingAttckBuff
	_bool m_bUsingAttckBuff = { FALSE };

	//ScaleAnimation
	_float m_fAnimFrame = { 0.f };
	_float m_fAnimStartTimer = { 0.f };
	

	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END