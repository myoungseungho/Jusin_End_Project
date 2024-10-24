#pragma once

#include "Base.h"
#include "Renderer_Defines.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
END

BEGIN(Renderer)

class CLight final : public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	LIGHT_DESC* Get_LightDesc()  {
		return &m_LightDesc;
	}

public:
	HRESULT Initialize(const LIGHT_DESC& LightDesc);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer, _int iPassIndex = 1);
	HRESULT Render_Map(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT Render_Player(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	HRESULT Render_Effect(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
private:
	LIGHT_DESC				m_LightDesc{};

public:
	static CLight* Create(const LIGHT_DESC& LightDesc);
	virtual void Free() override;
};

END

/*

����Ʈ ����Ʈ�� ������ �� ���� ����׸��°� ����
�÷��̾��� ����Ʈ�� ���� �׸��� ���
������Ÿ���� ���� ����Ʈ���� �����ؼ� �� ���� �ѹ��� ����������


*/