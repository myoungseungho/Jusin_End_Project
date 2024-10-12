#pragma once
#include "IMGUI_Tab.h"
#include "Effect_Rect.h"
#include "Shader_Texture.h"
BEGIN(Client)

class CIMGUI_Shader_Tab : public CIMGUI_Tab
{
public:
	enum ENDNODE_TYPE { END_DIFFUSE, END_ALPHA };
	enum NODE_TYPE { NODE_BASIC, NODE_FUNCTION, NODE_LINK, NODE_END};
	enum FUNCTION_TYPE { FUNCTION_TEXMOVE, FUNCTION_SPRITE, FUNCTION_END };
public:
	typedef struct
	{
		_int MoveTex_node_id = { 1500 };
		_float2 fDirection = { 0.f,0.f };
		_float fSpeed = { 0.f };
	}MoveTex_Node;

	typedef struct
	{
		_int MoveTex_node_id = { 1500 };
		_float2 fDirection = { 0.f,0.f };
		_float fSpeed = { 0.f };
	}MoveTex_Node;

	typedef struct
	{
		_int iID;
		ImTextureID Texture;
	}SRV_Texture;
protected:
	CIMGUI_Shader_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CTexture* pTexture);
	virtual ~CIMGUI_Shader_Tab() = default;

public:
	HRESULT Initialize() override;
	void Render(_float fTimeDelta) override;
	void TabPos_Init();
	void Set_EffectRect(CEffect_Rect* pEffectRect) { m_Effect_Rect = pEffectRect; }
	void Create_NodeTexture(string szPath);
	void Push_ShaderTexture(CShader_Texture* pShaderTexture) { m_NodeTextures.push_back(pShaderTexture); }
	virtual const _char* GetTabName() const { return "Shader"; };
	void Render_MainNode();
	void Render_TextureNode();
	void Render_MoveTexNode();
	void Render_Link();
	void Check_Create_Link();
	void Check_Delete_Link();
public:
	_int m_iNumberId = { -1 };
	_bool m_TabPick = { false };
private:
	CTexture*					m_pDefaultTexture = { nullptr };			// ���� ���� �Ҷ� �ʿ��� ����ؽ�����
	vector<CShader_Texture*>	m_NodeTextures;								// �ؽ��� ��忡 �ؽ��İ�ü��
	vector<SRV_Texture>			m_NodeTextureSRVs;							// �� �ؽ��İ�ü�� �׸��� ����Ÿ���� �׷��� ��� ShaderReasouceView*

	CTexture*					m_TestEffectModel_Texture = { nullptr };	// ������ ������ �׽�Ʈ����Ʈ��ü�� �ؽ��� ������Ʈ

	CEffect_Rect*				m_Effect_Rect = { nullptr };	// �ӽ÷� ����Ʈ������ �����Ѱ� �ƴ� ��Ʈ

	_int						m_iNodeTextureCount = 0;		// ��� �ؽ����� ����
	_bool						isStart = false;				// �ܺο��� �����ִ� �Ҷ� ���� ����

private: /* ImNodes �� ���õ� ��� ���� */
	vector<pair<_int, _int>>	links;							// ���� ��� ���̸� �̾��ֱ� ���� ���� ( �ܼ��� int�� ID�� ������ ���� )

	const _int					m_iMain_node_id = -1;			// ���� ��� id
	_int						m_iMain_Input_Diffuse_id = 0;	// ��ǻ� ����� id

	_int						m_iAttributeCount = 4;			// ��� Attribute ���� �̰� �þ�� ��� �ִ� ������ �پ��

	_int						unique_node_id = 1;				// ��� �ִ� ���� 500��
	vector<_int>				node_ids;						// �̰� �ؽ��� ���� ����

	_int						m_MoveTex_node_id = 1501;		// �ӽ÷� ������ �����ؽ� ��� ���̵� ������ 
	list<MoveTex_Node>			m_MoveTex_Node_ids;				// �����ؽ� ����

	unordered_map<int, ImVec2>	node_positions;					// �� ��ȯ�� ��� ��ġ�� �ʱ�ȭ�Ǵµ� �װ� �������� �׻� ������ ��ġ�� ��� ����

public:
	static CIMGUI_Shader_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,CTexture* pTexture);
	virtual void Free() override;
};

END
