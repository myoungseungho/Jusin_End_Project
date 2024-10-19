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
		case VIRTUAL_CAMERA_SON_SKILL_3:
			name = "Camera_Son_Skill_3";
			break;
		case VIRTUAL_CAMERA_HIT_SKILL_1:
			name = "Camera_Hit_Skill_1";
			break;
		case VIRTUAL_CAMERA_HIT_SKILL_2:
			name = "Camera_Hit_Skill_2";
			break;
		case VIRTUAL_CAMERA_HIT_SKILL_3:
			name = "Camera_Hit_Skill_3";
			break;
		case VIRTUAL_CAMERA_MINE_SKILL_1:
			name = "Camera_Mine_Skill_1";
			break;
		case VIRTUAL_CAMERA_MINE_SKILL_2:
			name = "Camera_Mine_Skill_2";
			break;
		case VIRTUAL_CAMERA_MINE_SKILL_3:
			name = "Camera_Mine_Skill_3";
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

	//stringToSkillID[{1, L"SKILLID_SON_SKILL1"}] = 0;
	//stringToSkillID[{1, L"SKILLID_SON_SKILL2"}] = 1;

	//// MODELID_HIT ��ų �ʱ�ȭ
	//stringToSkillID[{2, L"SKILLID_HIT_SKILL1"}] = 0;

	//// MODELID_MINE ��ų �ʱ�ȭ
	//stringToSkillID[{3, L"SKILLID_MINE_SKILL1"}] = 0;
	//stringToSkillID[{3, L"SKILLID_MINE_SKILL2"}] = 1;

	//// MODELID_21 ��ų �ʱ�ȭ
	//stringToSkillID[{4, L"SKILLID_21_SKILL1"}] = 0;
	//stringToSkillID[{4, L"SKILLID_21_SKILL2"}] = 1;
	//stringToSkillID[{4, L"SKILLID_21_SKILL3"}] = 2;

	//// MODELID_SON ��ų1 �ִϸ��̼� �ʱ�ȭ
	//stringToAnimID[{1, 0, L"ANIMID_SON_SKILL1_ANIM1"}] = 0;
	//stringToAnimID[{1, 0, L"ANIMID_SON_SKILL1_ANIM2"}] = 1;
	//stringToAnimID[{1, 0, L"ANIMID_SON_SKILL1_ANIM3"}] = 2;

	//// MODELID_SON ��ų2 �ִϸ��̼� �ʱ�ȭ
	//stringToAnimID[{1, 1, L"ANIMID_SON_SKILL2_ANIM1"}] = 0;
	//stringToAnimID[{1, 1, L"ANIMID_SON_SKILL2_ANIM2"}] = 1;

	//// MODELID_HIT ��ų1 �ִϸ��̼� �ʱ�ȭ
	//stringToAnimID[{2, 0, L"ANIMID_HIT_SKILL1_ANIM1"}] = 0;
	//stringToAnimID[{2, 0, L"ANIMID_HIT_SKILL1_ANIM2"}] = 1;

	//// MODELID_MINE ��ų1 �ִϸ��̼� �ʱ�ȭ
	//stringToAnimID[{3, 0, L"ANIMID_MINE_SKILL1_ANIM1"}] = 0;
	//stringToAnimID[{3, 0, L"ANIMID_MINE_SKILL1_ANIM2"}] = 1;

	//// MODELID_MINE ��ų2 �ִϸ��̼� �ʱ�ȭ
	//stringToAnimID[{3, 1, L"ANIMID_MINE_SKILL2_ANIM1"}] = 0;
	//stringToAnimID[{3, 1, L"ANIMID_MINE_SKILL2_ANIM2"}] = 1;
	//stringToAnimID[{3, 1, L"ANIMID_MINE_SKILL2_ANIM3"}] = 2;

	//// MODELID_21 ��ų1 �ִϸ��̼� �ʱ�ȭ
	//stringToAnimID[{4, 0, L"ANIMID_21_SKILL1_ANIM1"}] = 0;

	//// MODELID_21 ��ų2 �ִϸ��̼� �ʱ�ȭ
	//stringToAnimID[{4, 1, L"ANIMID_21_SKILL2_ANIM1"}] = 0;
	//stringToAnimID[{4, 1, L"ANIMID_21_SKILL2_ANIM2"}] = 1;

	//// MODELID_21 ��ų3 �ִϸ��̼� �ʱ�ȭ
	//stringToAnimID[{4, 2, L"ANIMID_21_SKILL3_ANIM1"}] = 0;
	//stringToAnimID[{4, 2, L"ANIMID_21_SKILL3_ANIM2"}] = 1;
	//stringToAnimID[{4, 2, L"ANIMID_21_SKILL3_ANIM3"}] = 2;
	//stringToAnimID[{4, 2, L"ANIMID_21_SKILL3_ANIM4"}] = 3;

	return S_OK;
}

void CMain_Camera::Priority_Update(_float fTimeDelta)
{
	//���õ� ����ī�޶� ������Ʈ
	m_vecVirtualCamera[m_currentVirtualMode]->Priority_Update(fTimeDelta);

	//���õ� ����ī�޶��� ������ �� ���� �����
	Update_Camera(m_vecVirtualCamera[m_currentVirtualMode], fTimeDelta);

	//����ī�޶��� �������� ������ ����ī�޶��� �����ϱ�
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
	//���� ����ī�޶� ����ϰ� �ִ� ����ī�޶��� listPoints�� �����´�.
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
		//�𵨸��� ����.
		_int modelID = modelData.modelID;

		//���� ��ų���� ����
		for (const auto& skillData : modelData.skills)
		{
			//��ų���� ī�޶� �ִ�.
			string skillNameStr = skillData.skillName;
			// ���ڿ� ��ȯ�� �ʿ��ϴٸ� WStringToString �Լ��� ����մϴ�.
			_wstring skillName = _wstring(skillNameStr.begin(), skillNameStr.end());

			// ��ų �̸��� ��ų ID�� ����
			_int skillID = -1;
			//auto skillIt = stringToSkillID.find(skillName);
			if (skillIt != stringToSkillID.end())
			{
				skillID = skillIt->second;
			}
			else
				continue;

			// ī�޶� �ε��� ���
			_int cameraIndex = Get_CameraIndex(modelID, skillID);

			CVirtual_Camera* pCurrentCamera = m_vecVirtualCamera[cameraIndex];

			//��ų�� �ִ� �ִϸ��̼�
			for (const auto& animData : skillData.animations)
			{
				string animNameStr = animData.animationName;
				_wstring animName = _wstring(animNameStr.begin(), animNameStr.end());

				// �ִϸ��̼� �̸��� �ִϸ��̼� ID�� ����
				_int animID = -1;
				auto animIt = stringToAnimID.find(animName);
				if (animIt != stringToAnimID.end())
					animID = animIt->second;

				// ���� ����Ʈ �ʱ�ȭ
				pCurrentCamera->m_mapPoints[animID].clear();

				// ���ο� ����Ʈ �Ҵ�
				vector<CameraPoint> points;
				for (const auto& pointData : animData.points)
				{
					CameraPoint point;
					point.position = pointData.position;
					point.rotation = pointData.rotation;
					point.duration = pointData.duration;
					point.interpolationType = static_cast<_int>(pointData.interpolationType);
					point.damping = pointData.damping;
					point.hasWorldFloat4x4 = pointData.hasWorldFloat4x4;
					point.pWorldFloat4x4 = nullptr; // �ʿ信 ���� ����

					points.push_back(point);
				}

				// ����Ʈ ����
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
		else if (skillID == 2)
			index = VIRTUAL_CAMERA_SON_SKILL_3;
	}
	else if (modelID == 2) { // MODELID_HIT
		if (skillID == 0)
			index = VIRTUAL_CAMERA_HIT_SKILL_1;
		else if (skillID == 1)
			index = VIRTUAL_CAMERA_HIT_SKILL_2;
		else if (skillID == 2)
			index = VIRTUAL_CAMERA_HIT_SKILL_3;
	}
	else if (modelID == 3) { // MODELID_HIT
		if (skillID == 0)
			index = VIRTUAL_CAMERA_MINE_SKILL_1;
		else if (skillID == 1)
			index = VIRTUAL_CAMERA_MINE_SKILL_2;
		else if (skillID == 2)
			index = VIRTUAL_CAMERA_MINE_SKILL_3;
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
