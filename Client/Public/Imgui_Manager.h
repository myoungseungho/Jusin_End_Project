#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "Renderer_Defines.h"

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
	CImgui_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CRenderInstance* pRenderInstance);
	virtual ~CImgui_Manager() = default;

public:
	virtual HRESULT Initialize();
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

private:
	void Render_IMGUI(_float fTimeDelta);

	//IMGUI
private:
	HRESULT IMGUI_Show_Animation();
	HRESULT IMGUI_Show_Effect();
	HRESULT IMGUI_Show_UI();
	HRESULT IMGUI_Show_Shader();

	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CRenderInstance* m_pRenderInstance = { nullptr };

public:
	static CImgui_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CRenderInstance* pRenderInstance);
	virtual void Free() override;
};

END