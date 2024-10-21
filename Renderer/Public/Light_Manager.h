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
private:
	CLight_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC* Get_LightDesc(_uint iLightIndex) const;

public:
	HRESULT Initialize();
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

	//void  

private:
	list<class CLight*>				m_Lights;
	// 플레이어 라이트 키값 검색으로 자기 빛 계산 따로 해줘야할듯
	list<class CLight*>				m_PlayerLights;

	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };

public:
	static CLight_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END