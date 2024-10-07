#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

#include "AnimationEvent_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CFrameEvent_Manager final : public CBase
{
	DECLARE_SINGLETON(CFrameEvent_Manager)
private:
	CFrameEvent_Manager();
	virtual ~CFrameEvent_Manager() = default;

public:
	FrameEventMap Get_FrameEventMap() { return FrameEvent; };
	FrameEventMap* Get_pFrameEventMap() { return &FrameEvent; };

	//void Add_Event(_wstring strText);
	//void Add_Event(CHARACTER_INDEX iCharacterIndex, _int iAnimationIndex, _float fPosition, _wstring strText);
	void Add_Event(CHARACTER_INDEX iCharacterIndex, _int iAnimationIndex, _float fPosition, string strText);

	//void LoadFile(CHARACTER_INDEX iCharacterIndex);
	//void LoadFile(const _char* TextFilePath);
	void LoadFile2(const _char* TextFilePath);

	void UseEvent(string strEventText, CGameObject* pGameObject);


private:
	CHARACTER_INDEX Convert_strtoCharacterIndex(string strText);
	_float			Convert_strtoFloat(string strText);

public:
	FrameEventMap FrameEvent{};

public:

	virtual void Free() override;
};

END