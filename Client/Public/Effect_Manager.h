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
	class CEffect* Find_Layer_Effect(wstring& layerName, wstring& effectName);
	_bool	Find_KeyFrame(wstring& layerName, wstring& effectName, _uint frameNumber);

	EFFECT_KEYFRAME Get_KeyFrame(wstring& layerName, wstring& effectName, _uint frameNumber);

	HRESULT Delete_Layer(const wstring& strEffectLayerTag);

	vector<wstring> Get_Layer_List();
	vector<wstring> Get_In_Layer_Effect_List(wstring* LayerName);

	HRESULT Add_Effect_To_Layer(_int iCurTestEffectIndex, const wstring& strEffectLayerTag, void* pArg = nullptr);
	HRESULT Add_All_Effect_To_Layer(const wstring& strEffectLayerTag, void* pArg = nullptr);
	HRESULT Add_Test_Effect(EFFECT_TYPE eEffectType, wstring* EffectName, wstring* ModelName);

	HRESULT Delete_Test_Effect(_uint iCurTestEffectID);
	vector<_int> Delete_All_Test_Effect();

	HRESULT Set_Effect_Scaled(_int EffectId, _float3 ChangeScaled);
	HRESULT Set_Effect_Position(_int EffectId, _float3 ChangePosition);
	HRESULT Set_Effect_Rotation(_int EffectId, _float3 ChangeRotation);

	_float3 Get_Effect_Scaled(_int EffectId);
	_float3 Get_Effect_Position(_int EffectId);
	_float3 Get_Effect_Rotation(_int EffectId);

	HRESULT Set_Layer_Effect_Scaled(wstring& layerName, wstring& effectName, _float3 ChangeScaled);
	HRESULT Set_Layer_Effect_Position(wstring& layerName, wstring& effectName, _float3 ChangePosition);
	HRESULT Set_Layer_Effect_Rotation(wstring& layerName, wstring& effectName, _float3 ChangeRotation);
	HRESULT Set_Layer_Effect_IsNotPlaying(wstring& layerName, wstring& effectName, _bool bIsNotPlaying);

	_float3 Get_Layer_Effect_Scaled(wstring& layerName, wstring& effectName);
	_float3 Get_Layer_Effect_Position(wstring& layerName, wstring& effectName);
	_float3 Get_Layer_Effect_Rotation(wstring& layerName, wstring& effectName);
	_bool Get_Layer_Effect_IsPlaying(wstring& layerName, wstring& effectName);

	void Add_KeyFrame(const wstring& LayerName, const wstring& EffectName, _uint KeyFrameNumber, EFFECT_KEYFRAME NewKeyFrame);
	EFFECT_KEYFRAME Get_Layer_Effect_KeyFrame(wstring& layerName, wstring& effectName, _uint KeyFrameNumber);
public:
	HRESULT		Play_Layer_Animation(_float fTimeDelta, const wstring& LayerName);
	HRESULT		Set_Layer_Animation_Position(const wstring& LayerName, _float CurAnimPos);

private:
	HRESULT Ready_Components();

public:
	map<const wstring, class CModel*>			m_EffectModel;
	map<const wstring, class CTexture*>		m_EffectTexture;

	map<const wstring, class CEffect_Layer*>		m_FinalEffects;
	vector<class CEffect*>					m_TestEffect;

public:
	_int									m_TestEffect_Count = { 0 };

private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	static CEffect_Manager* Create();
	virtual void Free() override;
};

END