#include "stdafx.h"
#include "Particle_Manager.h"
#include "GameInstance.h"
#include "Particle.h"
IMPLEMENT_SINGLETON(CParticle_Manager)

CParticle_Manager::CParticle_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CParticle_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// 각 파티클 타입별로 최소 3개의 파티클 객체를 미리 생성하여 풀에 추가
	for (int i = 0; i < 3; ++i)
	{
		// HEAVY_ATTACK_PARTICLE 타입의 파티클 생성
		CParticle* pParticle = static_cast<CParticle*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Particle_Spread")));
		pParticle->SetActive(false);
		m_ParticlePools[HEAVY_ATTACK_PARTICLE].push_back(pParticle);

	}

	return S_OK;
}

void CParticle_Manager::Camera_Update(_float fTimeDelta)
{
}


void CParticle_Manager::Update(_float fTimeDelta)
{
	for (auto& poolPair : m_ParticlePools)
	{
		auto& pool = poolPair.second;
		for (auto& particle : pool)
		{
			if (particle->IsActive())
			{
				particle->Update(fTimeDelta);
			}
		}
	}
}

void CParticle_Manager::Late_Update(_float fTimeDelta)
{
	for (auto& poolPair : m_ParticlePools)
	{
		auto& pool = poolPair.second;
		for (auto& particle : pool)
		{
			if (particle->IsActive())
			{
				particle->Late_Update(fTimeDelta);
			}
		}
	}
}

HRESULT CParticle_Manager::Render(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CParticle_Manager::Play(PARTICLE_ID eID, const _float3& vPosition)
{
	auto& pool = m_ParticlePools[eID];
	CParticle* pParticle = nullptr;

	//풀의 사이즈가 0이 아닌상황에
	if (pool.size() != 0)
	{
		// 비활성화된 파티클을 풀에서 검색
		for (auto& particle : pool)
		{
			if (!particle->IsActive())
			{
				pParticle = particle;
				break;
			}
		}
	}

	if (pParticle == nullptr)
	{
		// 비활성화된 파티클이 없으면 새로 생성
		switch (eID)
		{
		case HEAVY_ATTACK_PARTICLE:
			pParticle = static_cast<CParticle*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Particle_Spread")));
			pool.push_back(pParticle);
			break;
			// 추후 다른 PARTICLE_ID에 대한 케이스 추가 가능
		}
	}

	 //파티클 활성화 및 초기화
	pParticle->SetActive(true); // 초기에는 비활성화 상태
	pParticle->Set_Position(vPosition);

	// 추가적인 초기화가 필요하면 여기에 구현

	return S_OK;
}


void CParticle_Manager::Free()
{
	Safe_Release(m_pGameInstance);

	for (auto& poolPair : m_ParticlePools)
	{
		auto& pool = poolPair.second;
		for (auto& particle : pool)
		{
			Safe_Release(particle);
		}
		pool.clear();
	}

	__super::Free();
}
