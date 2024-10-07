#include "stdafx.h"
#include "IMGUI_Object_Tab.h"
#include "GameInstance.h"
#include "RenderInstance.h"
#include <codecvt>
#include "Transform.h"


CIMGUI_Object_Tab::CIMGUI_Object_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CIMGUI_Tab{ pDevice,pContext }
{
}

HRESULT CIMGUI_Object_Tab::Initialize()
{
	return S_OK;
}

void CIMGUI_Object_Tab::Render(_float fTimeDelta)
{
	static bool bShowPrototypeObjects = true;
	static bool bShowLayerObjects = true;

	if (ImGui::BeginTabBar("Object_InnerTabs")) { // 하위 탭 바 시작
		if (ImGui::BeginTabItem("Prototype_ObjectList", &bShowPrototypeObjects)) { // 하위 Prototype 탭
			if (bShowPrototypeObjects) {
				IMGUI_Show_PrototypeObjects(); // Prototype 오브젝트 표시
			}
			ImGui::EndTabItem(); // 하위 Prototype 탭 종료
		}

		if (ImGui::BeginTabItem("Layer_ObjectList", &bShowLayerObjects)) { // 하위 Layer 탭
			if (bShowLayerObjects) {
				IMGUI_Show_LayerObjects(); // Layer 오브젝트 표시
			}
			ImGui::EndTabItem(); // 하위 Layer 탭 종료
		}
		ImGui::EndTabBar(); // 하위 탭 바 종료
	}
}


HRESULT CIMGUI_Object_Tab::IMGUI_Show_PrototypeObjects()
{
	vector<string> objectPrototypes;
	m_pGameInstance->Get_Prototype_Names(&objectPrototypes);
	int selectedItem = -1; // 초기 선택 항목 없음

	static bool showTerrainInput = false;
	static string selectedTerrainType;

	// 리스트 박스 시작
	if (ImGui::BeginListBox("##PrototypesListBox", ImVec2(-FLT_MIN, 30 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int i = 0; i < objectPrototypes.size(); i++) {
			if (ImGui::Selectable(objectPrototypes[i].c_str(), selectedItem == i)) {
				selectedItem = i;
				// 오브젝트 생성 로직
				IMGUI_SpawnObjectAtZero(objectPrototypes[i]);

				// 상태 업데이트
				m_bIsClickedPrototype = true;
				m_SelectedPrototype = objectPrototypes[i];
				m_pCurrentMovingObject = nullptr; // 현재 이동 객체 초기화
			}
		}
		ImGui::EndListBox();
	}

	return S_OK;
}

HRESULT CIMGUI_Object_Tab::IMGUI_SpawnObjectAtZero(const std::string& type)
{
	_uint currentLevel = m_pGameInstance->Get_CurrentLevel_Index();

	// 문자열에서 "Prototype_GameObject_" 접두어 제거
	std::string prefix = "Prototype_GameObject_";
	std::string suffix;

	if (type.substr(0, prefix.size()) == prefix) {
		suffix = type.substr(prefix.size());
	}
	else {
		suffix = type; // 접두어가 없다면 전체 타입을 사용
	}

	// Layer 이름 생성
	std::string layerName = "Layer_" + suffix;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring converted = converter.from_bytes(type);
	std::wstring wLayerName = converter.from_bytes(layerName);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, converted.c_str(), wLayerName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CIMGUI_Object_Tab::IMGUI_Show_LayerObjects()
{
	_uint currentLevel = m_pGameInstance->Get_CurrentLevel_Index();
	ImGui::Columns(2, NULL, true); // 두 개의 컬럼 생성

	//몇번째 레이어를 클릭했는지 알기 위함
	//ShowLayerList(왼쪽 패널)에서 갱신 함.
	static _int iSelectedItemIndex = -1; // 초기 선택 항목 없음

	//현재 레벨에 있는 레이어들의 이름과, 레이어에 속해있는 GameObject list로 각각 가져오기.
	vector<pair<string, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->Add_Object_Layers_Vector(currentLevel, &objectLayersVector);

	//레이어의 정보들을 왼쪽 Left_Panel에 이동
	ShowLayerList(objectLayersVector, iSelectedItemIndex);
	ImGui::NextColumn(); // 다음 컬럼으로 이동

	//각 레이어들이 갖고있는 개별 게임오브젝트를 보여주는 함수
	ShowObjectListAndControls(objectLayersVector, iSelectedItemIndex);

	ImGui::Columns(1); // 열 병합

	return S_OK;
}

HRESULT CIMGUI_Object_Tab::IMGUI_Save_Button_Pressed(_bool*, _bool*)
{
	return E_NOTIMPL;
}

void CIMGUI_Object_Tab::ShowLayerList(const vector<pair<string, list<CGameObject*>>>& objectLayersVector, _int& selectedItem)
{
}

void CIMGUI_Object_Tab::ShowObjectListAndControls(const vector<pair<string, list<CGameObject*>>>& objectLayersVector, _int selectedItem)
{
}

void CIMGUI_Object_Tab::HandleContainerObject(CContainerObject* containerObject)
{
}

void CIMGUI_Object_Tab::HandleAABBColliderObject(CGameObject* containerObject)
{
}

void CIMGUI_Object_Tab::HandleObjectPosition(CGameObject* _pSelectedGameObject)
{
}

void CIMGUI_Object_Tab::HandleColliderTransform(CCollider* _pCollider)
{
}

void CIMGUI_Object_Tab::HandleObjectRotation(CGameObject* _pSelectedGameObject)
{
}

void CIMGUI_Object_Tab::HandleObjectScale(CGameObject* _pSelectedGameObject)
{
}

void CIMGUI_Object_Tab::HandleObjectManagement(unordered_set<CGameObject*>& container, CGameObject* _pSelectedGameObject, const string& layerName)
{
}

void CIMGUI_Object_Tab::HandleMultipleObjectControl(const unordered_set<CGameObject*>& _hashSet_selectedGameObjects)
{
}

CIMGUI_Object_Tab* CIMGUI_Object_Tab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIMGUI_Object_Tab* pInstance = new CIMGUI_Object_Tab(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CIMGUI_Object_Tab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIMGUI_Object_Tab::Free()
{
	__super::Free();
}
