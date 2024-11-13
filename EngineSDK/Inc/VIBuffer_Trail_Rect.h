#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail_Rect final : public CVIBuffer
{
private:
	CVIBuffer_Trail_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Trail_Rect(const CVIBuffer_Trail_Rect& Prototype);
	virtual ~CVIBuffer_Trail_Rect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype_Half();
	virtual HRESULT Initialize(void* pArg) override;

	void CalculateQuad(const _float3& start);

private:
	_bool isInitialized = { FALSE };

public:
	static CVIBuffer_Trail_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	static CVIBuffer_Trail_Rect* Create_Half(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END