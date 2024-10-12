#pragma once
#include "IMGUI_Tab.h"


BEGIN(Client)

class CIMGUI_Camera_Tab : public CIMGUI_Tab
{

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
	void Open_Select_Camera(_float fTimeDelta);  // 새 창 생성 로직을 래핑한 함수
	void IMGUI_Camera_Select(_float fTimeDelta);

	_int m_iSelected_Model = -1;  // 모델 선택 상태를 저장
	_int m_iSelected_Skill = -1;  // 스킬 선택 상태를 저장

public:
	static CIMGUI_Camera_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
