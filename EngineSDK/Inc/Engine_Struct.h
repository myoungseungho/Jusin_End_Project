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

	struct EFFECT_KEYFRAME
	{
		XMFLOAT3 vScale;
		XMFLOAT3 vRotation;
		XMFLOAT3 vPosition;

		bool bIsNotPlaying;
		float fCurTime;
		float fDuration;

		// �����ڿ��� �ʱ�ȭ
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

		/* ���ݻ縦 �����ϱ����� ���� ����. */
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

		int*			pPlayerDirection;
		string			strName;
		float			fLifeTime;
		float			fAccTime;
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

#pragma region ī�޶�


	//���ϸŴ������� ������ �ؾ� �ϴ� ����ü�� ���� �־�� �Ѵ�.
	// ī�޶� ����Ʈ ����ü
	struct CameraPoint {
		//������
		_float3 position = {};

		//ȸ�� ���ʹϾ�
		_float4 rotation = {};

		_float duration; // ���� ����Ʈ���� �̵� �ð�
		_int interpolationType; //���� Ÿ��

		const _float4x4* pWorldFloat4x4;
		// Damping Mode���� ���Ǵ� ���
		_float damping = { 1.f }; // 0.0f ~ 2.0f (���� ������ ����)
		_bool hasWorldFloat4x4 = { true };
	};

	struct CameraSaveData {
		// �𵨺� ��ų, �ִϸ��̼�, ����Ʈ�� �����ϴ� ����ü

		struct ModelData 
		{
			_int modelID;
			struct SkillData {
				string skillName;
				struct AnimationData {
					string  animationName;
					vector<CameraPoint> points;
				};
				vector<AnimationData> animations;
			};
			vector<SkillData> skills;
		};

		vector<ModelData> models;
	};

	//�� extern�� �ʿ��ұ�?
	//struct�� enum�� Ÿ���� �����Ѵ�. �޸𸮸� ��� �Ҵ� ���ϰ� ���ĸ� �����Ѵ�.
	//unordered_map�� ��Ÿ�ӿ� �޸𸮸� �Ҵ��ؾ� �Ѵ�.
	//C++������ ���� ��ü�� ���� �ҽ� ���Ͽ��� �����Ϸ���, �� ������ �����ϰ� ������ ���Ͽ����� ���� �ؾ� �մϴ�.
	//extern Ű����� "�� ������ ���Ǵ� �ٸ� �ҽ� ���Ͽ� �ִ�"�� �ǹ̷� ���˴ϴ�.

	extern unordered_map<_int, _wstring> modelIDToString;
	extern unordered_map<_wstring, _int> stringToModelID;

#pragma endregion

#pragma region ����Ʈ
	struct EFFECT_KEYFRAME_DATA
	{
		_uint keyFrameNumber;
		_float3 position;
		_float3 scale;
		_float3 rotation;
		_float curTime;
		_float duration;
		_bool bIsNotPlaying;
	};

	struct EFFECT_DATA
	{
		wstring effectName;
		wstring modelName;
		wstring maskTextureName;
		wstring diffuseTextureName;
		_int	effectType;
		_int renderIndex;
		_int passIndex;
		_int uniqueIndex;
		_bool isNotPlaying;
		_bool isLoop;
		_float3 position;
		_float3 scale;
		_float3 rotation;
		_int iNumKeyFrame;
		vector<EFFECT_KEYFRAME_DATA> keyframes;
	};

	struct EFFECT_LAYER_DATA
	{
		wstring layerName;
		_float duration;
		_float tickPerSecond;
		_uint keyFramesCount;
		_int iNumEffect;
		vector<EFFECT_DATA> effects;
	};
#pragma endregion
	

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

#pragma endregion
}