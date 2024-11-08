#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END


BEGIN(Client)

class CQTE_Continuous_Attack final : public CGameObject
{
	enum MISSION_STATE
	{
		MISSION_NOT_DECIDED,
		MISSION_FAILED,
		MISSION_SUCCESS
	};
private:
	CQTE_Continuous_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQTE_Continuous_Attack(const CQTE_Continuous_Attack& Prototype);
	virtual ~CQTE_Continuous_Attack() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Camera_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	void Start_QTE();
	void End_QTE();
	void Handle_QTEInput();
	void Process_Command();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float					m_fSizeX{}, m_fSizeY{}, m_fX{}, m_fY{}, m_fAlpha{};
	_float4x4				m_ViewMatrix{}, m_ProjMatrix{};
	_int m_iTextureNumber = {};

	_bool m_bIsQTEActive = { false }; // QTE 활성화 여부
	_int m_iCharacterSide = { 1 };
	_float m_fTimer = { 0.f };

	//총 플레이 시간
	const _float m_fLifeTime = { 10.f };

	_int m_iCurrentKeyPressCount = 0; // 현재 연타한 횟수
	const _int m_iTargetKeyPressCount = 20; // 목표 연타 횟수

	MISSION_STATE m_eMissionState = MISSION_NOT_DECIDED;

public:
	static CQTE_Continuous_Attack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END