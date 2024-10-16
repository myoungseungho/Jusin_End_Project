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

	struct EFFECT_KEYFRAME
	{
		XMFLOAT3 vScale;
		XMFLOAT3 vRotation;
		XMFLOAT3 vPosition;

		bool bIsNotPlaying;
		float fCurTime;
		float fDuration;

		// 생성자에서 초기화
		EFFECT_KEYFRAME()
			: vScale(1.0f, 1.0f, 1.0f),
			vRotation(0.0f, 0.0f, 0.0f),
			vPosition(0.0f, 0.0f, 0.0f),
			bIsNotPlaying(false),
			fCurTime(0.0f),
			fDuration(0.0f)
		{
		}
	};

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

		/* 난반사를 정의하기위한 빛의 색상. */
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

	//파일 저장 구조체
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

	struct EFFECT_KEYFRAME_DATA
	{
		_uint keyFrameNumber;
		_float3 position;
		_float3 scale;
		_float3 rotation;
		_float curTime;
		_float duration;
	};

	struct EFFECT_DATA
	{
		wstring effectName;
		wstring modelName;
		wstring maskTextureName;
		wstring diffuseTextureName;
		_int renderIndex;
		_int passIndex;
		_int uniqueIndex;
		_bool isNotPlaying;
		_bool isLoop;
		_float3 position;
		_float3 scale;
		_float3 rotation;
		vector<EFFECT_KEYFRAME_DATA> keyframes;
	};

	struct EFFECT_LAYER_DATA
	{
		wstring layerName;
		_float duration;
		_float tickPerSecond;
		_uint keyFramesCount;
		vector<EFFECT_DATA> effects;
	};


#pragma region 바이너리
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
#pragma endregion


}