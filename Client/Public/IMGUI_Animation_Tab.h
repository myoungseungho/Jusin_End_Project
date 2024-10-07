#pragma once
#include "IMGUI_Tab.h"
#include "Model.h"

//#include "../../AssimpSaveAndLoad/Data.h"

BEGIN(Client)

struct VertexAnim {
    XMFLOAT3 vPosition;
    XMFLOAT3 vNormal;
    XMFLOAT2 vTexcoord;
    XMFLOAT3 vTangent;
    XMUINT4 vBlendIndex;
    XMFLOAT4 vBlendWeight;
};

struct Vertex {
    XMFLOAT3 vPosition;
    XMFLOAT3 vNormal;
    XMFLOAT2 vTexcoord;
    XMFLOAT3 vTangent;
};

struct Keyframe {
    XMFLOAT3 scale;
    XMFLOAT4 rotation;
    XMFLOAT3 position;
    float time;
};

//struct MODEL_DATA {
//    struct ANIMATION {
//        const char* szName;       // �ִϸ��̼� �̸�
//        float fDuration;          // �ִϸ��̼��� ���� �ð�
//        float fTickPerSecond;     // �ʴ� ƽ �� (�ִϸ��̼� �ӵ�)
//        unsigned int iNumChannels;// ä��(��)�� ����
//
//        struct CHANNEL {
//            const char* szName;         // �� �Ǵ� ����� �̸�
//            unsigned int iBoneIndex;    // �� �ε���
//            unsigned int iNumKeyFrames; // Ű�������� ����
//
//            struct KEYFRAME {
//                float fTime;           // Ű�������� �ð�
//                XMFLOAT3 vScale;       // ������ ��
//                XMFLOAT4 vRotation;    // ȸ�� �� (���ʹϾ�)
//                XMFLOAT3 vPosition;    // ��ġ ��
//            };
//
//            std::vector<KEYFRAME> keyFrames;  // Ű������ ����Ʈ
//        };
//
//        std::vector<CHANNEL> channels;   // ä�� ����Ʈ
//    };
//
//    std::vector<ANIMATION> animations;  // �ִϸ��̼� ����Ʈ
//};


class CIMGUI_Animation_Tab : public CIMGUI_Tab
{

protected:
	CIMGUI_Animation_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMGUI_Animation_Tab() = default;

public:
	HRESULT Initialize() override;
	void Render(_float fTimeDelta) override;

	virtual const _char* GetTabName() const { return "Animation"; };

    void Info_Anim();

private:
	//void LoadModelDataFromBinary();
	//void LoadBoneFromBinary_Tool(ifstream& inFile, BoneData& bone);
    void SelectedModelLoad(std::ifstream& inFile, BoneData& bone);
    void LoadFromFile(const std::string& filename);
    void ProcessEventsBetweenFrames(int characterIndex, int animationIndex, float prevFrame, float currentFrame);

public:
     
private:

   // _bool m_bDrag = false;
    CModel* m_pSelectedModelCom = { nullptr };

    _char buffer[128] = "../Bin/ModelData/Hit_Select.bin";
    _bool m_bMotionPlaying = false;
    _bool m_bCurrentPositionSlide = false;

    _float fSliderDebug = 0.f;

 public:
	static CIMGUI_Animation_Tab* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
