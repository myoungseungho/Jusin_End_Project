#pragma once

#include "Client_Defines.h"
#include "Level.h"



BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;
private:
	/* 내 게임내에 배경을 구성하는 객체들을 추가한다. */
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Lights();
	HRESULT Ready_Character();
	HRESULT Ready_UIObjects();
	void			Create_Effect_Manager();

private:
	class CUI_Manager* m_pUI_Manager = { nullptr };
	class CEffect_Manager* m_pEffect_Manager = { nullptr };
	class CImgui_Manager* m_pIMGUI_Manager = { nullptr };

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END