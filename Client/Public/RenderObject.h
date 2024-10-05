#pragma once
#include "GameObject.h"
#include "Client_Defines.h"


BEGIN(Renderer)
class CRenderInstance;
END

BEGIN(Client)

class CRenderObject : public CGameObject
{
protected:
	CRenderObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRenderObject(const CRenderObject& Prototype);
	virtual ~CRenderObject() = default;

protected:
	class Renderer::CRenderInstance* m_pRenderInstance = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
