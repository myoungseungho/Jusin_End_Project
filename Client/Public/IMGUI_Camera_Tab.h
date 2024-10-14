#pragma once
#include "IMGUI_Tab.h"


BEGIN(Engine)
class CCamera;
END

BEGIN(Client)

class CIMGUI_Camera_Tab : public CIMGUI_Tab
{
public:
	enum MODELID { MODELID_NOT = -1, MODELID_SON, MODELID_HIT, MODELID_MINE, MODELID_21, MODELID_END };
	enum SKILLID { SKILL_NOT = -1, SKILL1, SKILL2, SKILL3, SKILL_END };

private:
	struct pair_hash {
		size_t operator()(const std::pair<MODELID, SKILLID>& key) const {
			return std::hash<int>()(static_cast<int>(key.first)) ^ (std::hash<int>()(static_cast<int>(key.second)) << 1);
		}
	};

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
	void IMGUI_Show_Camera(_float fTimeDelta);
	void Activate_Select_Camera(_int selectedIndex);
	void UpdateCameraSelection();
	void IMGUI_Show_Points();	   // ���� ���� ī�޶��� ����Ʈ�� ������� �����ֱ�
	void IMGUI_Add_Point();        // Add_Point ��ư ó�� �Լ�
	void IMGUI_Play_Button();	   // Play ��ư
	void IMGUI_Delete_Point(_int index); //Delete ��ư
	void IMGUI_Modify_Point(_int index); //Modify ��ư

	MODELID m_iSelected_Model = MODELID_NOT;  // �� ���� ���¸� ����
	SKILLID m_iSelected_Skill = SKILL_NOT;  // ��ų ���� ���¸� ����

	//�𵨰� ��ų pair���� Ű�� �ް� ����ī�޶� �ε����� ������ ����
	unordered_map<pair<MODELID, SKILLID>, _uint, pair_hash> m_CameraIndexMap;

	//�׸� ���ÿ� �����̳�
	//Set�� ������ ����
	//�ڵ����� �ߺ��� ���� ������� �ʽ��ϴ�. ���� ������ �ε����� ���� �� ����Ǵ� ���� ����
	//�׻� ���ĵ� ���¸� �����մϴ�. �̴� Ư�� �ε����� ���������� ó���ϰų� ���ĵ� ���¿��� ������ �ʿ䰡 ���� �� ����
	set<_int> m_selectedPoints; // ���õ� ����Ʈ�� �ε����� ����

	//����ī�޶�
	class CMain_Camera* m_pMainCamera = { nullptr };

public:
	static CIMGUI_Camera_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
