#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	void Add_SoundEvent(_uint index, _float triggerTime, const wstring& soundAlias, _float volume);

	const _float4x4* Get_BoneMatrixPtr(const _char* pBoneName) const;

public:
	virtual HRESULT Initialize_Prototype(const _char* strModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);

public:
	void Play_Animation(_float fTimeDelta);
	void SetUp_Animation(_uint iAnimationIndex, _bool isLoop, _float blendDuration = 0.0f);
	_float GetDurationByIndex(_uint _animationIndex);
	_float Get_CurrentAnimationTime() const { return m_fCurrentAnimPosition; }

public:
	HRESULT Bind_MaterialSRV(class CShader* pShader, aiTextureType eType, const _char* pConstantName, _uint iMeshIndex);
	HRESULT Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);

private:
	TYPE						m_eModelType = { TYPE_END };
	_float4x4					m_PreTransformMatrix = {};

private: /* For.Meshes */
	_uint						m_iNumMeshes = {};
	vector<class CMesh*>		m_Meshes;

private: /* For.Materials */
	_uint						m_iNumMaterials = {};
	vector<MESHMATERIAL>		m_Materials;

private: /* For.Bones */
	vector<class CBone*>		m_Bones;

private: /* For.Animation */
	_float						m_fCurrentAnimPosition = {};
	_uint						m_iCurrentAnimationIndex = {};
	_bool						m_isLoopAnim = { false };
	_uint						m_iNumAnimations = {};
	vector<class CAnimation*>	m_Animations;
	vector<vector<_uint>>		m_KeyFrameIndices;

	_bool						m_isBlending = { false };
	_float						m_fBlendTime = {};
	_uint						m_iNextAnimationIndex = {};
	_float						m_fBlendDuration = {};
	_float						m_fNextAnimPosition = {};
public:
	// 바이너리 로드
	HRESULT InitializeFromBinary(const string& binFilePath, _fmatrix PreTransformMatrix);
	void LoadBoneFromBinary(std::ifstream& inFile, BoneData& bone);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END