#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CEffect_Animation : public CBase
{
private:
	CEffect_Animation();
	CEffect_Animation(const CEffect_Animation& Prototype);
	virtual ~CEffect_Animation() = default;

public:
	HRESULT Initialize();

public:
	void		Add_KeyFrame(_uint KeyFrameNumber, EFFECT_KEYFRAME NewKeyFrameData);
	_bool	Find_KeyFrame(_uint KeyFrameNumber);

private:
	class CGameInstance* m_pGameInstance = { nullptr };

	map<_uint, EFFECT_KEYFRAME>	m_EffectKeyFrames;
public:
	static CEffect_Animation* Create();
	virtual void Free() override;
};

END;