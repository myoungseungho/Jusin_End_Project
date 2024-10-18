#pragma once
#include "IMGUI_Tab.h"
#include "Camera.h"
#include "Virtual_Camera.h"
BEGIN(Engine)
class CCamera;
END

BEGIN(Client)

class CIMGUI_Camera_Tab : public CIMGUI_Tab
{
	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator()(const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
		}
	};



	//���� ����������
	enum CAMERA_MODELID { MODELID_NOT = -1, MODELID_DEFAULT, MODELID_SON, MODELID_HIT, MODELID_MINE, MODELID_21, MODELID_END };
	//��ų�� �� �𵨸��� enum�� �޶� map
protected:
	CIMGUI_Camera_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMGUI_Camera_Tab() = default;

public:
	HRESULT Initialize() override;
	void Render(_float fTimeDelta) override;
	virtual const _char* GetTabName() const { return "Camera"; };

private:
	void IMGUI_Camera_Select_Model(_float fTimeDelta);
	void IMGUI_Camera_Select_Skill(_float fTimeDelta);
	void IMGUI_Camera_Select_Animation(_float fTimeDelta);

	void IMGUI_Save_Button();
	void UpdateCameraSelection();
	void IMGUI_Show_Points();	   // ���� ���� ī�޶��� ����Ʈ�� ������� �����ֱ�
	void IMGUI_Button();        // Add_Point ��ư ó�� �Լ�
	void IMGUI_Delete_Points(_int animationIndex);
	void IMGUI_Play_Button();	   // Play ��ư
	void IMGUI_Pause_Button();	   // Pause ��ư
	void IMGUI_Stop_Button();	   // Stop ��ư

	void IMGUI_Delete_Point(_int index, _int animationIndex); //Delete ��ư
	void IMGUI_Modify_Point(_int index); //Modify ��ư
	void IMGUI_Modify_Point_UI(_int index);
	void IMGUI_Point_Modify_Save();

	const _float4x4* Get_Model_Float4x4();

private:
	CAMERA_MODELID m_iSelected_Model = MODELID_DEFAULT;  // �� ���� ���¸� ����
	_int  m_iSelected_Skill = -1;
	_int m_iSelected_Animation = -1; // �ִϸ��̼� ���� ���¸� ���� (�ε��� ���)

	// �𵨺� ��ų ���
	unordered_map<CAMERA_MODELID, vector<string>> m_ModelSkills;
	// �� �̸� �迭 ����
	const char* MODEL_NAMES[MODELID_END];

	// ��ų�� �ִϸ��̼� �ε��� ���� Ű�� �ް� �ִϸ��̼� ����� ������ ���� ��
	unordered_map<pair<CAMERA_MODELID, int>, vector<string>, pair_hash> m_SkillAnimations;

























	_int m_selectedPoint = -1;
	_bool m_isEditing = { false };     // ���� ��� ����
	_bool m_isCompleteCameraSelect = { true };
	//�𵨰� ��ų pair���� Ű�� �ް� ����ī�޶� �ε����� ������ ����
	unordered_map<pair<CAMERA_MODELID, _int>, _uint, pair_hash> m_CameraIndexMap;


	// �ӽ� ������ ����ü
	struct TempPointData {
		_float duration;
		CVirtual_Camera::InterpolationType interpType;
		_float damping = { 1.f };
	};

	TempPointData m_tempPointData;

	//����ī�޶�
	class CMain_Camera* m_pMainCamera = { nullptr };

public:
	static CIMGUI_Camera_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
