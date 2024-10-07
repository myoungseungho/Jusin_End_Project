#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
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
	class CEffect* Find_EachEffect(const wstring& strEachEffectTag);
	class CEffect_Layer* Find_Effect_Layer(const wstring& strEffectLayerTag);

	HRESULT Add_EachEffect(const wstring& strEachEffectTag, class CEffect* pEachEffect);
	HRESULT Add_Effect_To_Layer(const wstring& strEachEffectTag, const wstring& strEffectLayerTag, void* pArg = nullptr);

private:
	HRESULT Ready_Components();

private:
	// 낱개 이펙트 (프로토타입 같은 역할)
	map<const wstring, class CModel*>		m_EffectModel;

	// 낱개 이펙트 (프로토타입 같은 역할)
	map<const wstring, class CEffect*>		m_EachEffects;

	// 조합된 이펙트들을 보관 > 검색해서 완성된 이펙트를 뱉어냄
	map<const wstring, class CEffect_Layer*>		m_FinalEffects;

private:
	CGameInstance* m_pGameInstance = { nullptr };

public:
	static CEffect_Manager* Create();
	virtual void Free() override;
};

END