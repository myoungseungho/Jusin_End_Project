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

	const _int m_iMain_node_id = -1;	// ���� ��� id
	_int m_iMain_Input_Diffuse_id = 0;  // ��ǻ� ����� id

	_int m_iAttributeCount = 3; // ��� Attribute ���� �̰� �þ�� ��� �ִ� ������ �پ��

	_int unique_node_id = 1;	// ��� �ִ� ���� 500��
	vector<_int> node_ids;		// �̰� �ؽ��� ���� ����

	_int m_MoveTex_node_id = 1501;	// 
	vector<MoveTex_Node> m_MoveTex_Node_ids;

	//// �����
	//map<_int, _int> node_values; 
	//map<_int, _int> input_accumulated_values; 

public:
	static CIMGUI_Shader_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
