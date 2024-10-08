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
	void UsingAttckBuff(_float fAttBufDuration);

public:
	void UI_Setting_Debug(_float& fSizeX , _float& fSizeY , _float& fPosX , _float& fPosY , _float fSizeRadio);
	

public:
	//UsingAttckBuff
	_float m_fDuration = { 0.f };	

	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END