#pragma once

#include "Client_Defines.h"
#include "AnimationEvent_Defines.h"
#include "FrameEvent_Manager.h"
#include "Character.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CPlay_Goku final : public CCharacter
{
public:
	enum AttackGrade {Attack_light =0, Attack_Medium, Attack_Heavy=2, Attack_Special=2, Attack_Command, Attack_Skill, Attack_FinalSkill};
	typedef struct: CGameObject::GAMEOBJECT_DESC
	{
		_wstring strModelName;
	}Play_Goku_DESC;
private:
	CPlay_Goku(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlay_Goku(const CPlay_Goku& Prototype);
	virtual ~CPlay_Goku() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

	virtual void NextMoveCheck();
	virtual void Test_InputCommand();
	virtual void Set_Animation(_uint iAnimationIndex) override;

	void KeyTest();

private:
	
	

	CModel* m_pModelCom_Opening = { nullptr };
	CModel* m_pModelCom_Skill = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	
public:
	static CPlay_Goku* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END