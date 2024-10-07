#include "stdafx.h"

#include <algorithm> // std::remove
#include <sstream>
#include <algorithm>
#include <cmath>
#include <locale>
#include <codecvt>
#include <string>
#include <iostream>

#include "IMGUI_Shader_Tab.h"
#include "GameInstance.h"
#include "RenderInstance.h"

CIMGUI_Shader_Tab::CIMGUI_Shader_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CIMGUI_Tab{ pDevice,pContext }
{
}

HRESULT CIMGUI_Shader_Tab::Initialize()
{
	return S_OK;
}

void CIMGUI_Shader_Tab::Render(_float fTimeDelta)
{


    if (ImGui::Button("Add Rect") && !isStart)
    {
        //몬스터 생성
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Rect"), TEXT("Layer_Effect_Rect"))))
            return;

        isStart = true;
        DragAcceptFiles(g_hWnd, TRUE);
    }
    

    ImGui::Separator();

    // Node addition button
    if (ImGui::Button("Add Node") && isStart)
    {
        node_ids.push_back(unique_node_id++); // Add a new node
        node_values[unique_node_id - 1] = 0;  // Initialize new node value to 0
        input_accumulated_values[unique_node_id - 1] = 0; // Initialize accumulated value for new node
    }

    ImNodes::BeginNodeEditor(); // Start node editor scope

    for (size_t i = 0; i < node_ids.size(); ++i)
    {
        int node_id = node_ids[i];

        ImNodes::BeginNode(node_id);
        ImGui::Text("Node %d", node_id);

        // Input field for the node value
        ImGui::InputInt("Value", &node_values[node_id]); // Input field for node value

        // Output attribute
        ImNodes::BeginOutputAttribute(node_id * 2);
        ImGui::Text("Output");
        ImNodes::EndOutputAttribute();

        // Input attribute
        ImNodes::BeginInputAttribute(node_id * 2 + 1);
        ImGui::Text("Input");
        ImNodes::EndInputAttribute();

        // Display the current value of the node (accumulated if input)
        ImGui::Text("Current Value: %d", node_values[node_id] + input_accumulated_values[node_id]);

        // Display the texture as an image, assuming m_NodeTextureSRVs[i] has valid ImTextureID for the current node
        if (i < m_NodeTextureSRVs.size())
        {
            ImGui::Image(m_NodeTextureSRVs[i], ImVec2(64, 64)); // Display the texture with a size of 64x64 pixels
        }

        ImNodes::EndNode(); // End node
    }

    // Render existing links
    for (const auto& link : links)
    {
        ImNodes::Link(link.first, link.first, link.second); // Link creation
    }

    // End the node editor
    ImNodes::EndNodeEditor(); // End node editor scope

    // Check for new link creation (after ending the editor)
    int start_attr = 0, end_attr = 0; // Initialize
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
    {
        links.push_back(std::make_pair(start_attr, end_attr));
    }

    // Link deletion logic
    int link_id;
    if (ImNodes::IsLinkDestroyed(&link_id))
    {
        links.erase(std::remove_if(links.begin(), links.end(),
            [link_id](const std::pair<int, int>& link) {
                return link.first == link_id || link.second == link_id;
            }), links.end());
    }

    // Reset accumulated input values for each frame
    for (auto& iter : input_accumulated_values)
    {
        iter.second = 0;
       // value = 0; // Reset accumulated value for all input nodes
    }

    // Update the node values based on links
    for (const auto& link : links)
    {
        int output_node = link.first / 2;  // The node providing the output
        int input_node = link.second / 2;   // The node receiving the input

        // Check if both nodes exist
        if (node_values.find(output_node) != node_values.end() &&
            input_accumulated_values.find(input_node) != input_accumulated_values.end())
        {
            // Add the output node value to the input node's accumulated value
            input_accumulated_values[input_node] += node_values[output_node];
        }
    }
}

void CIMGUI_Shader_Tab::Create_NodeTexture(string szPath)
{
    size_t testModelsPos = szPath.find("Shader");

    if (testModelsPos != string::npos)
    {
        string relativePath = szPath.substr(testModelsPos);

        replace(relativePath.begin(), relativePath.end(), '\\', '/');

        string fullPath = string("../Bin/Resources/") + relativePath;
        wstring_convert<codecvt_utf8_utf16<_tchar>> converter;

        wstring prototypeKey = TEXT("Prototype_Component_Texture_Shader_");
        wstring prototypeKeyWithCount = prototypeKey + to_wstring(m_iNodeTextureCount);

        CShader_Texture::SHADER_TEXTURE_DESC tDesc{};
        tDesc.prototypeKey = prototypeKeyWithCount.c_str();

        if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, prototypeKeyWithCount.c_str(),
            CTexture::Create(m_pDevice, m_pContext, converter.from_bytes(fullPath).c_str(), 1))))
            return;

        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Shader_Texture"), TEXT("Layer_Shader_Texture"), &tDesc)))
	        return;

        _float2 fTextureSize = m_NodeTextures.back()->m_pTextureCom->Get_TextureSize();
//        m_pRenderInstance->Add_ClientRenderTarget(prototypeKeyWithCount.c_str(), prototypeKeyWithCount.c_str(), fTextureSize.x, fTextureSize.y, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(1.f, 1.f, 1.f, 0.f));
        m_pRenderInstance->Add_ClientRenderTarget(prototypeKeyWithCount.c_str(), prototypeKeyWithCount.c_str(), 1280, 720, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(1.f, 1.f, 1.f, 0.f));
        m_NodeTextureSRVs.push_back((ImTextureID)m_pRenderInstance->Copy_RenderTarget_SRV(prototypeKeyWithCount.c_str()));

        node_ids.push_back(unique_node_id++);            
        node_values[unique_node_id - 1] = 0;             
        input_accumulated_values[unique_node_id - 1] = 0;

        DragAcceptFiles(g_hWnd, TRUE);
        m_iNodeTextureCount++;
    }
}



CIMGUI_Shader_Tab* CIMGUI_Shader_Tab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIMGUI_Shader_Tab* pInstance = new CIMGUI_Shader_Tab(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CIMGUI_Shader_Tab"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIMGUI_Shader_Tab::Free()
{
	__super::Free();
}
