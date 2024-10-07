#include "stdafx.h"
#include "IMGUI_Animation_Tab.h"
#include "GameInstance.h"
#include "RenderInstance.h"

#include "imgui.h"

#include <fstream>  
#include <iostream> 
#include <vector>   
#include <string>   

//#include "Windows.h"
//#include "shellapi.h"

#include "Animation.h"
#include "Channel.h"



#include "AnimationEvent_Defines.h"

CIMGUI_Animation_Tab::CIMGUI_Animation_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CIMGUI_Tab{ pDevice,pContext }
{
}


static FrameEventMap FrameEvent;// [CHARACTER_INDEX_END] [100] [2] ;

HRESULT CIMGUI_Animation_Tab::Initialize()
{

    //static FrameEventMap FrameEvent[CHARACTER_INDEX_END][100][2];
  //  FrameEvent[SELECT_HIT][0][5].emplace("TEST");
    FrameEvent[SELECT_HIT][0][50].push_back("TEST");

    //FrameEvent[PlAYER_GOKU][ATTACK][50].push_back("TEST");

	return S_OK;
}


static int g_CurrentAnimationIndex = 999;

void CIMGUI_Animation_Tab::Render(_float fTimeDelta)
{
    
   //�巡�׸� ���� �ٷ� �ε� �ϴ� ����� ����� ������ Default\Client ���� ���� �ؼ� ���
   //if (ImGui::Button("DragMod"))
   //{
   //   // m_bDrag = !m_bDrag;
   //   // DragAcceptFiles(g_hWnd, m_bDrag);
   //}



    ImGui::InputText("Label", buffer, IM_ARRAYSIZE(buffer));

    if (ImGui::Button("Label Load"))
    {

        //../Bin/ModelData\Hit_Select.bin

        if (m_pSelectedModelCom != nullptr)
            Safe_Release(m_pSelectedModelCom);

        _fmatrix preMatrix = XMMatrixIdentity();
       m_pSelectedModelCom = CModel::Create(m_pDevice, m_pContext, buffer, preMatrix);

    }
    if (ImGui::Button("Object Load"))
    {

        //../Bin/ModelData\Hit_Select.bin

        if (m_pSelectedModelCom != nullptr)
            Safe_Release(m_pSelectedModelCom);

        //_fmatrix preMatrix = XMMatrixIdentity();

        //m_pSelectedModelCom =static_cast<CModel*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Com_Model"), 0));
        m_pSelectedModelCom = static_cast<CModel*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Model_Preview"), TEXT("Com_Model"), 0));

    }

    
    if (m_pSelectedModelCom != nullptr)
    {
         m_pSelectedModelCom->m_Animations;
         Info_Anim();

         if (m_bMotionPlaying)
         {

             _float fPrePosition = m_pSelectedModelCom->m_fCurrentAnimPosition;

             m_pSelectedModelCom->Play_Animation(fTimeDelta);
             ImGui::Text("CurrentAnimPosition :%f " ,(m_pSelectedModelCom->m_fCurrentAnimPosition) );

             _float fCurPosition = m_pSelectedModelCom->m_fCurrentAnimPosition;
             

            //if(fPrePosition != fCurPosition)
            //{
            //    for (int i = fPrePosition; i < fCurPosition; i++)
            //    {
            //
            //        Event tevent=  FrameEvent[SELECT_HIT][m_pSelectedModelCom->m_iCurrentAnimationIndex][i];
            //
            //    }
            //
            //}

             ProcessEventsBetweenFrames(0, m_pSelectedModelCom->m_iCurrentAnimationIndex, fPrePosition, fCurPosition);

         }

         if (m_bCurrentPositionSlide)
         {
             float maxValue = m_pSelectedModelCom->m_Animations[g_CurrentAnimationIndex]->m_fDuration;       // �����̴� �ִ밪
             ImGui::SliderFloat("Current Position", &(m_pSelectedModelCom->m_fCurrentAnimPosition), 0, maxValue);
         }

    }

    _bool DebugPoint = false;



}

void CIMGUI_Animation_Tab::Info_Anim()
{
    //999�� �� ������ 0���� �ϸ� ������ 
   // static int g_CurrentAnimationIndex = 999;

    static bool isLoop = false;

    std::vector<const char*> animNames;

    //�¿�
   // for (const auto& anim : m_pSelectedModelCom->m_ModelData.animations) {
   //     animNames.push_back(anim.szName);
   // }

    //���� ��
    for (auto AnimData : m_pSelectedModelCom->m_Animations)
    {
        animNames.push_back(AnimData->m_szName);
    }


    ImGui::Text("Selected Animation");
    ImGui::Combo("##", &g_CurrentAnimationIndex, animNames.data(), animNames.size());

    if (g_CurrentAnimationIndex < animNames.size())
    {
        // ���� ���õ� �ִϸ��̼� �����͸� ������
         
        //Ver.�¿�
        //const auto& selectedAnim = m_pSelectedModelCom->m_ModelData.animations[g_CurrentAnimationIndex];


        const auto& selectedAnim = m_pSelectedModelCom->m_Animations[g_CurrentAnimationIndex];

        // �ִϸ��̼��� �� ����� ImGui::Text�� ǥ��
        ImGui::Text("AnimIndex: %d", g_CurrentAnimationIndex);
        ImGui::Text("Name: %s", selectedAnim->m_szName);
        ImGui::Text("Duration: %f", selectedAnim->m_fDuration);
        ImGui::Text("Ticks Per Second: %f", selectedAnim->m_fTickPerSecond);
        ImGui::Text("Number of Channels: %u", selectedAnim->m_iNumChannels);

        // �� ä���� �ϳ��� ���� Ʈ�� ���� ǥ��
        if (ImGui::TreeNode("Channels"))
        {
            // �� ä���� ���� ǥ��
            for (size_t i = 0; i < selectedAnim->m_Channels.size(); ++i)
            {
                const auto& channel = selectedAnim->m_Channels[i];
               //const auto channel = selectedAnim->m_Channels[i];


                //if (ImGui::TreeNode((void*)(intptr_t)i, "Channel %u: %s", static_cast<unsigned>(i), channel->m_szName))
                if (ImGui::TreeNode((void*)(intptr_t)i, "Channel %u: %s", static_cast<unsigned>(i), channel->GetName()))
                {
                   //ImGui::Text("  Name: %s", channel.m_szName);
                   //ImGui::Text("  Bone Index: %u", channel.iBoneIndex);
                   //ImGui::Text("  Number of KeyFrames: %u", channel.iNumKeyFrames);

                    ImGui::Text("  Name: %s", channel->GetName());
                    ImGui::Text("  Bone Index: %u", channel->m_iBoneIndex);
                    ImGui::Text("  Number of KeyFrames: %u", channel->m_iNumKeyFrames);

                    // �� Ű�������� ���� ǥ��
                    if (ImGui::TreeNode("KeyFrames"))
                    {
                        for (size_t j = 0; j < channel->m_KeyFrames.size(); ++j)
                        {
                            const auto& keyFrame = channel->m_KeyFrames[j];
                            if (ImGui::TreeNode((void*)(intptr_t)j, "KeyFrame %u", static_cast<unsigned>(j)))
                            {
                                ImGui::Text("    Time: %f", keyFrame.fTime);
                                ImGui::Text("    Scale: [%f, %f, %f]", keyFrame.vScale.x, keyFrame.vScale.y, keyFrame.vScale.z);
                                ImGui::Text("    Rotation: [%f, %f, %f, %f]", keyFrame.vRotation.x, keyFrame.vRotation.y, keyFrame.vRotation.z, keyFrame.vRotation.w);
                                ImGui::Text("    Position: [%f, %f, %f]", keyFrame.vPosition.x, keyFrame.vPosition.y, keyFrame.vPosition.z);
                                ImGui::TreePop();
                            }
                        }
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }

        ImGui::Checkbox("Loop", &isLoop);
        if (ImGui::Button("Play"))
        {
            m_pSelectedModelCom->SetUp_Animation(g_CurrentAnimationIndex, isLoop);

            m_bMotionPlaying = true;
            m_bCurrentPositionSlide = true;

        }
        if (ImGui::Button("Pause"))
        {
            m_bMotionPlaying = false;

        }

        if (ImGui::Button("Stop"))
        {

            m_bMotionPlaying = false;
            m_bCurrentPositionSlide = false;
            m_pSelectedModelCom->m_fCurrentAnimPosition = 0.f;
        }
    }
}


 
void CIMGUI_Animation_Tab::SelectedModelLoad(std::ifstream& inFile, BoneData& bone)
{
    m_pSelectedModelCom->LoadBoneFromBinary(inFile, bone);
}



//void CIMGUI_Animation_Tab::LoadBoneFromBinary_Tool(std::ifstream& inFile, BoneData& bone) {
//    // BoneData�� �̸� �б�
//    size_t nameLength;
//    inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
//    bone.name.resize(nameLength);
//    inFile.read(&bone.name[0], nameLength);
//    
//    // BoneData�� ��ȯ ��� �б�
//    inFile.read(reinterpret_cast<char*>(&bone.transformationMatrix), sizeof(XMFLOAT4X4));
//    
//    // BoneData�� �ڽ� ���� �б�
//    uint32_t numChildren;
//    inFile.read(reinterpret_cast<char*>(&numChildren), sizeof(numChildren));
//    bone.children.resize(numChildren);
//    for (auto& child : bone.children) {
//        LoadBoneFromBinary_Tool(inFile, child);
//    }
//}

void CIMGUI_Animation_Tab::LoadFromFile(const std::string& filename)
{
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    // ModelHeader �б�
    ModelHeader header;
    inFile.read(reinterpret_cast<char*>(&header), sizeof(header));
    std::cout << "Loaded header: " << header.numMeshes << " meshes, "
        << header.numMaterials << " materials, "
        << header.numAnimations << " animations" << std::endl;

    std::vector<MeshData> meshes(header.numMeshes);
    for (auto& mesh : meshes) {
        // Mesh �̸� �б�
        size_t nameLength;
        inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        mesh.name.resize(nameLength);
        inFile.read(&mesh.name[0], nameLength);

        // Vertex�� Index ���� �б�
        inFile.read(reinterpret_cast<char*>(&mesh.vertexCount), sizeof(mesh.vertexCount));
        inFile.read(reinterpret_cast<char*>(&mesh.indexCount), sizeof(mesh.indexCount));

        // Anim ���ο� ���� Vertex ������ �б�
        if (header.isAnim) {
            mesh.animVertices.resize(mesh.vertexCount);
            inFile.read(reinterpret_cast<char*>(mesh.animVertices.data()), mesh.vertexCount * sizeof(VertexAnim));
        }
        else {
            mesh.vertices.resize(mesh.vertexCount);
            inFile.read(reinterpret_cast<char*>(mesh.vertices.data()), mesh.vertexCount * sizeof(Vertex));
        }

        // Index ������ �б�
        mesh.indices.resize(mesh.indexCount);
        inFile.read(reinterpret_cast<char*>(mesh.indices.data()), mesh.indexCount * sizeof(unsigned int));

        // Material index �б�
        inFile.read(reinterpret_cast<char*>(&mesh.materialIndex), sizeof(mesh.materialIndex));

        if (header.isAnim) {
            // ���� ���� �б�
            uint32_t numBones;
            inFile.read(reinterpret_cast<char*>(&numBones), sizeof(numBones));
            mesh.bones.resize(numBones);
            for (auto& bone : mesh.bones) {
                //LoadBoneFromBinary(inFile, bone);
               m_pSelectedModelCom->LoadBoneFromBinary(inFile, bone);

            }

           //SelectedModelLoad(inFile, bone);
           //m_pSelectedModelCom->LoadBoneFromBinary(inFile, bone);


            // ������ ��� �б�
            uint32_t numOffsetMatrices;
            inFile.read(reinterpret_cast<char*>(&numOffsetMatrices), sizeof(numOffsetMatrices));
            mesh.offsetMatrices.resize(numOffsetMatrices);
            inFile.read(reinterpret_cast<char*>(mesh.offsetMatrices.data()), numOffsetMatrices * sizeof(XMFLOAT4X4));
        }
    }

    // MaterialData �б�
    std::vector<MaterialData> materials(header.numMaterials);
    for (auto& material : materials) {
        for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; ++i) {
            size_t pathLength;
            inFile.read(reinterpret_cast<char*>(&pathLength), sizeof(pathLength));
            material.texturePaths[i].resize(pathLength);
            inFile.read(&material.texturePaths[i][0], pathLength);

            std::cout << "Loaded texture path: " << material.texturePaths[i] << std::endl;
        }
    }

    // AnimationData �б�
    std::vector<AnimationData> animations(header.numAnimations);
    if (header.isAnim) {
        for (auto& anim : animations) {
            // Animation �̸� �б�
            size_t nameLength;
            inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
            anim.name.resize(nameLength);
            inFile.read(&anim.name[0], nameLength);

            // Animation duration�� ticks per second �б�
            inFile.read(reinterpret_cast<char*>(&anim.duration), sizeof(anim.duration));
            inFile.read(reinterpret_cast<char*>(&anim.ticksPerSecond), sizeof(anim.ticksPerSecond));

            // Animation ä�� �б�
            uint32_t numChannels;
            inFile.read(reinterpret_cast<char*>(&numChannels), sizeof(numChannels));
            anim.channels.resize(numChannels);
            for (auto& channel : anim.channels) {
                // ä�� �̸� �б�
                size_t channelNameLength;
                inFile.read(reinterpret_cast<char*>(&channelNameLength), sizeof(channelNameLength));
                channel.name.resize(channelNameLength);
                inFile.read(&channel.name[0], channelNameLength);

                // Keyframes �б�
                uint32_t numKeyframes;
                inFile.read(reinterpret_cast<char*>(&numKeyframes), sizeof(numKeyframes));
                channel.keyframes.resize(numKeyframes);
                inFile.read(reinterpret_cast<char*>(channel.keyframes.data()), numKeyframes * sizeof(Keyframe));
            }
        }
    }

    inFile.close();
}

void CIMGUI_Animation_Tab::ProcessEventsBetweenFrames(int characterIndex, int animationIndex, float prevFrame, float currentFrame)
{
    // ĳ���� �ε��� Ž��
    auto characterIt = FrameEvent.find(SELECT_HIT);   //�׽�Ʈ��
    if (characterIt != FrameEvent.end()) 
    {
        auto& animationMap = characterIt->second;
        auto animationIt = animationMap.find(animationIndex);
        if (animationIt != animationMap.end()) 
        {
            auto& frameMap = animationIt->second;


            //list�� void*�� ���� �ڵ�
            
            // ���� �����Ӱ� ���� ������ ���̿� �ִ� ��� �̺�Ʈ ó��
            //for (auto frameIt = frameMap.lower_bound(prevFrame); frameIt != frameMap.end() && frameIt->first <= currentFrame; ++frameIt) {
            //    // �ش� �����ӿ����� �̺�Ʈ ����Ʈ�� ���
            //    for (void* event : frameIt->second) {
            //        std::cout << "������ " << frameIt->first << "���� �̺�Ʈ �߻�: " << event << std::endl;
            //    }
            //}


            //�׽�Ʈ�Ѵ�ð� string ���� �ڵ�
            for (auto frameIt = frameMap.lower_bound(prevFrame); frameIt != frameMap.end() && frameIt->first <= currentFrame; ++frameIt) 
            {
                // �ش� �����ӿ����� �̺�Ʈ ����Ʈ�� ���
                for (string event : frameIt->second) 
                {
                    //MSG_BOX(TEXT("������ " + frameIt->first + "���� �̺�Ʈ �߻�: " + event));

                    _bool bdebug1 = false;

                    m_pSelectedModelCom->m_Animations[m_pSelectedModelCom->m_iCurrentAnimationIndex]->m_fTickPerSecond = 100.f;
                    //���� 25���� ���ƿ��� �ʴ� ������ ����...?


                }
            }

        }
    }
}

CIMGUI_Animation_Tab* CIMGUI_Animation_Tab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIMGUI_Animation_Tab* pInstance = new CIMGUI_Animation_Tab(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CIMGUI_Animation_Tab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIMGUI_Animation_Tab::Free()
{
	__super::Free();

    if (m_pSelectedModelCom != nullptr)
    {
        Safe_Release(m_pSelectedModelCom);
    }
}
