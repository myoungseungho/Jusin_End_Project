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

	string WStringToUTF8(const std::wstring& wstr);
	wstring UTF8ToWString(const string& utf8Str);
	virtual const _char* GetTabName() const { return "Effect"; };

private:
	class CEffect_Manager* m_pEffect_Manager = { nullptr };
	vector<string> ModelName;
	vector<string> TextureName;

	_bool m_isInitialize = { false };
public:
	static CIMGUI_Effect_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
