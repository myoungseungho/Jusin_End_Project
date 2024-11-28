#pragma once

#include "Base.h"
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
END

BEGIN(Client)

class CSubTitle_Manager : public CBase
{
	DECLARE_SINGLETON(CSubTitle_Manager)

public:
	enum SUBTITLE_ID
	{
		SUBTITLE_HELLO,
		SUBTITLE_WELCOME,
		// 필요한 만큼 추가하세요
		SUBTITLE_END
	};

private:
	CSubTitle_Manager();
	virtual ~CSubTitle_Manager() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Camera_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(_float fTimeDelta);

	HRESULT Play(SUBTITLE_ID eID, _float duration);
	void Stop(SUBTITLE_ID eID);

	CGameInstance* m_pGameInstance = { nullptr };

	unordered_map<SUBTITLE_ID, _wstring> m_mapSubTitleText;

	// 현재 표시 중인 자막 정보
	struct SubTitle
	{
		SUBTITLE_ID eID;
		_float fRemainingTime;
	} m_CurrentSubTitle;

	_bool m_bIsDisplaying = false;

public:
	virtual void Free() override;
};

END