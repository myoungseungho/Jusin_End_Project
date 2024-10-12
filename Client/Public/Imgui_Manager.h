#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "Renderer_Defines.h"

#include "imgui.h"
#include "Effect.h"
BEGIN(Engine)
class CGameInstance;
END

BEGIN(Renderer)
class CRenderInstance;
END


BEGIN(Client)

class CImgui_Manager : public CBase
{
	DECLARE_SINGLETON(CImgui_Manager)

public:
	typedef struct
	{
		ImVec2 MainImGuiPos = { 0.f,0.f };
		ImVec2 MainImGuiSize = { 0.f,0.f };

		ImVec2 ShaderImGuiPos = { 0.f,0.f };
		ImVec2 ShaderImGuiSize = { 0.f,0.f };
	}IMGUI_SCREEN;
private:

	CImgui_Manager();
	virtual ~CImgui_Manager() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

	class CIMGUI_Shader_Tab* Access_Shader_Tab() { return m_vecShader_Tabs[to_string(m_iCurShaderTabIndex)]; }
	void Push_Shader_Tab(CTexture* pTexture);
	void Delete_Shader_Tab(_int iIndex);

	_int Get_CurShaderTab_Index() { return m_iCurShaderTabIndex; }
	_int Get_CurShaderTab_Id() { return m_iCurShaderTabId; }
	_int Pick_Effect_Mesh();

	IMGUI_SCREEN Get_Screen_Desc() {
		return m_ImGuiScreen;
	}
	
private:
	void Render_IMGUI(_float fTimeDelta);
	void Render_ShaderTabs(_float fTimeDelta);
	void Render_EffectAnimationTabs(_float fTimeDelta);

	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CRenderInstance* m_pRenderInstance = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };

	vector<class CIMGUI_Tab*> m_vecTabs;
	map<string, class CIMGUI_Shader_Tab*> m_vecShader_Tabs;

	IMGUI_SCREEN m_ImGuiScreen;

	_int m_iShaderCount = { 0 };
	_int m_iCurShaderTabIndex = { -1 };
	_int m_iCurShaderTabId = { -1 };
public:
	virtual void Free() override;
};

END