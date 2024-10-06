#include "stdafx.h"
#include "IMGUI_Animation_Tab.h"

#include "imgui.h"

#include <fstream>  
#include <iostream> 
#include <vector>   
#include <string>   

//#include "Windows.h"
//#include "shellapi.h"

#include "Animation.h"
#include "Channel.h"


CIMGUI_Animation_Tab::CIMGUI_Animation_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CIMGUI_Tab{ pDevice,pContext }
{
}

HRESULT CIMGUI_Animation_Tab::Initialize()
{
	return S_OK;
}

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
    
    if (m_pSelectedModelCom != nullptr)
    {
         m_pSelectedModelCom->m_Animations;
         Info_Anim();

         if (m_bMotionPlaying)
         {
             m_pSelectedModelCom->Play_Animation(fTimeDelta);
             ImGui::Text("CurrentAnimPosition :%f " ,(m_pSelectedModelCom->m_fCurrentAnimPosition) );

         }

         if (m_bCurrentPositionSlide)
         {
             CreateSlider(m_pSelectedModelCom->m_fCurrentAnimPosition, m_pSelectedModelCom->m_fBlendDuration);
         }

    }

    _bool DebugPoint = false;

}

void CIMGUI_Animation_Tab::Info_Anim()
{
    //999�� �� ������ 0���� �ϸ� ������ 
    static int currentIndex = 999;

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
    ImGui::Combo("##", &currentIndex, animNames.data(), animNames.size());

    if (currentIndex < animNames.size())
    {
        // ���� ���õ� �ִϸ��̼� �����͸� ������
         
        //Ver.�¿�
        //const auto& selectedAnim = m_pSelectedModelCom->m_ModelData.animations[currentIndex];


        const auto& selectedAnim = m_pSelectedModelCom->m_Animations[currentIndex];

        // �ִϸ��̼��� �� ����� ImGui::Text�� ǥ��
        ImGui::Text("AnimIndex: %d", currentIndex);
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
            m_pSelectedModelCom->SetUp_Animation(currentIndex, isLoop);
            //m_pSelectedModelCom->Play_Animation(0.1f);

            m_bMotionPlaying = true;

            CreateSlider(50, 100);

            ImGui::Text("Current Value: %.0f", 50);
            m_bCurrentPositionSlide = true;

        }
        if (ImGui::Button("Stop"))
        {

            m_bMotionPlaying = false;
            m_bCurrentPositionSlide = false;
        }
    }
}

void CIMGUI_Animation_Tab::CreateSlider(_float currentValue, _float maxValue)
{
    // Define the slider length
    const _float sliderLength = 300.0f;

    // Calculate the normalized position of the current value
    //_float normalizedValue = currentValue / maxValue;
    // Calculate the slider position based on the normalized value
    //float sliderPosition = normalizedValue * sliderLength;

    // Create a slider using ImGui
    //ImGui::SliderFloat("Slider", &currentValue, 0, maxValue, "%.0f", ImGuiSliderFlags_AlwaysClamp);

    // Set the current value to the calculated position
   // ImGui::SetCursorPos(ImVec2(sliderPosition, ImGui::GetCursorPosY()));

    ImGui::SliderFloat("Slider", &currentValue, 0, maxValue, "%.0f", ImGuiSliderFlags_AlwaysClamp);

    

}




//void CIMGUI_Animation_Tab::LoadModelDataFromBinary(){}
 
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
