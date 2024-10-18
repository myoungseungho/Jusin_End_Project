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

#pragma region 명승호

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

#pragma region 카메라
	enum InterpolationType {
		INTERPOLATION_LINEAR_MODE,
		INTERPOLATION_DAMPING_MODE,
		INTERPOLATION_SKIP_MODE,
		INTERPOLATION_END
	};


	// 카메라 포인트 구조체
	struct CameraPoint {
		//포지션
		_float3 position = {};

		//회전 쿼터니언
		_float4 rotation = {};

		_float duration; // 다음 포인트까지 이동 시간
		_int interpolationType; //보간 타입

		const _float4x4* pWorldFloat4x4;
		// Damping Mode에서 사용되는 계수
		_float damping = { 1.f }; // 0.0f ~ 2.0f (조절 가능한 범위)
		_bool hasWorldFloat4x4 = { true };
	};

	struct CameraData {
		_int modelID;
		_int skillID;
		vector<CameraPoint> points;
	};

	//왜 extern이 필요할까?
	//struct나 enum은 타입을 정의한다. 메모리를 즉시 할당 안하고 형식만 정의한다.
	//unordered_map은 런타임에 메모리를 할당해야 한다.
	//C++에서는 전역 객체를 여러 소스 파일에서 공유하려면, 한 곳에만 정의하고 나머지 파일에서는 선언만 해야 합니다.
	//extern 키워드는 "이 변수의 정의는 다른 소스 파일에 있다"는 의미로 사용됩니다.

	extern unordered_map<_int, _wstring> modelIDToString;
	extern unordered_map<_int, _wstring> skillIDToString;
	extern unordered_map<_wstring, _int> stringToModelID;
	extern unordered_map<_wstring, _int> stringToSkillID;

#pragma endregion



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

#pragma endregion
}