#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "IMGUI_Shader_Tab.h"
#include "Camera_Free.h"
#include "Monster.h"

//#include "Monster.h"
//#include "Terrain.h"
//#include "Camera.h"
//#include "Player.h"
//#include "Effect.h"
//#include "Sky.h"

//UI ���
#include "UI_Cursor.h"
#include "UI_HpPanel.h"
#include "UI_HpGauge.h"
#include "UI_Chara_Icon_Panel.h"
#include "UI_Chara_Icon.h"
#include "UI_AttBuf.h"
#include "UI_AttBufEffect.h"
#include "UI_AttBufThunderEffect.h"
#include "UI_AttBufNone.h"
#include "UI_AttBufMark.h"
#include "UI_SkillGauge.h"
#include "UI_SkillGaugeBar.h"
#include "UI_SkillNumber.h"
#include "UI_Timer.h"
#include "UI_ComboNumber.h"
#include "UI_ComboFont.h"
#include "UI_ComboEffect.h"

//

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

	Loading_For_UI();

	/* �ؽ��ĸ� �ε��Ѵ�. */
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε� �� �Դϴ�."));

	/* ���� �ε��Ѵ�. */
	lstrcpy(m_szLoadingText, TEXT("��(���� -> ������ -> �޽� -> ��)�� �ε� �� �Դϴ�."));	
	
	_matrix			PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	//�տ���
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_untitled"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/ModelData/Goku_SS3_Rotate0.bin", PreTransformMatrix))))
		return E_FAIL;


	/* ��ü������ �ε��Ѵ�. */
	lstrcpy(m_szLoadingText, TEXT("��ü������ �ε� �� �Դϴ�."));

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_UI()
{
	//�ؽ�ó ������Ʈ

	//Ŀ��

	/* For.Prototype_Component_Texture_UI_Cursor */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Cursor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_dg_ball_Eff00.png")))))
		return E_FAIL;

	//Combo
	/* For.Prototype_Component_Texture_UI_ComboNumber */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ComboNumber"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Left/Number/cp_combo_count_red_%d.png"),10))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_ComboFont */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ComboFont"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Left/cp_combo_hit_red.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_ComboEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ComboEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Left/cp_combo_eff.png")))))
		return E_FAIL;

	//HP

	/* For.Prototype_Component_Texture_UI_HpPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HpPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_gauge_physical_frame01_S3.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_HpGauge*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HpGauge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/HpGauge.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_HpGaugeAnimMask*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HpGaugeAnimMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_lifegauge_animmask.png")))))
		return E_FAIL;

	//�ð� ��

	/* For.Prototype_Component_Texture_UI_Time */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Timer"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/TimerNumber/TimerNum%d.png"), 11))))
		return E_FAIL;


	//��ų ������

	/* For.Prototype_Component_Texture_UI_SKillGauge */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SKillGauge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_tensiontex_base00_S3.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SKillGaugeBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SKillGaugeBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/Cp_Tensiontex/cp_tensiontex_0%d.png"),8))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SPNum */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SPNum"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/SP_Number/SP_Num%d.png"), 8))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SkillFlowEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SkillFlowEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/cp_lifegauge_animmask_S3.png")))))
		return E_FAIL;

	//ĳ���� ������

	/* For.Prototype_Component_Texture_UI_CharIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_CharaIconPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame//Top/Cp_CharaIconPanel.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_CharIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_CharaIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Chara_Image//Face_A/Chara_Icon/CharaIcon%d.png"), 4))))
		return E_FAIL;

	//���� ����

	/* For.Prototype_Component_Texture_UI_AttBufNone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_AttBufNone"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/AttBufEffect/cp_spicon_none.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_AttBuf */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_AttBuf"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/AttBufEffect/cp_spicon0%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_AttBufEff */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_AttBufEff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/AttBufEffect/cp_spicon_eff0%d.png"), 3))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_AttBufMark */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_AttBufMark"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/AttBufEffect/cp_spicon_S.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_AttBufElectricEff */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_AttBufElectricEff"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/3.InGame/AttBufEffect/ElectricEff/cp_spicon_ElectricEff%d.png"), 6))))
		return E_FAIL;


	//���� ������Ʈ


	/* For.Prototype_GameObject_Cursor */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Cursor"),
		CUI_Cursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_HpPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HpPanel"),
		CUI_HpPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Cursor */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_HpGauge"),
		CUI_HpGauge::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Chara_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Chara_Icon_Panel"),
		CUI_Chara_Icon_Panel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Chara_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Chara_Icon"),
		CUI_Chara_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_AttBufMark */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AttBufMark"),
		CUI_AttBufMark::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_AttBufMark */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AttBufNone"),
		CUI_AttBufNone::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_AttBuf */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AttBuf"),
		CUI_AttBuf::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_AttBufEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AttBufEffect"),
		CUI_AttBufEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_AttBufEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_AttBufThunderEffect"),
		CUI_AttBufThunderEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_SkillGauge */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillGauge"),
		CUI_SkillGauge::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_SkillGaugeBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillGaugeBar"),
		CUI_SkillGaugeBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_SkillGauge */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SkillNumber"),
		CUI_SkillNumber::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Timer */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Timer"),
		CUI_Timer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//�޺�

	/* For.Prototype_GameObject_UI_ComboNumber */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ComboNumber"),
		CUI_ComboNumber::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_ComboFont */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ComboFont"),
		CUI_ComboFont::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_ComboEffect */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_ComboEffect"),
		CUI_ComboEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

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
