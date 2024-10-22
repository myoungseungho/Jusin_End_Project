#include "stdafx.h"
#include "..\Public\Main_Camera.h"
#include "Virtual_Camera.h"
#include "GameInstance.h"

CMain_Camera::CMain_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CMain_Camera::CMain_Camera(const CMain_Camera& Prototype)
	: CCamera{ Prototype }
{

}

HRESULT CMain_Camera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMain_Camera::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_vecVirtualCamera.reserve(VIRTUAL_CAMERA_END);

	_char* name = "";

	for (size_t i = VIRTUAL_CAMERA_NORMAL; i < VIRTUAL_CAMERA_END; i++)
	{
		switch (i)
		{
		case VIRTUAL_CAMERA_NORMAL:
			name = "Camera_Normal";
			break;
		case VIRTUAL_CAMERA_SON_SKILL_1:
			name = "Camera_Son_Skill_1";
			break;
		case VIRTUAL_CAMERA_SON_SKILL_2:
			name = "Camera_Son_Skill_2";
			break;
		case VIRTUAL_CAMERA_HIT_SKILL_1:
			name = "Camera_Hit_Skill_1";
			break;
		case VIRTUAL_CAMERA_MINE_SKILL_1:
			name = "Camera_Mine_Skill_1";
			break;
		case VIRTUAL_CAMERA_MINE_SKILL_2:
			name = "Camera_Mine_Skill_2";
			break;
		case VIRTUAL_CAMERA_21_SKILL_1:
			name = "Camera_21_Skill_1";
			break;
		case VIRTUAL_CAMERA_21_SKILL_2:
			name = "Camera_21_Skill_2";
			break;
		case VIRTUAL_CAMERA_21_SKILL_3:
			name = "Camera_21_Skill_3";
			break;
		}

		CGameObject* virtualCamera_Skill = m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Virtual_Camera"), &name);
		m_vecVirtualCamera.push_back(static_cast<CVirtual_Camera*>(virtualCamera_Skill));
	}

	stringToSkillID["Son_Skill1"] = VIRTUAL_CAMERA_SON_SKILL_1;
	stringToSkillID["Son_Skill2"] = VIRTUAL_CAMERA_SON_SKILL_2;
	stringToSkillID["Hit_Skill1"] = VIRTUAL_CAMERA_HIT_SKILL_1;
	stringToSkillID["Mine_Skill1"] = VIRTUAL_CAMERA_MINE_SKILL_1;
	stringToSkillID["Mine_Skill2"] = VIRTUAL_CAMERA_MINE_SKILL_2;
	stringToSkillID["21_Skill1"] = VIRTUAL_CAMERA_21_SKILL_1;
	stringToSkillID["21_Skill2"] = VIRTUAL_CAMERA_21_SKILL_2;
	stringToSkillID["21_Skill3"] = VIRTUAL_CAMERA_21_SKILL_3;

	stringToAnimID["Son_Skill1_Anim1"] = 0;
	stringToAnimID["Son_Skill1_Anim2"] = 1;
	stringToAnimID["Son_Skill1_Anim3"] = 2;

	stringToAnimID["Son_Skill2_Anim1"] = 0;
	stringToAnimID["Son_Skill2_Anim2"] = 1;

	stringToAnimID["Hit_Skill1_Anim1"] = 0;
	stringToAnimID["Hit_Skill1_Anim2"] = 1;

	stringToAnimID["Mine_Skill1_Anim1"] = 0;
	stringToAnimID["Mine_Skill1_Anim2"] = 1;

	stringToAnimID["Mine_Skill2_Anim1"] = 0;
	stringToAnimID["Mine_Skill2_Anim2"] = 1;
	stringToAnimID["Mine_Skill2_Anim3"] = 2;

	stringToAnimID["21_Skill1_Anim1"] = 0;

	stringToAnimID["21_Skill2_Anim1"] = 0;
	stringToAnimID["21_Skill2_Anim2"] = 1;

	stringToAnimID["21_Skill3_Anim1"] = 0;
	stringToAnimID["21_Skill3_Anim2"] = 1;
	stringToAnimID["21_Skill3_Anim3"] = 2;
	stringToAnimID["21_Skill3_Anim4"] = 3;

	return S_OK;
}

void CMain_Camera::Priority_Update(_float fTimeDelta)
{
	//선택된 가상카메라 업데이트
	m_vecVirtualCamera[m_currentVirtualMode]->Priority_Update(fTimeDelta);

	//선택된 가상카메라의 정보로 뷰 투영 만들기
	Update_Camera(m_vecVirtualCamera[m_currentVirtualMode], fTimeDelta);

	//가상카메라의 포지션의 정보를 메인카메라의 셋팅하기
	_vector position = static_cast<CTransform*>(m_vecVirtualCamera[m_currentVirtualMode]->Get_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, position);
}

void CMain_Camera::Update(_float fTimeDelta)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Update(fTimeDelta);
}

void CMain_Camera::Late_Update(_float fTimeDelta)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Late_Update(fTimeDelta);
}

void CMain_Camera::Set_Virtual_Camera(VIRTUAL_CAMERA mode)
{
	m_currentVirtualMode = mode;
	m_vecVirtualCamera[m_currentVirtualMode]->m_currentMode = CVirtual_Camera::CAMERA_FREE_MODE;
}

void CMain_Camera::Add_Point(_float duration, _int type, const _float4x4* worldMatrixPtr, _float damping, _bool hasWorldFloat4x4, _int animationIndex)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Add_Point(duration, type, worldMatrixPtr, damping, hasWorldFloat4x4, animationIndex);
}

void CMain_Camera::Remove_Point(_int currentIndex, _int animationIndex)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Remove_Point(currentIndex, animationIndex);
}

vector<CameraPoint>& CMain_Camera::Get_VectorPoint(_int index)
{
	//현재 메인카메라가 사용하고 있는 가상카메라의 listPoints를 가져온다.
	return m_vecVirtualCamera[m_currentVirtualMode]->m_mapPoints[index];
}

void CMain_Camera::Play(_int animationIndex)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Start_Play(animationIndex);
}

void CMain_Camera::Stop()
{
	m_vecVirtualCamera[m_currentVirtualMode]->Stop();
}

void CMain_Camera::Pause()
{
	m_vecVirtualCamera[m_currentVirtualMode]->Pause();
}

void CMain_Camera::Button_Stop()
{
	m_vecVirtualCamera[m_currentVirtualMode]->Button_Stop();
}

void CMain_Camera::StartCameraShake(_float fDuration, _float fMagnitude)
{
	m_vecVirtualCamera[m_currentVirtualMode]->StartCameraShake(fDuration, fMagnitude);
}

void CMain_Camera::Move_Point(_int index, _int animationIndex)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Move_Point(index, animationIndex);
}

void CMain_Camera::Modify_Transform(_int index, _int animationIndex)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Modify_Transform(index, animationIndex);
}

void CMain_Camera::ApplyCameraData(CameraSaveData& cameraData)
{
	for (const auto& modelData : cameraData.models)
	{
		// 모델마다 돈다.
		_int modelID = modelData.modelID;

		// 모델의 월드 행렬 포인터를 가져옵니다.
		const _float4x4* pWorldMatrix = nullptr;

		switch (modelID)
		{
		case 1:
			pWorldMatrix = static_cast<CTransform*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Character")->Get_Component(L"Com_Transform"))->Get_WorldMatrixPtr();
			break;
		case 2:
			pWorldMatrix = static_cast<CTransform*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Character")->Get_Component(L"Com_Transform"))->Get_WorldMatrixPtr();
			break;
		case 3:
			pWorldMatrix = static_cast<CTransform*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Character")->Get_Component(L"Com_Transform"))->Get_WorldMatrixPtr();
			break;
		case 4:
			pWorldMatrix = static_cast<CTransform*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Character")->Get_Component(L"Com_Transform"))->Get_WorldMatrixPtr();
			break;
		default:
			// 알 수 없는 모델 ID 처리
			continue;
		}

		// 모델의 스킬마다 돈다
		for (const auto& skillData : modelData.skills)
		{
			// 스킬 이름을 스킬 ID로 매핑
			std::string skillNameStr = skillData.skillName;
			auto skillIt = stringToSkillID.find(skillNameStr);
			if (skillIt == stringToSkillID.end())
			{
				// 스킬이 맵에 없으면 건너뜁니다.
				continue;
			}

			_int skillID = skillIt->second;

			// 카메라 인덱스 얻기
			_int cameraIndex = skillID;
			if (cameraIndex >= m_vecVirtualCamera.size())
			{
				// 유효하지 않은 인덱스면 건너뜁니다.
				continue;
			}

			CVirtual_Camera* pCurrentCamera = m_vecVirtualCamera[cameraIndex];

			// 스킬에 있는 애니메이션
			for (const auto& animData : skillData.animations)
			{
				// 애니메이션 이름을 애니메이션 ID로 매핑
				std::string animNameStr = animData.animationName;
				auto animIt = stringToAnimID.find(animNameStr);
				if (animIt == stringToAnimID.end())
				{
					// 애니메이션이 맵에 없으면 건너뜁니다.
					continue;
				}

				_int animID = animIt->second;

				// 기존 포인트 초기화
				pCurrentCamera->m_mapPoints[animID].clear();

				// 새로운 포인트 할당
				vector<CameraPoint> points;

				for (const auto& pointData : animData.points)
				{
					CameraPoint point = pointData;
					// pWorldFloat4x4 설정
					point.pWorldFloat4x4 = pWorldMatrix;

					// 포인트 벡터에 추가
					points.push_back(point);
				}

				// 포인트 저장
				pCurrentCamera->m_mapPoints[animID] = points;
			}
		}
	}
}

_int CMain_Camera::Get_CameraIndex(_int modelID, _int skillID)
{
	_int index = -1;

	if (modelID == 0)
		index = VIRTUAL_CAMERA_NORMAL;

	else if (modelID == 1) { // MODELID_SON
		if (skillID == 0)
			index = VIRTUAL_CAMERA_SON_SKILL_1;
		else if (skillID == 1)
			index = VIRTUAL_CAMERA_SON_SKILL_2;
	}
	else if (modelID == 2) { // MODELID_HIT
		if (skillID == 0)
			index = VIRTUAL_CAMERA_HIT_SKILL_1;
	}
	else if (modelID == 3) { // MODELID_HIT
		if (skillID == 0)
			index = VIRTUAL_CAMERA_MINE_SKILL_1;
		else if (skillID == 1)
			index = VIRTUAL_CAMERA_MINE_SKILL_2;
	}
	else if (modelID == 4) { // MODELID_HIT
		if (skillID == 0)
			index = VIRTUAL_CAMERA_21_SKILL_1;
		else if (skillID == 1)
			index = VIRTUAL_CAMERA_21_SKILL_2;
		else if (skillID == 2)
			index = VIRTUAL_CAMERA_21_SKILL_3;
	}

	return index;
}

void CMain_Camera::Delete_Points(_int animationIndex)
{
	m_vecVirtualCamera[m_currentVirtualMode]->Delete_Points(animationIndex);
}

void CMain_Camera::SetPosition(_fvector position)
{
	CTransform* virtual_Transform = static_cast<CTransform*>(m_vecVirtualCamera[m_currentVirtualMode]->Get_Component(TEXT("Com_Transform")));
	virtual_Transform->Set_State(CTransform::STATE_POSITION, position);
}

void CMain_Camera::SetPlayer(PLAYER_STATE state, CGameObject* pPlayer)
{
	m_vecVirtualCamera[VIRTUAL_CAMERA_NORMAL]->SetPlayer(state, pPlayer);
}

const char* CMain_Camera::Get_Current_CameraName()
{
	return m_vecVirtualCamera[m_currentVirtualMode]->GetTabName();
}

_bool CMain_Camera::Get_IsPlay()
{
	return m_vecVirtualCamera[m_currentVirtualMode]->Get_IsPlay();
}


HRESULT CMain_Camera::Render(_float fTimeDelta)
{
	return S_OK;
}


CMain_Camera* CMain_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMain_Camera* pInstance = new CMain_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMain_Camera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMain_Camera::Clone(void* pArg)
{
	CMain_Camera* pInstance = new CMain_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMain_Camera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMain_Camera::Free()
{
	for (auto& iter : m_vecVirtualCamera)
		Safe_Release(iter);

	__super::Free();

}
