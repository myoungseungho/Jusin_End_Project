#pragma once
#include "IMGUI_Tab.h"
#include "Camera.h"

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
	void IMGUI_Save_Button();
	void Activate_Select_Camera(_int selectedIndex);
	void UpdateCameraSelection();
	void IMGUI_Show_Points();	   // 현재 가상 카메라의 포인트를 목록으로 보여주기
	void IMGUI_Add_Point();        // Add_Point 버튼 처리 함수
	void IMGUI_Play_Button();	   // Play 버튼
	void IMGUI_Delete_Point(_int index); //Delete 버튼
	void IMGUI_Modify_Point(_int index); //Modify 버튼
	void IMGUI_Modify_Point_UI(_int index);
	void IMGUI_Point_Modify_Save();

	MODELID m_iSelected_Model = MODELID_NOT;  // 모델 선택 상태를 저장
	SKILLID m_iSelected_Skill = SKILL_NOT;  // 스킬 선택 상태를 저장
	_int m_selectedPoint = -1;
	_bool m_isEditing = { false };     // 수정 모드 여부
	//모델과 스킬 pair쌍을 키로 받고 가상카메라 인덱스를 값으로 갖는
	unordered_map<pair<_int, _int>, _uint, pair_hash> m_CameraIndexMap;

	// 임시 데이터 구조체
	struct TempPointData {
		_float duration;
		InterpolationType interpType;
		_float damping = { 1.f };
	};

	TempPointData m_tempPointData;

	//메인카메라
	class CMain_Camera* m_pMainCamera = { nullptr };

public:
	static CIMGUI_Camera_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
