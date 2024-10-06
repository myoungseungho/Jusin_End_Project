#include "stdafx.h"
#include "IMGUI_Animation_Tab.h"

#include "imgui.h"

#include <fstream>  
#include <iostream> 
#include <vector>   
#include <string>   

//#include "Windows.h"
//#include "shellapi.h"


CIMGUI_Animation_Tab::CIMGUI_Animation_Tab()
{
}

HRESULT CIMGUI_Animation_Tab::Initialize()
{
	return S_OK;
}

void CIMGUI_Animation_Tab::Render(_float fTimeDelta)
{
   
    if (ImGui::Button("DragMod"))
    {
       // m_bDrag = !m_bDrag;
       // DragAcceptFiles(g_hWnd, m_bDrag);
    }

    ImGui::InputText("Label", buffer, IM_ARRAYSIZE(buffer));

    if (ImGui::Button("Label Load"))
    {
        //strncpy(buffer, inputText.c_str(), sizeof(buffer));

        if (m_pSelectedModelCom != nullptr)
            Safe_Release(m_pSelectedModelCom);

        _fmatrix preMatrix = XMMatrixIdentity();
       // m_pSelectedModelCom = CModel::Create(m_pDevice, m_pContext, buffer, preMatrix);

        
    }
    

}

void CIMGUI_Animation_Tab::Info_Anim()
{

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


CIMGUI_Animation_Tab* CIMGUI_Animation_Tab::Create()
{
	CIMGUI_Animation_Tab* pInstance = new CIMGUI_Animation_Tab();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CIMGUI_Animation_Tab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIMGUI_Animation_Tab::Free()
{
}
