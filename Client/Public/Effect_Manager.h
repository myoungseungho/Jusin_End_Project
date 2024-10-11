#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CModel;
END

BEGIN(Client)

class CEffect_Manager : public CBase
{
	DECLARE_SINGLETON(CEffect_Manager)
private:
	CEffect_Manager();
	virtual ~CEffect_Manager() = default;

public:
	HRESULT Initialize();
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Render(_float fTimeDelta);

public:
	class CEffect_Layer* Find_Effect_Layer(const wstring& strEffectLayerTag);


	HRESULT Delete_Layer(const wstring& strEffectLayerTag);
	vector<wstring> Get_Layer_List();
	HRESULT Add_Effect_To_Layer(_int iCurTestEffectIndex, const wstring& strEffectLayerTag, void* pArg = nullptr);

	HRESULT Add_Test_Effect(EFFECT_TYPE eEffectType, wstring* ModelName);
	HRESULT Delete_Test_Effect(_uint iCurTestEffectID);
	HRESULT Set_Effect_Scaled(_int EffectId, _float3 ChangeScaled);
	HRESULT Set_Effect_Position(_int EffectId, _float3 ChangePosition);
	HRESULT Set_Effect_Rotation(_int EffectId, _float3 ChangeRotation);
	_float3 Get_Effect_Scaled(_int EffectId);
	_float3 Get_Effect_Position(_int EffectId);
	_float3 Get_Effect_Rotation(_int EffectId);
	void Add_KeyFrame(_int EffectId, EFFECT_KEYFRAME NewKeyFrame);

private:
	HRESULT Ready_Components();

public:
	map<const wstring, class CModel*>			m_EffectModel;
	map<const wstring, class CTexture*>		m_EffectTexture;

	map<const wstring, class CEffect_Layer*>		m_FinalEffects;

	vector<class CEffect*>					m_TestEffect;
	_int									m_TestEffect_Count = { 0 };
private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	static CEffect_Manager* Create();
	virtual void Free() override;
};

END