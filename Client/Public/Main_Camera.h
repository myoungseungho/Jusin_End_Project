#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CMain_Camera final : public CCamera
{
public:
	enum CAMERA_MODE
	{
		CAMERA_FREE_MODE,
		CAMERA_DEFAULT_MODE,
		CAMERA_MODE_END
	};

	enum VIRTUAL_CAMERA
	{
		VIRTUAL_CAMERA_NORMAL,
		VIRTUAL_CAMERA_SON_SKILL_1,
		VIRTUAL_CAMERA_SON_SKILL_2,
		VIRTUAL_CAMERA_SON_SKILL_3,
		VIRTUAL_CAMERA_HIT_SKILL_1,
		VIRTUAL_CAMERA_HIT_SKILL_2,
		VIRTUAL_CAMERA_HIT_SKILL_3,
		VIRTUAL_CAMERA_MINE_SKILL_1,
		VIRTUAL_CAMERA_MINE_SKILL_2,
		VIRTUAL_CAMERA_MINE_SKILL_3,
		VIRTUAL_CAMERA_21_SKILL_1,
		VIRTUAL_CAMERA_21_SKILL_2,
		VIRTUAL_CAMERA_21_SKILL_3,
		VIRTUAL_CAMERA_END
	};

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
	void Set_Virtual_Camera(VIRTUAL_CAMERA mode) { m_currentVirtualMode = mode; }
	VIRTUAL_CAMERA Get_Virtual_Camera() { return m_currentVirtualMode; };
	void Add_Point(_float duration, InterpolationType type, const _float4x4* worldMatrixPtr, _float damping, _bool hasWorldFloat4x4);
	void Remove_Point(_int currentIndex);
	vector<CameraPoint>& Get_VectorPoint();
	void Play();
	void Move_Point(_int index);
	void Modify_Transform(_int index);
	void ApplyCameraData(vector<CameraData>& cameraDataList);
	_int Get_CameraIndex(_int modelID, _int skillID);
	void Delete_Points();
	
private:
	void Free_Camera(_float fTimeDelta);
	void Default_Camera(_float fTimeDelta);
public:
	vector<CCamera*> m_vecVirtualCamera;

private:
	VIRTUAL_CAMERA m_currentVirtualMode = { VIRTUAL_CAMERA_NORMAL };
	CAMERA_MODE m_currentMode = { CAMERA_FREE_MODE };


public:
	static CMain_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END