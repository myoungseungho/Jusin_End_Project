#include "stdafx.h"
#include "..\Public\Camera_Free.h"

#include "GameInstance.h"
#include "Player.h"
#include "UIManager.h"
#include "UI_MiniMap.h"
#include "Alpha_Enemy.h"
#include "Hyunwoo_Enemy.h"
CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
	: CCamera{ Prototype }
{

}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	m_fMouseSensor = static_cast<CCamera_Free::CAMERA_FREE_DESC*>(pArg)->fSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVectorSet(0.86f, 0.f, 0.49f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVectorSet(-0.385f, 0.62f, 0.67f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVectorSet(-0.3f, -0.78f, 0.54f, 0.f));

	InitializeCameraPoints();

	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{


	__super::Priority_Update(fTimeDelta);
}

void CCamera_Free::Update(_float fTimeDelta)
{
	// ���� ��忡 ���� ī�޶� ������Ʈ
	switch (m_currentMode)
	{
	case CAMERA_DEFAULT_MODE:
		PlayerCamera();
		break;
	case CAMERA_FREE_MODE:
		FreeCamera(fTimeDelta);
		break;
	case CAMERA_CINEMATIC_MODE:
		CinematicCamera(fTimeDelta);
		break;
	}

	// ��鸲 ȿ�� ����
	if (m_bIsShaking)
	{
		ApplyCameraShake(fTimeDelta);
	}
}


void CCamera_Free::Late_Update(_float fTimeDelta)
{
}

HRESULT CCamera_Free::Render(_float fTimeDelta)
{
	return S_OK;
}

void CCamera_Free::StartCameraShake(_float fDuration, _float fMagnitude)
{
	m_bIsShaking = true;
	m_fShakeDuration = fDuration;
	m_fShakeMagnitude = fMagnitude;
	m_fElapsedShakeTime = 0.0f;
}

void CCamera_Free::StopCameraShake()
{
	m_bIsShaking = false;
	m_fShakeDuration = 0.0f;
	m_fElapsedShakeTime = 0.0f;
	m_fShakeMagnitude = 0.0f;
	m_vShakeOffset = XMVectorZero();
}

void CCamera_Free::StartCinematic_Mode(const wstring& cinematicKey, CGameObject* pTargetObject)
{
	// ���޹��� Ű�� �ó׸�ƽ�� ã���ϴ�.
	auto it = m_Cinematics.find(cinematicKey);
	if (it != m_Cinematics.end())
	{
		m_pCurrentCinematicPoints = &(it->second);
		m_CurrentCinematicKey = cinematicKey;

		// ��� ��ü ����
		if (pTargetObject != nullptr)
			m_pTargetObject = pTargetObject;
		else
			m_pTargetObject = m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player");

		// �ó׸�ƽ �ʱ�ȭ ������ ����
		m_bCinematicInitialized = false;
		m_CurrentIndex = 0;
		m_fElapsedTime = 0.0f;

		// ��带 �ó׸�ƽ ���� ��ȯ
		m_currentMode = CAMERA_CINEMATIC_MODE;

		//�÷��̾� HUD�� Enemy Hud�� �� ������
		CPlayer* player = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player"));
		player->m_vecHandlers[CPlayer::HUD_HANDLER]->SetActive(false);

		CEnemy* enemy = dynamic_cast<CEnemy*>(m_pTargetObject);
		if (enemy != nullptr)
			enemy->m_vecHandlers[CEnemy::HUD_HANDLER]->SetActive(false);
	}
}
void CCamera_Free::PlayerCamera()
{
	// �÷��̾��� ���� ��ġ�� �����ɴϴ�.
	CComponent* playerTransformCom = m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"));
	CTransform* playerTransform = static_cast<CTransform*>(playerTransformCom);
	_vector playerPosition = playerTransform->Get_State(CTransform::STATE_POSITION);

	// �⺻������ ī�޶�� �÷��̾ ���󰩴ϴ�.
	_vector defaultCameraOffset = XMVectorSet(4.279f, 11.847f, -8.13f, 0.0f);
	_vector cameraPosition = playerPosition + defaultCameraOffset;

	// ���콺 �� �Է� ���� �� ó��
	_byte MouseWheel = m_pGameInstance->Get_DIMouseWheelState();
	float wheelSensitivity = 0.005f; // �� ���� ����

	if (MouseWheel != 0) // ���� �������� ���� ó��
	{
		// ī�޶��� ���� ���͸� �����ɴϴ�.
		_vector forward = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

		// �� �Է¿� ���� ī�޶� �������� ������Ʈ�մϴ�.
		_vector wheelOffset = forward * (wheelSensitivity * MouseWheel);
		m_vCameraOffset.x += XMVectorGetX(wheelOffset);
		m_vCameraOffset.y += XMVectorGetY(wheelOffset);
		m_vCameraOffset.z += XMVectorGetZ(wheelOffset);
	}

	// _float3�� _vector�� ��ȯ�մϴ�.
	_vector vCameraOffset = XMVectorSet(m_vCameraOffset.x, m_vCameraOffset.y, m_vCameraOffset.z, 0.0f);

	// ī�޶��� ��ġ�� ����մϴ�.
	m_vBaseCameraPosition = cameraPosition + vCameraOffset; // ī�޶��� �⺻ ��ġ ����

	// ī�޶��� ��ġ�� �� ��ġ�� �����մϴ�.
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, cameraPosition + vCameraOffset);

	// **ī�޶� �÷��̾ �ٶ󺸵��� �����մϴ�.**
	m_pTransformCom->LookAt(playerPosition);
}

void CCamera_Free::FreeCamera(_float fTimeDelta)
{
	//�⺻ �̵� �ӵ�
	_float fMoveSpeed = 1.0f;

	// ������ ��ư�� ���ȴ��� Ȯ��
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		// Shift Ű�� ���ȴ��� Ȯ���ϰ�, ���ȴٸ� �̵� �ӵ��� ����
		if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT) & 0x80)
		{
			fMoveSpeed = 3.0f; // ���ϴ� �ӵ��� ����
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
		{
			m_pTransformCom->Go_Left(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
		{
			m_pTransformCom->Go_Right(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
		{
			m_pTransformCom->Go_Straight(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
		{
			m_pTransformCom->Go_Backward(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_Q) & 0x80)
		{
			m_pTransformCom->Go_Down(fTimeDelta * fMoveSpeed);
		}

		if (m_pGameInstance->Get_DIKeyState(DIK_E) & 0x80)
		{
			m_pTransformCom->Go_Up(fTimeDelta * fMoveSpeed);
		}

		_long MouseMove = {};

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMouseSensor * MouseMove * fTimeDelta);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), m_fMouseSensor * MouseMove * fTimeDelta);
		}

	}
}

void CCamera_Free::ApplyCameraShake(_float fTimeDelta)
{
	m_fElapsedShakeTime += fTimeDelta;

	if (m_fElapsedShakeTime >= m_fShakeDuration)
	{
		// ��鸲 ����
		StopCameraShake();
		return;
	}

	_float progress = m_fElapsedShakeTime / m_fShakeDuration;
	_float damper = 1.0f - progress; // ��鸲 ���� ����

	// ���� ������ ���
	_float offsetX = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;
	_float offsetY = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;
	_float offsetZ = ((rand() % 1000) / 500.0f - 1.0f) * m_fShakeMagnitude * damper;

	// ��鸲 ������ ���� ����
	m_vShakeOffset = XMVectorSet(offsetX, offsetY, offsetZ, 0.0f);

	// �⺻ ī�޶� ��ġ�� ��鸲 �������� ���մϴ�.
	_vector shakenPosition = m_vBaseCameraPosition + m_vShakeOffset;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, shakenPosition);
}


void CCamera_Free::CinematicCamera(_float fTimeDelta)
{
	if (m_pCurrentCinematicPoints == nullptr || m_pCurrentCinematicPoints->empty())
	{
		// �ó׸�ƽ ����Ʈ�� ���� ��� �⺻ ���� ��ȯ
		m_currentMode = CAMERA_DEFAULT_MODE;


		return;
	}

	// �ʱ�ȭ�� �ʿ��� ���
	if (!m_bCinematicInitialized)
	{
		// �ε��� �� �ð� �ʱ�ȭ
		m_CurrentIndex = 0;
		m_fElapsedTime = 0.0f;

		// ���� �� ���� ī�޶� ����Ʈ ����
		m_CurrentCameraPoint = (*m_pCurrentCinematicPoints)[m_CurrentIndex];
		if (m_pCurrentCinematicPoints->size() > 1)
		{
			m_NextCameraPoint = (*m_pCurrentCinematicPoints)[m_CurrentIndex + 1];
		}
		else
		{
			// ����Ʈ�� �ϳ����̶�� ���� ����Ʈ�� ���� ����Ʈ�� ����
			m_NextCameraPoint = m_CurrentCameraPoint;
		}

		// �ʱ�ȭ �Ϸ� ǥ��
		m_bCinematicInitialized = true;
	}

	// ��� �ð��� ������Ʈ�մϴ�.
	m_fElapsedTime += fTimeDelta;

	// ���� �Ķ���� t ���
	_float duration = m_CurrentCameraPoint.duration;
	_float t = m_fElapsedTime / duration;
	t = max(0.0f, min(1.0f, t));

	// ������ ���� ��ġ ���
	_vector interpolatedLocalPosition;

	switch (m_currentMoveMode)
	{
	case CAMERA_CINEMATIC_MOVE_LIENAR_MODE:
		// ���� ���� ���
		interpolatedLocalPosition = XMVectorLerp(
			m_CurrentCameraPoint.localPosition,
			m_NextCameraPoint.localPosition,
			t
		);
		break;

	case CAMERA_CINEMATIC_MOVE_SPLINE_MODE:
		// ���ö��� ���� ��� (���� �ʿ�)
		interpolatedLocalPosition = CatmullRomSplinePosition(t);
		break;
	}

	// ��� ��ü�� �������� ���� ��� �⺻ ���� ��ȯ
	if (m_pTargetObject == nullptr)
	{
		m_currentMode = CAMERA_DEFAULT_MODE;
		return;
	}

	// ��� ��ü�� Transform ������Ʈ�� �����ɴϴ�.
	CTransform* targetTransform = static_cast<CTransform*>(m_pTargetObject->Get_Component(TEXT("Com_Transform")));
	if (targetTransform == nullptr)
	{
		m_currentMode = CAMERA_DEFAULT_MODE;
		return;
	}

	// ��� ��ü�� ���� ����� �����ɴϴ�.
	_matrix targetWorldMatrix = targetTransform->Get_WorldMatrix();

	// ���� ��ǥ�� ���� ��ǥ�� ��ȯ�մϴ�.
	_vector worldCameraPosition = XMVector3TransformCoord(interpolatedLocalPosition, targetWorldMatrix);

	// ī�޶��� ��ġ�� �����մϴ�.
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, worldCameraPosition);

	// ī�޶��� �⺻ ��ġ�� �����մϴ� (��鸲 ������ ����)
	m_vBaseCameraPosition = worldCameraPosition;

	// ī�޶� �ٶ� ������ �����մϴ� (��� ��ü�� ��ġ)
	_vector worldLookAtPosition = targetTransform->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->LookAt(worldLookAtPosition);

	// ���� ������ �Ϸ�Ǿ����� Ȯ���մϴ�.
	if (t >= 1.0f)
	{
		// ���� ī�޶� ����Ʈ�� �̵�
		m_CurrentIndex++;
		if (m_CurrentIndex < m_pCurrentCinematicPoints->size() - 1)
		{
			// ���� ����Ʈ ����
			m_CurrentCameraPoint = (*m_pCurrentCinematicPoints)[m_CurrentIndex];
			m_NextCameraPoint = (*m_pCurrentCinematicPoints)[m_CurrentIndex + 1];
			m_fElapsedTime = 0.0f; // ��� �ð� �ʱ�ȭ
		}
		else
		{
			m_pGameInstance->StopSlowMotion();

			// ������ ����Ʈ�� ����; �ó׸�ƽ ��� ����
			m_currentMode = CAMERA_DEFAULT_MODE;

			// �ʱ�ȭ ���� ����
			m_bCinematicInitialized = false;
			m_pCurrentCinematicPoints = nullptr;
			m_CurrentCinematicKey.clear();

			// �ʿ��� ��� �߰� ó���� �����մϴ�.

			CPlayer* player = static_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player"));
			player->m_vecHandlers[CPlayer::HUD_HANDLER]->SetActive(true);

			CEnemy* enemy = dynamic_cast<CEnemy*>(m_pTargetObject);
			if (enemy != nullptr)
				enemy->m_vecHandlers[CEnemy::HUD_HANDLER]->SetActive(true);

			if (dynamic_cast<CHyunwoo_Enemy*>(enemy))
				CUIManager::Get_Instance()->Show_Monster(CUI_MiniMap::BOSS_MINIMAP);

			if (dynamic_cast<CAlpha_Enemy*>(enemy))
				CUIManager::Get_Instance()->Show_Monster(CUI_MiniMap::ALPHA_MINIMAP);

		}
	}
}


void CCamera_Free::InitializeCameraPoints()
{
	vector<CameraPoint> cinematic1;

	CameraPoint point1;
	point1.localPosition = XMVectorSet(3.f, 7.f, 0.f, 1.f);
	point1.duration = 0.5f;
	cinematic1.push_back(point1);

	CameraPoint point3;
	point3.localPosition = XMVectorSet(-3.f, 8.f, 0.f, 1.f);  // 180��
	point3.duration = 0.5f;
	cinematic1.push_back(point3);

	CameraPoint point6;
	point6.localPosition = XMVectorSet(0.f, 10.f, -3.f, 1.f);  // 270��
	point6.duration = 0.5f;
	cinematic1.push_back(point6);

	CameraPoint point9;
	point9.localPosition = XMVectorSet(2.5f, 6.f, 2.5f, 1.f);
	point9.duration = 0.5f;
	cinematic1.push_back(point9);

	CameraPoint point11;
	point11.localPosition = XMVectorSet(2.5f, 5.f, 2.5f, 1.f);  // �� ������
	point11.duration = 0.5f;
	cinematic1.push_back(point11);

	// �����̳ʿ� �߰� (Ű�� "SkillR_Cinematic" ������ ����)
	m_Cinematics[L"SkillR_Cinematic"] = cinematic1;

	// ���ο� Alpha_Cinematic ����Ʈ ����
	vector<CameraPoint> alphaCinematic;

	CameraPoint alphaPoint0;
	alphaPoint0.localPosition = XMVectorSet(3.0f, 3.f, 0.0f, 1.0f);    // 0��
	alphaPoint0.duration = 2.f; // ���ϴ� �ð����� ����
	alphaCinematic.push_back(alphaPoint0);

	CameraPoint alphaPoint90;
	alphaPoint90.localPosition = XMVectorSet(0.0f, 3.0f, 3.0f, 1.0f);   // 90��
	alphaPoint90.duration = 2.f;
	alphaCinematic.push_back(alphaPoint90);

	CameraPoint alphaPoint180;
	alphaPoint180.localPosition = XMVectorSet(-3.0f, 3.0f, 0.0f, 1.0f);  // 180��
	alphaPoint180.duration = 2.f;
	alphaCinematic.push_back(alphaPoint180);

	CameraPoint alphaPoint270;
	alphaPoint270.localPosition = XMVectorSet(0.0f, 3.0f, -3.0f, 1.0f);  // 270��
	alphaPoint270.duration = 2.f;
	alphaCinematic.push_back(alphaPoint270);

	// �����̳ʿ� �߰� (Ű�� "Alpha_Cinematic"���� ����)
	m_Cinematics[L"Alpha_Cinematic"] = alphaCinematic;

	// ���ο� Alpha_Cinematic ����Ʈ ����
	vector<CameraPoint> victoryCinematic;

	CameraPoint VictoryPoint0;
	VictoryPoint0.localPosition = XMVectorSet(3.0f, 3.f, 0.0f, 1.0f);    // 0��
	VictoryPoint0.duration = 3.f; // ���ϴ� �ð����� ����
	victoryCinematic.push_back(VictoryPoint0);

	CameraPoint VictoryPoint90;
	VictoryPoint90.localPosition = XMVectorSet(0.0f, 3.0f, 3.0f, 1.0f);   // 90��
	VictoryPoint90.duration = 3.f;
	victoryCinematic.push_back(VictoryPoint90);

	CameraPoint VictoryPoint180;
	VictoryPoint180.localPosition = XMVectorSet(-3.0f, 3.0f, 0.0f, 1.0f);  // 180��
	VictoryPoint180.duration = 3.f;
	victoryCinematic.push_back(VictoryPoint180);

	CameraPoint VictoryPoint270;
	VictoryPoint270.localPosition = XMVectorSet(0.0f, 3.0f, -3.0f, 1.0f);  // 270��
	VictoryPoint270.duration = 3.f;
	victoryCinematic.push_back(VictoryPoint270);

	// �����̳ʿ� �߰� (Ű�� "Alpha_Cinematic"���� ����)
	m_Cinematics[L"Victory_Cinematic"] = victoryCinematic;
}



_vector CCamera_Free::CatmullRomSplinePosition(_float t)
{
	// ����Ʈ ���� Ȯ��
	size_t numPoints = m_pCurrentCinematicPoints->size();

	// ���� �ε���
	size_t index1 = m_CurrentIndex;
	size_t index2 = m_CurrentIndex + 1;

	// ���� �κ� ó��

	size_t index0 = (index1 == 0) ? index1 : index1 - 1;

	// �� �κ� ó��
	size_t index3 = (index2 + 1 < numPoints) ? index2 + 1 : index2;

	// ����Ʈ ��������
	_vector p0 = (*m_pCurrentCinematicPoints)[index0].localPosition;
	_vector p1 = (*m_pCurrentCinematicPoints)[index1].localPosition;
	_vector p2 = (*m_pCurrentCinematicPoints)[index2].localPosition;
	_vector p3 = (*m_pCurrentCinematicPoints)[index3].localPosition;

	// ���� ���� �־��� m_vecPoint���� ī�޶� ������ �̵��Ϸ��� �ϴ� ����Ʈ�� �ۿ� ����.
	// �׷��� ���࿡ �翷�� ����Ʈ�� ���� ���� �� ������ ��ĭ�� ������ �༮�� �糡�� ����Ʈ�� �����Ѵ�.
	// p0, p1, p2, p3�̶�� �Ѵٸ� p0������ ������ p-1�� p0�̶�� �δ°�, �׸��� p4�� p3���� �δ� ��.
	// 
	// ���� �κп��� index0�� index1�� ���ٸ� p0�� p1�� ����
	if (index1 == 0)
		p0 = p1;

	// �� �κп��� index2�� index3�� ���ٸ� p3�� p2�� ����
	if (index3 == index2)
		p3 = p2;

	// ���ö��� ���� ���
	return CatmullRom(p0, p1, p2, p3, t);
}


_vector CCamera_Free::CatmullRom(_vector p0, _vector p1, _vector p2, _vector p3, _float t)
{
	_float t2 = t * t;
	_float t3 = t2 * t;

	return 0.5f * (
		(2.0f * p1) +
		(-p0 + p2) * t +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
		(-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
		);
}


CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();
}
