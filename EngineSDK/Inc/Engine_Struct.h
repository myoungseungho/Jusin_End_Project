#pragma once
#include "Engine_Typedef.h"


namespace Engine
{
	typedef struct
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;

		/* KEYFRAME : 뼈의 상태 */
		/* fTime : 이 뼈가 이 fTime일때 KeyFrame의 상태를 취한다. */
		float			fTime;
	}KEYFRAME;

	typedef struct
	{
		class CTexture* pMaterials[AI_TEXTURE_TYPE_MAX];
	}MESHMATERIAL;

	/*D3DDECLUSAGE*/
	typedef struct
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;
	}VTXPOSTEX;

	typedef struct
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexcoord;
	}VTXCUBE;

	struct ENGINE_DLL VTXMESH
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static const unsigned int					iNumElements = { 4 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];

	};

	struct ENGINE_DLL VTXANIMMESH
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex;
		XMFLOAT4		vBlendWeight;

		static const unsigned int					iNumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];

	};

	struct BoneWeight {
		_uint vertexID;
		_float weight;
	};

	struct BoneData {
		string name;
		XMFLOAT4X4 transformationMatrix;
		vector<BoneData> children; // Add children to store hierarchical data
	};

	struct VertexWeight {
		_uint vertexID;
		_float weight;
	};

	struct MeshData {
		string name;
		_uint vertexCount;
		_uint indexCount;
		vector<VTXMESH> vertices;
		vector<VTXANIMMESH> animVertices;
		vector<_uint> indices;
		_uint materialIndex;
		vector<BoneData> bones; // 뼈 데이터를 저장할 벡터 추가
		vector<XMFLOAT4X4> offsetMatrices; // 오프셋 행렬을 저장할 벡터 추가
	};

	struct ModelHeader {
		_bool isAnim;
		_uint numMeshes;
		_uint numMaterials;
		_uint numAnimations;
	};

	struct MaterialData {
		string texturePaths[AI_TEXTURE_TYPE_MAX];
	};

	struct ChannelData {
		string name;
		vector<KEYFRAME> keyframes;
	};


	struct AnimationData {
		string name;
		float duration;
		float ticksPerSecond;
		vector<ChannelData> channels;
	};
}