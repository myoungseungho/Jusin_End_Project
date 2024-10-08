#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "IMGUI_Shader_Tab.h"
#include "Camera_Free.h"

#include "Monster.h"
#include "Model_Preview.h"

//#include "Monster.h"
//#include "Terrain.h"
//#include "Camera.h"
//#include "Player.h"
//#include "Effect.h"
//#include "Sky.h"


CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

/* �� �Լ��� ȣ���� ������� � ��Ȱ? ���ҽ��� �ε��Ѵ�. � ������? */
_uint APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{
	/* � ������ ���� �غ� �ؾ��ϴ��� */
	m_eLevelID = eNextLevelID;

	InitializeCriticalSection(&m_Critical_Section);

	/* ������ ������ �ڿ�(�ؽ���, ����, �� ���)�� �غ��Ѵ�. */
	/* ���ο� �����带 ���� �غ��Ѵ�. ���� �� �ڵ带 �����ϴ� ���ν������ �ε� ȭ��(Update, Render) �� ���� ó���� �Ѵ�. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;
	
	return S_OK;
}

/* �߰������� ������ �����尡 ȣ���� �Լ�. */
HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr{};

	m_isFinished = false;

	switch (m_eLevelID)
	{
	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlayLevel();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}


HRESULT CLoader::Loading_For_GamePlayLevel()
{
	/* �����÷��̷����� �ڿ��� �ε�����. */

	/* �ؽ��ĸ� �ε��Ѵ�. */
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε� �� �Դϴ�."));

	/* ���� �ε��Ѵ�. */
	lstrcpy(m_szLoadingText, TEXT("��(���� -> ������ -> �޽� -> ��)�� �ε� �� �Դϴ�."));	
	
	_matrix			PreTransformMatrix = XMMatrixIdentity();

	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	//�տ��� ������
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_Opening.bin", PreTransformMatrix))))
	//	return E_FAIL;


	//�տ���
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_SS1.bin", PreTransformMatrix))))
	//	return E_FAIL;




	//�ο�
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Bou.bin", PreTransformMatrix))))
	//	return E_FAIL;

	


	//��Ʈ ����ȭ��
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Hit_Select.bin", PreTransformMatrix))))
	//	return E_FAIL;


	//PreTransformMatrix = PreTransformMatrix * XMMatrixRotationX(XMConvertToRadians(180.0f));  //�̰͵� ���� �̻���
	PreTransformMatrix = PreTransformMatrix * XMMatrixRotationX(XMConvertToRadians(180.0f));


	//�տ��� 3��ų
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_SS3.bin", PreTransformMatrix))))
	//	return E_FAIL;



	//��Ʈ
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Hit.bin", PreTransformMatrix))))
	//	return E_FAIL;

	//21ȣ
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Ton.bin", PreTransformMatrix))))
		return E_FAIL;




	//21ȣ ����ȭ��
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Ton_Select.bin", PreTransformMatrix))))
	//	return E_FAIL;

	//�ο� ����ȭ��
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Bou_Select.bin", PreTransformMatrix))))
	//	return E_FAIL;

	//���� ����ȭ��
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_Select.bin", PreTransformMatrix))))
	//	return E_FAIL;


	/* ��ü������ �ε��Ѵ�. */
	lstrcpy(m_szLoadingText, TEXT("��ü������ �ε� �� �Դϴ�."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Model_Preview"),
		CModel_Preview::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_Critical_Section);

	Safe_Release(m_pGameInstance);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
