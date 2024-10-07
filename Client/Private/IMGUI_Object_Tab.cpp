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

	if (ImGui::BeginTabBar("Object_InnerTabs")) { // ���� �� �� ����
		if (ImGui::BeginTabItem("Prototype_ObjectList", &bShowPrototypeObjects)) { // ���� Prototype ��
			if (bShowPrototypeObjects) {
				IMGUI_Show_PrototypeObjects(); // Prototype ������Ʈ ǥ��
			}
			ImGui::EndTabItem(); // ���� Prototype �� ����
		}

		if (ImGui::BeginTabItem("Layer_ObjectList", &bShowLayerObjects)) { // ���� Layer ��
			if (bShowLayerObjects) {
				IMGUI_Show_LayerObjects(); // Layer ������Ʈ ǥ��
			}
			ImGui::EndTabItem(); // ���� Layer �� ����
		}
		ImGui::EndTabBar(); // ���� �� �� ����
	}
}


HRESULT CIMGUI_Object_Tab::IMGUI_Show_PrototypeObjects()
{
	vector<string> objectPrototypes;
	m_pGameInstance->Get_Prototype_Names(&objectPrototypes);
	int selectedItem = -1; // �ʱ� ���� �׸� ����

	static bool showTerrainInput = false;
	static string selectedTerrainType;

	// ����Ʈ �ڽ� ����
	if (ImGui::BeginListBox("##PrototypesListBox", ImVec2(-FLT_MIN, 30 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int i = 0; i < objectPrototypes.size(); i++) {
			if (ImGui::Selectable(objectPrototypes[i].c_str(), selectedItem == i)) {
				selectedItem = i;
				// ������Ʈ ���� ����
				IMGUI_SpawnObjectAtZero(objectPrototypes[i]);

				// ���� ������Ʈ
				m_bIsClickedPrototype = true;
				m_SelectedPrototype = objectPrototypes[i];
				m_pCurrentMovingObject = nullptr; // ���� �̵� ��ü �ʱ�ȭ
			}
		}
		ImGui::EndListBox();
	}

	return S_OK;
}

HRESULT CIMGUI_Object_Tab::IMGUI_SpawnObjectAtZero(const std::string& type)
{
	_uint currentLevel = m_pGameInstance->Get_CurrentLevel_Index();

	// ���ڿ����� "Prototype_GameObject_" ���ξ� ����
	std::string prefix = "Prototype_GameObject_";
	std::string suffix;

	if (type.substr(0, prefix.size()) == prefix) {
		suffix = type.substr(prefix.size());
	}
	else {
		suffix = type; // ���ξ ���ٸ� ��ü Ÿ���� ���
	}

	// Layer �̸� ����
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
	ImGui::Columns(2, NULL, true); // �� ���� �÷� ����

	//���° ���̾ Ŭ���ߴ��� �˱� ����
	//ShowLayerList(���� �г�)���� ���� ��.
	static _int iSelectedItemIndex = -1; // �ʱ� ���� �׸� ����

	//���� ������ �ִ� ���̾���� �̸���, ���̾ �����ִ� GameObject list�� ���� ��������.
	vector<pair<string, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->Add_Object_Layers_Vector(currentLevel, &objectLayersVector);

	//���̾��� �������� ���� Left_Panel�� �̵�
	ShowLayerList(objectLayersVector, iSelectedItemIndex);
	ImGui::NextColumn(); // ���� �÷����� �̵�

	//�� ���̾���� �����ִ� ���� ���ӿ�����Ʈ�� �����ִ� �Լ�
	ShowObjectListAndControls(objectLayersVector, iSelectedItemIndex);

	ImGui::Columns(1); // �� ����

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
