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
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Rect"), TEXT("Layer_Effect_Rect"))))
            return;

        isStart = true;
        DragAcceptFiles(g_hWnd, TRUE);
    }

    ImGui::Separator();

    // Node addition button
    if (ImGui::Button("Add Node") && isStart)
    {
        node_ids.push_back(unique_node_id++); 
        node_values[unique_node_id - 1] = 0; 
        input_accumulated_values[unique_node_id - 1] = 0;
    }

    /* 노드 생성시 무조건 호출해야함 */
    ImNodes::BeginNodeEditor();

    if (isStart == true)
    {
        ImNodes::BeginNode(m_iMain_node_id);
        ImGui::Text("Main_Node");

        // Input attribute
        ImNodes::BeginInputAttribute(m_iMain_node_id * 2);
        ImGui::Text("Diffuse");
        ImNodes::EndInputAttribute();

        ImNodes::BeginInputAttribute(m_iMain_node_id * 2 - 1);
        ImGui::Text("Alpha");
        ImNodes::EndInputAttribute();

        ImNodes::EndNode();
    }

    for (size_t i = 0; i < node_ids.size(); ++i)
    {
        int node_id = node_ids[i];

        ImNodes::BeginNode(node_id);
        ImGui::Text("Node %d", node_id);

        ImGui::InputInt("Value", &node_values[node_id]);

        ImNodes::BeginOutputAttribute(node_id * 2);
        ImGui::Text("Output");
        ImNodes::EndOutputAttribute();

        ImNodes::BeginInputAttribute(node_id * 2 + 1);
        ImGui::Text("Input");
        ImNodes::EndInputAttribute();

        ImGui::Text("Current Value: %d", node_values[node_id] + input_accumulated_values[node_id]);

        if (i < m_NodeTextureSRVs.size())
        {
            ImGui::Image(m_NodeTextureSRVs[i], ImVec2(64, 64));
        }

        ImNodes::EndNode();
    }

    for (const auto& link : links)
    {
        ImNodes::Link(link.first, link.first, link.second);
    }

    ImNodes::EndNodeEditor();

    _int start_attr = 0, end_attr = 0;
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
    {
        // Main_Node Diffuse에 연결된거임
        if (end_attr == -2)
        {
            m_iMain_Input_Diffuse_id = start_attr / 2;
            m_Effect_Rect->Push_Texture_Diffuse((ID3D11ShaderResourceView*)m_NodeTextureSRVs[m_iMain_Input_Diffuse_id - 1], 0);
        }


        links.push_back(std::make_pair(start_attr, end_attr));
    }

    _int link_id;
    if (ImNodes::IsLinkDestroyed(&link_id))
    {
        links.erase(std::remove_if(links.begin(), links.end(),
            [link_id](const std::pair<int, int>& link) {
                return link.first == link_id || link.second == link_id;
            }), links.end());
    }

    for (auto& iter : input_accumulated_values)
    {
        iter.second = 0;
    }

    for (const auto& link : links)
    {
        int output_node = link.first / 2;  
        int input_node = link.second / 2;  

        if (node_values.find(output_node) != node_values.end() &&
            input_accumulated_values.find(input_node) != input_accumulated_values.end())
        {
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

        if (fTextureSize.x > g_iWinSizeX)
        {
            _float fDiff = 1280 - fTextureSize.x;
            fTextureSize.x += fDiff;
            fTextureSize.y += fDiff;
        }

        if (fTextureSize.y > g_iWinSizeY)
        {
            _float fDiff = 720 - fTextureSize.y;
            fTextureSize.x += fDiff;
            fTextureSize.y += fDiff;
        }

        m_pRenderInstance->Add_ClientRenderTarget(prototypeKeyWithCount.c_str(), prototypeKeyWithCount.c_str(), fTextureSize.x, fTextureSize.y, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(1.f, 1.f, 1.f, 0.f));
        
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
