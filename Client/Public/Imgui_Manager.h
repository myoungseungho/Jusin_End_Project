#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "Renderer_Defines.h"
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

private:

	CImgui_Manager();
	virtual ~CImgui_Manager() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);
	
	class CIMGUI_Shader_Tab* Access_Shader_Tab() { return m_vecShader_Tabs[0]; }
	void Push_Shader_Tab(CTexture* pTexture);

private:
	void Render_IMGUI(_float fTimeDelta);

	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CRenderInstance* m_pRenderInstance = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };

	vector<class CIMGUI_Tab*> m_vecTabs;
	vector<class CIMGUI_Shader_Tab*> m_vecShader_Tabs;
public:
	virtual void Free() override;
};

END