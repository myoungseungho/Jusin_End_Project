#pragma once

#include "Base.h"
#include "Renderer_Defines.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
END

BEGIN(Renderer)

class CLight_Manager final : public CBase
{
public:
	enum LIGHT_TYPE { LIGHT_BACKGROUND, LIGHT_PLAYER, LIGHT_EFFECT };
private:
	CLight_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC* Get_LightDesc(LIGHT_TYPE eLightType, _uint iLightIndex, string strName = "") const;

public:
	HRESULT Initialize();
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Add_Player_Light(string strKey, const LIGHT_DESC& LightDesc);
	HRESULT Render_Lights(LIGHT_TYPE eLightType, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer, string strName = "");


	//void  

private:
	list<class CLight*>				m_Lights;
	// 플레이어 라이트 키값 검색으로 자기 빛 계산 따로 해줘야할 수도 있음
	map<string, class CLight*>		m_PlayerLights; // 각 플레이어는 바라보는 정면으로 빛을 받고 있다
	map<string, class CLight*>		m_EffectLights; // 단순히 색이 바뀔 수도 포인트 처리를 할 수도 따로처리해줘야함

	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };

public:
	static CLight_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END