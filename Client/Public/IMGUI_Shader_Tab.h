#pragma once
#include "IMGUI_Tab.h"
#include "Effect_Rect.h"
#include "Shader_Texture.h"
BEGIN(Client)

class CIMGUI_Shader_Tab : public CIMGUI_Tab
{
public:
	typedef struct
	{
		_int MoveTex_node_id = { 1500 };
		_float2 fDirection = { 0.f,0.f };
		_float fSpeed = { 0.f };
	}MoveTex_Node;
protected:
	CIMGUI_Shader_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMGUI_Shader_Tab() = default;

public:
	HRESULT Initialize() override;
	void Render(_float fTimeDelta) override;

	void Set_EffectRect(CEffect_Rect* pEffectRect) { m_Effect_Rect = pEffectRect; }
	void Create_NodeTexture(string szPath);
	void Push_ShaderTexture(CShader_Texture* pShaderTexture) { m_NodeTextures.push_back(pShaderTexture); }
	virtual const _char* GetTabName() const { return "Shader"; };
	void Render_MainNode();
	void Render_TextureNode();
	void Render_MoveTexNode();
private:
	vector<CShader_Texture*> m_NodeTextures;
	vector<ImTextureID> m_NodeTextureSRVs;
	CEffect_Rect* m_Effect_Rect = { nullptr };

	_int m_iNodeTextureCount = 0;
	_bool isStart = false;

	vector<pair<_int, _int>> links;

	const _int m_iMain_node_id = -1;	// 메인 노드 id
	_int m_iMain_Input_Diffuse_id = 0;  // 디퓨즈에 연결된 id

	_int m_iAttributeCount = 3; // 노드 Attribute 개수 이게 늘어나면 노드 최대 개수가 줄어듦

	_int unique_node_id = 1;	// 노드 최대 개수 500개
	vector<_int> node_ids;		// 이건 텍스쳐 노드로 쓰자

	_int m_MoveTex_node_id = 1501;	// 
	vector<MoveTex_Node> m_MoveTex_Node_ids;

	//// 실험용
	//map<_int, _int> node_values; 
	//map<_int, _int> input_accumulated_values; 

public:
	static CIMGUI_Shader_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
