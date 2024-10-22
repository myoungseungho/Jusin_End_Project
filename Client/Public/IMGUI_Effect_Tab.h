#pragma once
#include "IMGUI_Tab.h"


BEGIN(Client)

class CIMGUI_Effect_Tab : public CIMGUI_Tab
{

protected:
	CIMGUI_Effect_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMGUI_Effect_Tab() = default;

public:
	HRESULT Initialize() override;
	void Render(_float fTimeDelta) override;
	void Push_Initialize();
	void Save_To_Effect_Layer(_uint iCurTestEffectIndex, const wstring& strEffectLayerTag, void* pArg = nullptr);
	HRESULT Save_Effects_File(const wstring& strEffectLayerTag);

	string WStringToUTF8(const std::wstring& wstr);
	wstring UTF8ToWString(const string& utf8Str);
	virtual const _char* GetTabName() const { return "Effect"; };

	void Render_For_Each_Effect();
	void Render_For_Effect_Layer();
	void Render_For_Layer_KeyFrame(_float fTimeDelta);
	void Render_For_Effect_KeyFrame();

private:
	class CEffect_Manager* m_pEffect_Manager = { nullptr };
	vector<string> ModelName;
	vector<string> TextureName;
	vector<EFFECT_LAYER_DATA> m_vecEffectData;
	string EffectLayerKey;

	_bool m_isInitialize = { false };

	_float SelectCurTime = { 0.f };

public:
	static CIMGUI_Effect_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
