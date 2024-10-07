#pragma once

#include "Client_Defines.h"
#include "Base.h"

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
	// ���� ����Ʈ (������Ÿ�� ���� ����)
	map<const wstring, class CEffect*>		m_EachEffects;

	// ���յ� ����Ʈ���� ���� > �˻��ؼ� �ϼ��� ����Ʈ�� ��
	map<const wstring, class CEffect_Layer*>		m_FinalEffects;

public:
	static CEffect_Manager* Create();
	virtual void Free() override;
};

END