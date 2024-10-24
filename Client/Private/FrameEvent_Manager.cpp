#include "stdafx.h"
#include "..\Public\FrameEvent_Manager.h"

#include "RenderInstance.h"
#include "GameInstance.h"


#include <fstream>  
#include <iostream> 
#include <vector>   
#include <string>   
#include <sstream>

#include "Animation.h"
#include "Character.h"
#include "Model_Preview.h"
#include "GameInstance.h"
#include "Main_Camera.h"

IMPLEMENT_SINGLETON(CFrameEvent_Manager)



CFrameEvent_Manager::CFrameEvent_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}


/*
void CFrameEvent_Manager::Add_Event(_wstring strText)
{



	// �и��� ���ڿ��� ������ �迭
	string splitText[10];
	int index = 0;

	// wstringstream�� ����Ͽ� wstring�� ó��
   wstringstream wss(strText);
   wstring temp;

	// ��ǥ�� �������� ���ڿ��� �и��Ͽ� splitText �迭�� ����
	while (std::getline(wss, temp, L',') && index < 10)
	{
		// wstring�� string���� ��ȯ �� ����
		splitText[index] = std::string(temp.begin(), temp.end());
		index++;
	}



	splitText;
	_bool DebugPo = true;

}
*/

/*
void CFrameEvent_Manager::Add_Event(CHARACTER_INDEX iCharacterIndex, _int iAnimationIndex, _float fPosition, _wstring strText)
{
	// �и��� ���ڿ��� ������ �迭
	string splitText[10];
	int index = 0;

	// wstringstream�� ����Ͽ� wstring�� ó��
	wstringstream wss(strText);
	wstring temp;

	// ��ǥ�� �������� ���ڿ��� �и��Ͽ� splitText �迭�� ����
	while (std::getline(wss, temp, L',') && index < 10)
	{
		// wstring�� string���� ��ȯ �� ����
		splitText[index] = std::string(temp.begin(), temp.end());
		index++;
	}



	splitText;
	_bool DebugPo = true;

	FrameEvent[iCharacterIndex][iAnimationIndex][fPosition].push_back("TEST");

}
*/

void CFrameEvent_Manager::Add_Event(CHARACTER_INDEX iCharacterIndex, _int iAnimationIndex, _float fPosition, string strText)
{
	string splitText[10];
	int index = 0;

	// stringstream�� ����Ͽ� string�� ó��
	stringstream ss(strText);
	string temp;

	// ��ǥ�� �������� ���ڿ��� �и��Ͽ� splitText �迭�� ����
	while (std::getline(ss, temp, ',') && index < 10) {
		splitText[index] = temp;
		index++;
	}


	_float fValue[9]{};

	//split[1] �� fValue[0] , split[2]�� fValue[1]...
	for (int i = 1; i < 9; i++)
	{
		if (splitText[i] == "")
			break;

		fValue[i - 1] = Convert_strtoFloat(splitText[i]);

	}


	_bool bDebugPoint = false;


	//if (splitText[0] == "FrameMoveTest")
	//{
	//
	//}


	FrameEvent[iCharacterIndex][iAnimationIndex][fPosition].push_back(strText);

}


/*
void CFrameEvent_Manager::LoadFile(const _char* TextFilePath)
{


	ifstream file(TextFilePath);

	// ������ ����� �������� Ȯ��
	if (!file.is_open())
	{

		MSG_BOX(TEXT("FrameEvevnt_Manager:: LoadFile ���� txt ���� �ε� ����\nPath:")) ;
		return ;
	}

	std::string line;
	// ������ �� ���� ó��
	while (std::getline(file, line))
	{


		//��� ��ǥ�� �и��ϴ� �ڵ�.  ��⿹��

	   // �и��� ���ڿ��� ������ �迭
	   std::string splitText[10];
	   int index = 0;

	   // stringstream�� ����Ͽ� ���� ��ǥ �������� �и�
	   std::stringstream ss(line);
	   std::string temp;

	   while (std::getline(ss, temp, ',') && index < 10)
	   {
		   splitText[index] = temp;
		   index++;
	   }

	   splitText;
	   _bool bDebugTest = true;

	   Add_Event(Convert_strtoCharacterIndex(splitText[0]),  //ĳ���� �̸� ��ȯ
				 Convert_strtoFloat(splitText[1]),           //�ִϸ��̼� �̸� ��ȯ  (����� float)
				 Convert_strtoFloat(splitText[2]),           //���ڸ� float �ִϸ��̼� position���� ��ȯ
				 splitText[3]);





	}

	// ���� �ݱ�
	file.close();

	//return LOAD_SUCCES;
	return ;

}
*/

void CFrameEvent_Manager::LoadFile2(const _char* TextFilePath)
{

	ifstream file(TextFilePath);

	// ������ ����� �������� Ȯ��
	if (!file.is_open())
	{

		MSG_BOX(TEXT("FrameEvevnt_Manager:: LoadFile ���� txt ���� �ε� ����\nPath:"));
		return;
	}

	std::string line;
	// ������ �� ���� ó��
	while (std::getline(file, line))
	{


		// �и��� ���ڿ��� ������ �迭
		std::string splitText[4]; // �ִ� 4���� ������ (3�� ��ǥ + ������)
		int index = 0;

		// stringstream�� ����Ͽ� ���� ��ǥ �������� �и�
		std::stringstream ss(line);
		std::string temp;

		// �ִ� �� ������ ��ǥ�� ������ �������� �״�� ����
		while (index < 3)
		{
			std::getline(ss, temp, ',');
			splitText[index] = temp;
			index++;
		}

		// ��Ʈ���� ���� �κ��� �� ���� ���� (���� ��� �ؽ�Ʈ ��������)
		std::getline(ss, temp); // ���� �κ� �б�
		splitText[3] = temp;

		// ������ ���
		_bool bDebugTest = true;


		//1. �ִϸ��̼��� ��ȣ�� �޴� �ڵ�

	  //Add_Event(Convert_strtoCharacterIndex(splitText[0]),  //ĳ���� �̸� ��ȯ  
	  //    Convert_strtoFloat(splitText[1]),           //�ִϸ��̼� �̸� ��ȯ  (����� float)
	  //    Convert_strtoFloat(splitText[2]),           //���ڸ� float �ִϸ��̼� position���� ��ȯ
	  //    splitText[3]);


		//2.�ִϸ��̼� �̸��� ��ȣ�� �ٲٴ� �Լ� ���.  �ٵ� 1��¥����

		if (splitText[0] == "//")
		{
			continue;
		}
		CHARACTER_INDEX eCharacterIndex = Convert_strtoCharacterIndex(splitText[0]);

		Add_Event(
			eCharacterIndex,  //ĳ���� �̸� ��ȯ  
			Convert_strtoCharacterAnimationIndex(eCharacterIndex, splitText[1]),           //�ִϸ��̼� �̸� ��ȯ  (����� float)
			Convert_strtoFloat(splitText[2]),           //���ڸ� float �ִϸ��̼� position���� ��ȯ
			splitText[3]
		);


		// CHARACTER_INDEX eCharacterIndex = Convert_strtoCharacterIndex(splitText[0]);
		//
		// Add_Event(
		//     eCharacterIndex,  //ĳ���� �̸� ��ȯ  
		//     m_AnimationIndex.Get_AnimationIndex(eCharacterIndex, splitText[1]),           //�ִϸ��̼� �̸� ��ȯ  (����� float)
		//     Convert_strtoFloat(splitText[2]),           //���ڸ� float �ִϸ��̼� position���� ��ȯ
		//     splitText[3]
		// );

	}

	// ���� �ݱ�
	file.close();

	//return LOAD_SUCCES;
	return;

}

void CFrameEvent_Manager::ReLoadFrameEvent(const _char* TextFilePath)
{
	ClearFrameEvent();
	LoadFile2(TextFilePath);

}

void CFrameEvent_Manager::ClearFrameEvent()
{
	FrameEvent.clear();
}


void CFrameEvent_Manager::UseEvent_Test(string strEventText, CGameObject* pGameobject)
{
	string splitText[10];
	int index = 0;

	// stringstream�� ����Ͽ� string�� ó��
	stringstream ss(strEventText);
	string temp;

	// ��ǥ�� �������� ���ڿ��� �и��Ͽ� splitText �迭�� ����
	while (std::getline(ss, temp, ',') && index < 10) {
		splitText[index] = temp;
		index++;
	}


	_float fValue[9]{};

	//split[1] �� fValue[0] , split[2]�� fValue[1]...
	for (int i = 1; i < 9; i++)
	{
		if (splitText[i] == "")
			break;

		fValue[i - 1] = Convert_strtoFloat(splitText[i]);

	}


	_bool bDebugPoint = false;

	if (splitText[0] == "ObjectMove")
	{


		CTransform* pTransform = static_cast<CTransform*>(pGameobject->Get_Component(TEXT("Com_Transform")));

		CModel_Preview* pModelPreview = static_cast<CModel_Preview*>(pGameobject);


		if (nullptr == pTransform)
			return;
		_vector vPos = pTransform->Get_State(CTransform::STATE_POSITION);
		vPos += _vector{ fValue[0] * pModelPreview->Get_iDirection(),fValue[1],fValue[2],fValue[3] };
		pTransform->Set_State(CTransform::STATE_POSITION, vPos);

		//static_cast<CTransform*>(pGameobject->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _vector{ fValue[0],fValue[1],fValue[2],fValue[3]});

		_bool bDebug = false;
	}

	else if (splitText[0] == "TickPerSecondChange")
	{

		CModel* pModel = static_cast<CModel*>(pGameobject->Get_Component(TEXT("Com_Model")));
		pModel->Get_pCurrentAnimation()->m_fTickPerSecond = fValue[0];

	}

	else if (splitText[0] == "AnimSpeedChange")
	{

		CModel* pModel = static_cast<CModel*>(pGameobject->Get_Component(TEXT("Com_Model")));
		pModel->Set_MaxAnimationUpdate_Time(fValue[0]);
		pModel->Get_pCurrentAnimation()->m_fTickPerSecond = fValue[1];

	}

	else if (splitText[0] == "PositionChange")
	{
		CModel* pModel = static_cast<CModel*>(pGameobject->Get_Component(TEXT("Com_Model")));

		//_float fCurPosition = pModel->Get_CurrentAnimationPosition();
		//_float fDelay = (fValue[0] - fCurPosition) / pModel->Get_CurrentAnimationTickPerSecond();
		//pModel->Play_Animation(fDelay);

		pModel->CurrentAnimationPositionJump(fValue[0]);

	}

	else if (splitText[0] == "FlipPlayerDirection")
	{
		CModel_Preview* pModelPreview = static_cast<CModel_Preview*>(pGameobject);
		pModelPreview->FlipDirection(fValue[0]);


	}

}


void CFrameEvent_Manager::UseEvent(string strEventText, CCharacter* pCharacter)
{
	string splitText[10];
	int index = 0;

	// stringstream�� ����Ͽ� string�� ó��
	stringstream ss(strEventText);
	string temp;

	// ��ǥ�� �������� ���ڿ��� �и��Ͽ� splitText �迭�� ����
	while (std::getline(ss, temp, ',') && index < 10) {
		splitText[index] = temp;
		index++;
	}

	_float fValue[9]{};

	for (int i = 1; i < 9; i++)
	{
		if (splitText[i] == "")
			break;

		fValue[i - 1] = Convert_strtoFloat(splitText[i]);
	}

	_bool bDebugPoint = false;

	if (splitText[0] == "ObjectMove")
	{

		CTransform* pTransform = static_cast<CTransform*>(pCharacter->Get_Component(TEXT("Com_Transform")));

		if (nullptr == pTransform)
			return;
		_vector vPos = pTransform->Get_State(CTransform::STATE_POSITION);
		vPos += _vector{ fValue[0] * pCharacter->Get_iDirection(),fValue[1],fValue[2],fValue[3] };
		pTransform->Set_State(CTransform::STATE_POSITION, vPos);

		//static_cast<CTransform*>(pGameobject->Get_Component(TEXT("Com_Transform")))->Set_State(CTransform::STATE_POSITION, _vector{ fValue[0],fValue[1],fValue[2],fValue[3]});

		_bool bDebug = false;

		//����ȭ�Ϸ��� ���⼭ float���� return�ϰ� �ϴ� ��ģ �� �ѹ��� ó���ϱ�?
	}

	else if (splitText[0] == "TickPerSecondChange")
	{

		CModel* pModel = static_cast<CModel*>(pCharacter->Get_Component(TEXT("Com_Model")));
		pModel->Get_pCurrentAnimation()->m_fTickPerSecond = fValue[0];

	}

	else if (splitText[0] == "AnimSpeedChange")
	{

		CModel* pModel = static_cast<CModel*>(pCharacter->Get_Component(TEXT("Com_Model")));
		pModel->Set_MaxAnimationUpdate_Time(fValue[0]);
		pModel->Get_pCurrentAnimation()->m_fTickPerSecond = fValue[1];

	}

	else if (splitText[0] == "PositionChange")
	{
		CModel* pModel = static_cast<CModel*>(pCharacter->Get_Component(TEXT("Com_Model")));
		//_float fCurPosition = pModel->Get_CurrentAnimationPosition();
		//_float fDelay = (fValue[0] - fCurPosition) / pModel->Get_CurrentAnimationTickPerSecond();
		//pModel->Play_Animation(fDelay);

		pModel->CurrentAnimationPositionJump(fValue[0]);
	}

	else if (splitText[0] == "NextAnimationCheck")
	{
		pCharacter->AttackNextMoveCheck();
	}
	else if (splitText[0] == "AttackEvent")
	{
		pCharacter->AttackEvent(fValue[0], fValue[1]);
	}
	else if (splitText[0] == "SetAnimation")
	{
		pCharacter->Set_Animation(fValue[0]);
	}
	else if (splitText[0] == "SetNextAnimation")
	{
		pCharacter->Set_NextAnimation(fValue[0], fValue[1]);
		//pCharacter->Set_Animation(fValue[0]);
	}
	else if (splitText[0] == "FlipPlayerDirection")
	{
		pCharacter->FlipDirection();
	}
	else if (splitText[0] == "EnemyChase")
	{
		pCharacter->Teleport_ToEnemy(fValue[0], fValue[1]);
	}
	else if (splitText[0] == "DebugPoint")
	{
		_int iDebug = pCharacter->Get_iDirection();
		pCharacter->FlipDirection();
		_bool bDebug = true;
	}

	// �������� UseEvent_MSH �Լ� ȣ��
	UseEvent_MSH(splitText, fValue, pCharacter);
}

void CFrameEvent_Manager::UseEvent_MSH(string splitText[], _float fValue[], CCharacter* pCharacter)
{
	// StartCameraShake ���� ó��
	if (splitText[0] == "StartCameraShake")
	{
		CGameObject* cameraObject = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Main_Camera"));
		CMain_Camera* main_Camera = static_cast<CMain_Camera*>(cameraObject);
		main_Camera->StartCameraShake(fValue[0], fValue[1]);
	}

	// �߰������� �ٸ� MSH ���� �̺�Ʈ�� ���⼭ ó�� ����
	// �ٸ� �̺�Ʈ�� �߰��ϰ� ������ �̰��� �߰��ϸ� �˴ϴ�.
}

void CFrameEvent_Manager::Initalize_NameMap()
{
	if (m_bInitalizeCount == false)
		m_AnimationIndex.Initalize();
}

CHARACTER_INDEX CFrameEvent_Manager::Convert_strtoCharacterIndex(string strText)
{
	/*
   const static enum CHARACTER_INDEX {
		SELECT_GOKU, SELECT_BOU, SELECT_21, SELECT_HIT,
		PLAY_GOKU, PLAY_BOU, PLAY_21, PLAY_HIT, CHARACTER_INDEX_END
	};
   */

	if (strText == "SELECT_GOKU")
		return SELECT_GOKU;

	if (strText == "SELECT_BOU")
		return SELECT_BOU;

	if (strText == "SELECT_21")
		return SELECT_21;

	if (strText == "SELECT_HIT")
		return SELECT_HIT;



	if (strText == "PLAY_GOKU")
		return PLAY_GOKU;

	if (strText == "PLAY_BOU")
		return PLAY_BOU;

	if (strText == "PLAY_21")
		return PLAY_21;

	if (strText == "PLAY_HIT")
		return PLAY_HIT;


	if (strText == "OPENING_GOKU")
		return OPENING_GOKU;


	if (strText == "OPENIING_21")
		return OPENIING_21;


	if (strText == "SKILL_GOKU")
		return SKILL_GOKU;




	return CHARACTER_INDEX_END;
}

_float CFrameEvent_Manager::Convert_strtoFloat(string strText)
{
	_float floatValue;

	stringstream ss(strText);
	ss >> floatValue;

	return floatValue;
}

_int CFrameEvent_Manager::Convert_strtoCharacterAnimationIndex(CHARACTER_INDEX eCharacterIndex, string strText)
{
	return m_AnimationIndex.Get_AnimationIndex(eCharacterIndex, strText);
}

void CFrameEvent_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
