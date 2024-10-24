#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CMain_Camera final : public CCamera
{
public:
	enum VIRTUAL_CAMERA
	{
		VIRTUAL_CAMERA_NORMAL,
		VIRTUAL_CAMERA_FREE,
		VIRTUAL_CAMERA_SON_SKILL_1,
		VIRTUAL_CAMERA_SON_SKILL_2,
		VIRTUAL_CAMERA_HIT_SKILL_1,
		VIRTUAL_CAMERA_MINE_SKILL_1,
		VIRTUAL_CAMERA_MINE_SKILL_2,
		VIRTUAL_CAMERA_21_SKILL_1,
		VIRTUAL_CAMERA_21_SKILL_2,
		VIRTUAL_CAMERA_21_SKILL_3,
		VIRTUAL_CAMERA_END
	};

	enum PLAYER_STATE
	{
		PLAYER_1P,
		PLAYER_2P
	};


	//// 스킬 이름을 모델 ID와 스킬 ID로 매핑
	//unordered_map<std::pair<_int, _wstring>, _int> stringToSkillID;
	//// 애니메이션 이름을 모델 ID, 스킬 ID, 애니메이션 ID로 매핑
	//unordered_map<std::tuple<_int, _int, _wstring>, _int> stringToAnimID;

private:
	CMain_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMain_Camera(const CMain_Camera& Prototype);
	virtual ~CMain_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render(_float fTimeDelta) override;

	//버츄얼 카메라 셋팅
	void Set_Virtual_Camera(VIRTUAL_CAMERA mode);
	VIRTUAL_CAMERA Get_Virtual_Camera() { return m_currentVirtualMode; };
	void Add_Point(_float duration, _int type, const _float4x4* worldMatrixPtr, _float damping, _bool hasWorldFloat4x4, _int animationIndex);
	void Remove_Point(_int currentIndex, _int animationIndex);
	vector<CameraPoint>& Get_VectorPoint(_int index);
	void Play(_int animationIndex);
	void Stop();
	void Pause();
	void Button_Stop();
	void StartCameraShake(_float fDuration, _float fMagnitude);

	void Move_Point(_int index, _int animationIndex);
	void Modify_Transform(_int index, _int animationIndex);
	void ApplyCameraData(CameraSaveData& cameraData);
	_int Get_CameraIndex(_int modelID, _int skillID);
	void Delete_Points(_int animationIndex);
	void SetPosition(_fvector position);

	void SetPlayer(PLAYER_STATE, CGameObject* pPlayer);
	const char* Get_Current_CameraName();
	_bool Get_IsPlay();

public:
	vector<class CVirtual_Camera*> m_vecVirtualCamera;

private:
	VIRTUAL_CAMERA m_currentVirtualMode = { VIRTUAL_CAMERA_NORMAL };

	unordered_map<std::string, int> stringToSkillID;
	unordered_map<std::string, int> stringToAnimID;

public:
	static CMain_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END