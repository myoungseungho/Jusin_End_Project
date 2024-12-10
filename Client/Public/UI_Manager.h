#pragma once

#include "Base.h"
#include "UI_Define.h"
#include "Client_Defines.h"
#include "input.h"

BEGIN(Engine)

class CGameInstance;

END

BEGIN(Client)


class CUI_Manager final : public CBase
{
public:
	enum ThreadPool_For_Loading
	{
		THREAD_TEXTURE_0,
		THREAD_TEXTURE_1,
		THREAD_TEXTURE_2,
		THREAD_MODEL_0,
		THREAD_MODEL_1,
		THREAD_OBJECT_0,
		THREAD_COMPONENT_0,
		THREAD_END
	};

	DECLARE_SINGLETON(CUI_Manager)

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;

	//구조
public:
	void Add_UIOjbect(CUI_Define::UI_LISTPOS ePos ,class CUIObject* pObject);

	//기능
public:
	void GamePlayUpdate(_float fTimeDelta);
	void UsingAttckBuff(CUI_Define::PLAYER_SLOT eSlotID);
	void UsingAttackDestroy(CUI_Define::PLAYER_SLOT eSlotID);

	void UsingSelectCharacher(class CCharacter* pPawn, CUI_Define::PLAYER_SLOT eSlotID) { m_pPawnArray[eSlotID] = pPawn; }
	void UsingChangeCharacher(CUI_Define::PLAYER_SLOT eCurrSlotID);
	void UsingCreateStartUI();
	void UsingCreateEndUI();

	void CutSceneUI(_bool bState);

	void WinUI(LEVELID CreateLevel);
	//void CutSceneFinish();

public:
	//값 초기화
	void InitUIObject();

	//쓰레드 관련
public:
	void AddToQueue(ThreadPool_For_Loading taskType)
	{
		lock_guard<mutex> lock(m_queueMutex);
		m_QueueThreadPool.push(taskType);
	}

	ThreadPool_For_Loading Get_Thread()
	{
		if (m_QueueThreadPool.empty() == FALSE)
		{
		
			ThreadPool_For_Loading TheadID =  m_QueueThreadPool.front();
			m_QueueThreadPool.pop();
			m_iNumThreadFinish++;
			return TheadID;
		}

		return THREAD_END;
	}

private:
	//쓰레드 풀(로딩 때 쓰레드 하나씩 들어올 때마다 Queue에 쌓임)
	queue<ThreadPool_For_Loading> m_QueueThreadPool;
	mutex m_queueMutex;

public:
	//쓰레드가 몇개 완료 됐는지 확인
	_uint m_iNumThreadFinish = { 0 };

private:
	void CreateOption();
	void DestroyOption();

public:
	//다음 로딩을 넘어갈 때 체크
	_bool m_bGamePlayLoadingFinish = { FALSE };
	
public:
	class CCharacter* m_pPawnArray[CUI_Define::SLOT_END] = {nullptr,nullptr ,nullptr ,nullptr };

	//Anim
	_float m_fTotalDuration = { 0.f };

	//Input
	//1P
	DirectionInput	m_eDirInput = {};
	ButtonInput		m_eBtnInput = {};

	//2P
	DirectionInput	m_eDirInput2 = {};
	ButtonInput		m_eBtnInput2 = {};

	//Input Icon
	_uint m_iNumCommandList = { 0 };
	_uint m_iNumCommandList2 = { 0 };

	//RedBlueTimer
	_float m_fColorValue = { 0.f };
	_bool m_bChange[2] = {FALSE , FALSE };
	 
public:
	_bool m_bOnOption = { FALSE };
	CUI_Define::NPC_ID m_eNpcID = {};

public:
	LEVELID m_eLevelID = { LEVEL_END };

public:
	list<class CUIObject*> m_ListTopUI = {};
	list<class CUIObject*> m_ListMidUI = {};
	list<class CUIObject*> m_ListBotUI = {};

private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END