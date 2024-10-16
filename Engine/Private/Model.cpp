#include "..\Public\Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"
#include "Texture.h"
#include "Animation.h"
#include "Channel.h"
#include <fstream>
#include <iostream>

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CModel::CModel(const CModel& Prototype)
	: CComponent{ Prototype }
	, m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
	, m_iNumMeshes{ Prototype.m_iNumMeshes }
	, m_Meshes{ Prototype.m_Meshes }
	, m_iNumMaterials{ Prototype.m_iNumMaterials }
	, m_Materials{ Prototype.m_Materials }
	/*, m_Bones { Prototype.m_Bones }*/
	, m_iCurrentAnimationIndex{ Prototype.m_iCurrentAnimationIndex }
	, m_iNumAnimations{ Prototype.m_iNumAnimations }
	, m_Animations{ Prototype.m_Animations }
	, m_KeyFrameIndices{ Prototype.m_KeyFrameIndices }
{


	for (auto& pPrototypeBone : Prototype.m_Bones)
		m_Bones.emplace_back(pPrototypeBone->Clone());

	for (auto& pAnimation : m_Animations)
		Safe_AddRef(pAnimation);

	for (auto& Material : m_Materials)
	{
		for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
		{
			Safe_AddRef(Material.pMaterials[i]);
		}
	}

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);
}

void CModel::Add_SoundEvent(_uint index, _float triggerTime, const wstring& soundAlias, _float volume)
{
	if (index >= m_Animations.size())
		return;

	m_Animations[index]->Add_SoundEvent(triggerTime, soundAlias, volume);
}

const _float4x4* CModel::Get_BoneMatrixPtr(const _char* pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone) ->_bool
		{
			if (false == strcmp(pBoneName, pBone->Get_Name()))
				return true;

			return false;
		});

	return (*iter)->Get_CombinedTransformationMatrix();
}

HRESULT CModel::Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	//bin 파일 string
	std::string filePath(pModelFilePath);

	//주어진 파일 경로에서 ".bin" 확장자가 포함되어 있는지를 확인하는 조건입니다.
	// 이 조건은 주어진 파일 경로에 ".bin"이 포함되어 있으면 true를 반환하고, 그렇지 않으면 false를 반환합니다. 
	if (filePath.find(".bin") != std::string::npos) {
		return InitializeFromBinary(filePath, PreTransformMatrix);
	}

	return E_FAIL; // 직접 파일을 로드하는 경우, 이 부분을 제거
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_Buffers();
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

void CModel::Play_Animation(_float fTimeDelta)
{
	// 현재 애니메이션 업데이트
	m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrix(&m_fCurrentAnimPosition, fTimeDelta, m_Bones, m_isLoopAnim, m_KeyFrameIndices[m_iCurrentAnimationIndex]);

	// 블렌딩이 활성화된 경우 블렌딩 처리
	if (m_isBlending)
	{
		m_fBlendTime += fTimeDelta;
		float blendFactor = m_fBlendTime / m_fBlendDuration;

		if (blendFactor >= 1.0f)
		{
			// 블렌딩 완료
			m_isBlending = false;
			m_iCurrentAnimationIndex = m_iNextAnimationIndex;
			m_fCurrentAnimPosition = m_fNextAnimPosition;
			blendFactor = 1.0f;
			return;
		}
		else
		{
			// 각 뼈의 변환 행렬 보간
			for (size_t i = 0; i < m_Bones.size(); i++)
			{
				CBone* pBone = m_Bones[i];
				_matrix currentMatrix = XMLoadFloat4x4(pBone->Get_TransformationMatrix());

				// 다음 애니메이션의 첫 번째 키프레임 행렬 계산
				_matrix nextMatrix;
				m_Animations[m_iNextAnimationIndex]->Compute_FirstKeyFrameMatrixForBone(pBone->Get_Name(), m_Bones, &nextMatrix);

				// 스케일, 회전, 위치 분리
				XMVECTOR currentScale, currentRotQuat, currentTrans;
				XMVECTOR nextScale, nextRotQuat, nextTrans;
				XMMatrixDecompose(&currentScale, &currentRotQuat, &currentTrans, currentMatrix);
				XMMatrixDecompose(&nextScale, &nextRotQuat, &nextTrans, nextMatrix);

				// 보간
				XMVECTOR blendedScale;
				if (pBone->IsDummyBone())
				{
					blendedScale = currentScale; // 더미 뼈는 스케일 보간 건너뛰기
				}
				else
				{
					blendedScale = XMVectorLerp(currentScale, nextScale, blendFactor);
				}
				XMVECTOR blendedRotQuat = XMQuaternionSlerp(currentRotQuat, nextRotQuat, blendFactor);
				XMVECTOR blendedTrans = XMVectorLerp(currentTrans, nextTrans, blendFactor);

				// 합성
				_matrix blendedMatrix = XMMatrixAffineTransformation(blendedScale, XMVectorZero(), blendedRotQuat, blendedTrans);
				m_Bones[i]->SetUp_TransformationMatrix(blendedMatrix);
			}
		}
	}

	// 모든 뼈의 CombinedTransformationMatrix 업데이트
	for (auto& pBone : m_Bones)
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
}


void CModel::SetUp_Animation(_uint iAnimationIndex, _bool isLoop, _float blendDuration)
{
	if (iAnimationIndex >= m_iNumAnimations)
		return;

	m_isLoopAnim = isLoop;

	// 애니메이션이 다를 때만 블렌딩을 설정하거나 초기화
	if (m_iCurrentAnimationIndex != iAnimationIndex)
	{
		if (blendDuration > 0.0f)
		{
			// 다음 애니메이션 설정 및 블렌딩 활성화
			// 블렌딩이 안되고 있을 때만 블렌딩 셋팅
			if (!m_isBlending || m_iNextAnimationIndex != iAnimationIndex)
			{
				m_iNextAnimationIndex = iAnimationIndex;
				m_fBlendDuration = blendDuration;
				m_fBlendTime = 0.0f;
				m_isBlending = true;

				// 다음 애니메이션의 첫 번째 키프레임으로 설정
				m_fNextAnimPosition = 0.0f;
				m_Animations[m_iNextAnimationIndex]->Update_TransformationMatrix(&m_fNextAnimPosition, 0, m_Bones, m_isLoopAnim, m_KeyFrameIndices[m_iNextAnimationIndex]);
			}
		}
		else
		{
			// 블렌딩 없이 애니메이션 설정
			m_iCurrentAnimationIndex = iAnimationIndex;
			m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrix(&m_fCurrentAnimPosition, 0, m_Bones, m_isLoopAnim, m_KeyFrameIndices[m_iCurrentAnimationIndex]);
		}
	}
	else if (m_isBlending && m_iNextAnimationIndex == iAnimationIndex)
	{
		// 같은 애니메이션을 설정하려는 경우, 블렌드 타임 초기화 방지
		m_isLoopAnim = isLoop;
	}
}

_float CModel::GetDurationByIndex(_uint _animationIndex)
{
	if (_animationIndex >= m_iNumAnimations)
		return -1.f;

	return m_Animations[_animationIndex]->GetDuration();
}


HRESULT CModel::Bind_MaterialSRV(CShader* pShader, aiTextureType eType, const _char* pConstantName, _uint iMeshIndex)
{
	_uint		iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pMaterials[eType]->Bind_ShaderResource(pShader, pConstantName, 0);
}

HRESULT CModel::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
	_float4x4		BoneMatrices[800] = {};

	m_Meshes[iMeshIndex]->SetUp_Matrices(BoneMatrices, m_Bones);

	pShader->Bind_Matrices(pConstantName, BoneMatrices, 800);

	return S_OK;
}

HRESULT CModel::InitializeFromBinary(const string& binFilePath, _fmatrix PreTransformMatrix) {
	std::ifstream inFile(binFilePath, std::ios::binary);
	if (!inFile) {
		OutputDebugStringA("ERROR:: Could not open binary file for reading\n");
		return E_FAIL;
	}

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	// 헤더 읽기
	ModelHeader header;
	inFile.read(reinterpret_cast<char*>(&header), sizeof(header));
	m_iNumMeshes = header.numMeshes;
	m_Meshes.resize(header.numMeshes);
	m_iNumMaterials = header.numMaterials;
	m_Materials.resize(header.numMaterials);

	/*char buffer[256];
	snprintf(buffer, sizeof(buffer), "Reading header: %u meshes, %u materials\n", header.numMeshes, header.numMaterials);
	OutputDebugStringA(buffer);*/

	// 애니메이션이 있는 경우, 전체 뼈 데이터를 먼저 읽어들입니다.
	if (header.isAnim) {
		//뼈 숫자
		_uint numBones;
		inFile.read(reinterpret_cast<char*>(&numBones), sizeof(numBones));
		m_Bones.reserve(numBones);

		//람다식
		function<void(_uint, int)> ReadBonesRecursive = [&](_uint numBones, int parentIndex) {
			for (_uint i = 0; i < numBones; ++i) {
				//이름 길이
				size_t nameLength;
				inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
				//이름
				string boneName(nameLength, '\0');
				inFile.read(&boneName[0], nameLength);

				//뼈의 변환행렬
				XMFLOAT4X4 transformationMatrix;
				inFile.read(reinterpret_cast<char*>(&transformationMatrix), sizeof(XMFLOAT4X4));
				//전치해서 저장
				XMStoreFloat4x4(&transformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&transformationMatrix)));

				//뼈의 자식수
				_uint numChildren;
				inFile.read(reinterpret_cast<char*>(&numChildren), sizeof(numChildren));

				//루트뼈는 parentIndex가 -1
				CBone* pBone = CBone::Create(boneName, parentIndex, transformationMatrix);
				if (nullptr == pBone) {
					OutputDebugStringA("ERROR:: Failed to create bone from binary data\n");
					return E_FAIL;
				}
				m_Bones.emplace_back(pBone);

				int currentBoneIndex = m_Bones.size() - 1;
				ReadBonesRecursive(numChildren, currentBoneIndex);
			}
			};

		ReadBonesRecursive(numBones, -1);
	}


	// 메쉬 데이터를 읽어들입니다.
	for (_uint i = 0; i < header.numMeshes; ++i) {
		MeshData meshData;

		//메쉬 이름길이
		size_t nameLength;
		inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
		//메쉬 이름
		meshData.name.resize(nameLength);
		inFile.read(&meshData.name[0], nameLength);

		//메쉬 버텍스 숫자
		inFile.read(reinterpret_cast<char*>(&meshData.vertexCount), sizeof(meshData.vertexCount));
		//메쉬 인덱스 숫자
		inFile.read(reinterpret_cast<char*>(&meshData.indexCount), sizeof(meshData.indexCount));

		if (header.isAnim) {
			meshData.animVertices.resize(meshData.vertexCount);
			inFile.read(reinterpret_cast<char*>(meshData.animVertices.data()), meshData.vertexCount * sizeof(VTXANIMMESH));
		}
		else {
			meshData.vertices.resize(meshData.vertexCount);
			inFile.read(reinterpret_cast<char*>(meshData.vertices.data()), meshData.vertexCount * sizeof(VTXMESH));
		}

		meshData.indices.resize(meshData.indexCount);
		inFile.read(reinterpret_cast<char*>(meshData.indices.data()), meshData.indexCount * sizeof(_uint));

		inFile.read(reinterpret_cast<char*>(&meshData.materialIndex), sizeof(meshData.materialIndex));

		if (header.isAnim) {
			_uint numBones;
			inFile.read(reinterpret_cast<char*>(&numBones), sizeof(numBones));
			meshData.bones.resize(numBones);
			for (auto& bone : meshData.bones) {
				LoadBoneFromBinary(inFile, bone);
			}

			_uint numOffsetMatrices;
			inFile.read(reinterpret_cast<char*>(&numOffsetMatrices), sizeof(numOffsetMatrices));
			meshData.offsetMatrices.resize(numOffsetMatrices);
			inFile.read(reinterpret_cast<char*>(meshData.offsetMatrices.data()), numOffsetMatrices * sizeof(XMFLOAT4X4));
		}

		// 메쉬 객체 생성
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, header.isAnim, meshData, m_Bones, PreTransformMatrix);
		if (nullptr == pMesh) {
			OutputDebugStringA("ERROR:: Failed to create mesh from binary data\n");
			return E_FAIL;
		}

		m_Meshes[i] = pMesh;
	}

	bool textureCreated = false;

	// Non-anim 모델일 때, 각 메쉬에 대해 별도의 텍스처 경로를 생성
	if (!header.isAnim) {
		m_iNumMaterials = header.numMeshes; // 메쉬의 숫자만큼 m_iNumMaterials 설정
		m_Materials.resize(m_iNumMaterials);
	}

	// 재료 데이터를 읽어들입니다.
	for (_uint i = 0; i < m_iNumMaterials; ++i) {
		bool textureCreated = false;

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j) {
			size_t pathLength;
			inFile.read(reinterpret_cast<char*>(&pathLength), sizeof(pathLength));

			if (pathLength > 0) {
				string texturePath(pathLength, '\0');
				inFile.read(&texturePath[0], pathLength);

				wstring basePath = L"../Bin/ModelData/";
				wstring wTexturePath = basePath + wstring(texturePath.begin(), texturePath.end());

				m_Materials[i].pMaterials[j] = CTexture::Create(m_pDevice, m_pContext, wTexturePath.c_str(), 1);

				char buffer[256];
				snprintf(buffer, sizeof(buffer), "Reading material[%u] texturePath[%u]: %s\n", i, j, texturePath.c_str());
				OutputDebugStringA(buffer);

				textureCreated = true;
			}
		}

		// 만약 어떤 텍스처도 생성되지 않았다면, 파일 이름에서 확장자를 변경하여 경로를 생성합니다.
		if (!textureCreated) {
			// 파일 이름 추출 및 확장자 변경
			size_t lastSlashPos = binFilePath.find_last_of("/\\");
			string fileName = binFilePath.substr(lastSlashPos + 1);
			size_t dotPos = fileName.find_last_of(".");
			if (dotPos != string::npos) {
				fileName = fileName.substr(0, dotPos) + "_" + std::to_string(i) + ".png";
			}
			else {
				fileName += "_" + std::to_string(i) + ".png";
			}

			if (fileName == "hit_Anime4.fbm/HTN_base.png")
				fileName = "HTN_base.png";
			wstring basePath = L"../Bin/ModelData/";
			wstring wTexturePath = basePath + wstring(fileName.begin(), fileName.end());

			// m_Materials[i].pMaterials[1]에 경로 설정
			m_Materials[i].pMaterials[1] = CTexture::Create(m_pDevice, m_pContext, wTexturePath.c_str(), 1);

		/*	char buffer[256];
			snprintf(buffer, sizeof(buffer), "Fallback texture path: %s\n", string(wTexturePath.begin(), wTexturePath.end()).c_str());
			OutputDebugStringA(buffer);*/
		}
	}

	// 애니메이션 데이터를 읽어들입니다.
	if (header.isAnim) {
		m_iNumAnimations = header.numAnimations;
		vector<AnimationData> animations(header.numAnimations);
		m_KeyFrameIndices.resize(header.numAnimations);
		for (auto& anim : animations) {
			size_t nameLength;
			inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
			anim.name.resize(nameLength);
			inFile.read(&anim.name[0], nameLength);
			inFile.read(reinterpret_cast<char*>(&anim.duration), sizeof(anim.duration));
			inFile.read(reinterpret_cast<char*>(&anim.ticksPerSecond), sizeof(anim.ticksPerSecond));

			_uint numChannels;
			inFile.read(reinterpret_cast<char*>(&numChannels), sizeof(numChannels));
			anim.channels.resize(numChannels);

			for (auto& channel : anim.channels) {
				size_t channelNameLength;
				inFile.read(reinterpret_cast<char*>(&channelNameLength), sizeof(channelNameLength));
				channel.name.resize(channelNameLength);
				inFile.read(&channel.name[0], channelNameLength);

				_uint numKeyframes;
				inFile.read(reinterpret_cast<char*>(&numKeyframes), sizeof(numKeyframes));
				channel.keyframes.resize(numKeyframes);

				inFile.read(reinterpret_cast<char*>(channel.keyframes.data()), numKeyframes * sizeof(KEYFRAME));
			}
		}

		for (size_t i = 0; i < animations.size(); ++i) {
			CAnimation* pAnimation = CAnimation::Create(animations[i], m_Bones, m_KeyFrameIndices[i]);
			if (nullptr == pAnimation) {
				OutputDebugStringA("ERROR:: Failed to create animation from binary data\n");
				return E_FAIL;
			}
			m_Animations.emplace_back(pAnimation);
		}
	}

	inFile.close();
	return S_OK;
}

void CModel::LoadBoneFromBinary(std::ifstream& inFile, BoneData& bone) {
	//메쉬에 영향을 주는 뼈의 이름 길이
	size_t nameLength;
	inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
	//뼈 이름
	bone.name.resize(nameLength);
	inFile.read(&bone.name[0], nameLength);
	//뼈의 데이터 추가
	inFile.read(reinterpret_cast<char*>(&bone.transformationMatrix), sizeof(bone.transformationMatrix));
	//뼈의 자식
	_uint numChildren;
	inFile.read(reinterpret_cast<char*>(&numChildren), sizeof(numChildren));
	bone.children.resize(numChildren);
	for (auto& child : bone.children) {
		LoadBoneFromBinary(inFile, child);
	}
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();

	for (auto& Material : m_Materials)
	{
		for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
			Safe_Release(Material.pMaterials[i]);
	}

	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);
}

