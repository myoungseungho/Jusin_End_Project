#include "..\Public\Bone.h"
#include <fstream>
#include <iostream>

CBone::CBone()
{

}

HRESULT CBone::Initialize(const string& name, _int parentIndex, const XMFLOAT4X4& transformationMatrix)
{
	// m_szName �迭�� name�� ����
	if (name.length() >= MAX_PATH) {
		OutputDebugStringA("ERROR:: Bone name length exceeds buffer size\n");
		return E_FAIL;
	}
	strcpy_s(m_szName, name.c_str());

	m_iParentIndex = parentIndex;

	// Transformation matrix ���� �� ��ġ
	m_TransformationMatrix = transformationMatrix;
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	/* 1. Ư�� �ִϸ��̼��� ���� ����ߴٸ�. == �� �ִϸ��̼��� �����ϴ� ���� TransformationMatrix�� ���ŵƴ�. */
	/* 2. ��� ������ ��ȸ�ϸ鼭 ���ŵ� Ư�� ���� TransformationMatrix�� ������ CombinedTransformation�� �������ش�. */
	// m_CombinedTransformationMatrix = m_TransformationMatrix * �θ�`s CombinedTransformationMatrix;

	if (-1 == m_iParentIndex)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, PreTransformMatrix * XMLoadFloat4x4(&m_TransformationMatrix));

	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentIndex]->m_CombinedTransformationMatrix));	
}

CBone* CBone::Create(const string& name, _int parentIndex, const XMFLOAT4X4& transformationMatrix)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(name, parentIndex, transformationMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CBone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone * CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
	__super::Free();

}

