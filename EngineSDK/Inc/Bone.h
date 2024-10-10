#pragma once

#include "Base.h"

/* assimp : ���� ǥ�� (aiNode*, aiBone*, aiNodeAnim*) */
/* ���� ���¸� ǥ���ϱ����� �����͸� �����Ѵ�. (aiNode) */
/* ���� ��Ӱ��踦 �����Ѵ�. (aiNode) */
/* ������ ���� � �������� �󸶳� ������ �ִ���? (aiBone) */

/* ���� �ð��� ���� ���� ��ȭ�� ���� ���. */
/* ���� ���µ��� ����ȭ�Ͽ� �������� �����ϴ� ��� */

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const _char* Get_Name() const {
		return m_szName;
	}

	const _float4x4* Get_TransformationMatrix() const {
		return &m_TransformationMatrix;
	}

	const _float4x4* Get_CombinedTransformationMatrix() const {
		return &m_CombinedTransformationMatrix;
	}

public:
	void SetUp_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

public:
	HRESULT Initialize(const string& name, _int parentIndex, const XMFLOAT4X4& transformationMatrix);

public:
	bool IsDummyBone() const { return m_isDummyBone; }
	void SetDummyBone(bool isDummy) { m_isDummyBone = isDummy; }

	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);
	

	//_matrix			ChildMatrix = XMMatrixTranslation(10, 0, 0);
	//_matrix			ParentMatrix = XMMatrixTranslation(10, 10, 10);

	// ParentMatrix = ParentMatrix.TransformationMatrix * GransFather`s MAtrix;

	//_matrix			BoneMatrix = ChildMatrix * ParentMatrix;

public:
	_char				m_szName[MAX_PATH] = {};

	/* �� ������ �������(�ڽ����) => �θ� �������� ǥ���� �������. */
	_float4x4			m_TransformationMatrix = {};
	_float4x4			m_CombinedTransformationMatrix = {};

	/* ���� ���� ������ �� ���� ���� ����� ó���ҷ��� �Ѵ�. */
	/* CBone��ü�� ������ ���� �����ϰ� �� ���̰� ���� ������ �ִ� �θ��� �ּҸ� ���Ӱ� ������ ���� �ּҷ� ��� ��ü���־���Ѵ�. xxxxxxxxxxx */ 
	// CBone*				m_pParent = { nullptr };
	/* ���� ���� ������ �� ���� ���� ����� ó���ҷ��� �Ѵ�. */
	/* CBone��ü�� ������ ���� �����ϰ� �� ���̰� ���� ������ �ִ� �θ��� �ε������� �� �����Ǹ� �ȴ�. */
	_int				m_iParentIndex = { -1 };
	_bool				m_isDummyBone = { false };
public:
	static CBone* Create(const string& name, _int parentIndex, const XMFLOAT4X4& transformationMatrix);

	CBone* Clone();
	virtual void Free() override;
};

END