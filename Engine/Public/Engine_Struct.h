#pragma once
#include "Engine_Typedef.h"


namespace Engine
{
	typedef struct
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;

		/* KEYFRAME : ���� ���� */
		/* fTime : �� ���� �� fTime�϶� KeyFrame�� ���¸� ���Ѵ�. */
		float			fTime;
	}KEYFRAME;

	typedef struct
	{
		class CTexture* pMaterials[AI_TEXTURE_TYPE_MAX];
	}MESHMATERIAL;

	struct ENGINE_DLL LIGHT_DESC
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE			eType;
		XMFLOAT4		vDirection;
		XMFLOAT4		vPosition; /* In.WorldSpace*/
		float			fRange;

		/* ���ݻ縦 �����ϱ����� ���� ����. */
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
	};

	/*D3DDECLUSAGE*/
	struct ENGINE_DLL VTXPOSTEX
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int					iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	};

	struct ENGINE_DLL VTXCUBE
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexcoord;
	};

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

#pragma region ���ȣ

	//���� ���� ����ü
	struct FILEDATA
	{
		_bool isParsing;
		wstring prototypeTag;
		wstring layerName;
		_uint levelIndex;
		_float3 position;
		_float3 rightRotation;
		_float3 upRotation;
		_float3 lookRotation;
		_float3 scale;
	};

#pragma region ���̳ʸ�
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
		vector<BoneData> bones; // �� �����͸� ������ ���� �߰�
		vector<XMFLOAT4X4> offsetMatrices; // ������ ����� ������ ���� �߰�
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
#pragma endregion

#pragma region ī�޶�
	struct CameraData
	{
		_float3					vEye{}, vAt{};
		_float					fFovy{}, fNear{}, fFar{};

		_float					fSpeedPerSec{};
		_float					fRotationPerSec{};
		_float					fSensor{};

		_float					fViewportWidth{};
		_float					fViewportHeight{};

		_float4x4				WorldMatrix_Float4x4;
	};
#pragma endregion

#pragma endregion
}