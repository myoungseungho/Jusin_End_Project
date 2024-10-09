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

CIMGUI_Shader_Tab::CIMGUI_Shader_Tab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CTexture* pTexture)
	:CIMGUI_Tab{ pDevice,pContext },
    m_TestEffectModel_Texture{ pTexture }
{
}

HRESULT CIMGUI_Shader_Tab::Initialize()
{
    DragAcceptFiles(g_hWnd, TRUE);
    isStart = true;
	return S_OK;
}

void CIMGUI_Shader_Tab::Render(_float fTimeDelta)
{
    //if (ImGui::Button("Add Rect") && !isStart)
    //{
    //    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Rect"), TEXT("Layer_Effect_Rect"))))
    //        return;

    //    isStart = true;
    //    DragAcceptFiles(g_hWnd, TRUE);
    //}

    ImGui::Separator();

    if (ImGui::Button("Add MoveTex_Node") && isStart)
    {
        MoveTex_Node nodeDesc{};
        nodeDesc.MoveTex_node_id = m_MoveTex_node_id++;
        m_MoveTex_Node_ids.push_back(nodeDesc);
    }

    ImNodes::BeginNodeEditor();  /* 노드 생성시 무조건 호출해야함 */

    Render_MainNode();

    Render_TextureNode();

    Render_MoveTexNode();

    // 노드들끼리 연결된 모든 애들을 시각화 하기 위한 함수
    for (size_t i = 0; i < links.size(); i++)
    {
        ImNodes::Link(i, links[i].first, links[i].second);
    }
    /*for (const auto& link : links)
        ImNodes::Link(link.first, link.first, link.second);*/

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_TopRight);

    ImNodes::EndNodeEditor(); /* 노드 끝 */

    // start 주는놈 end 받는놈
    _int start_attr = 0, end_attr = 0;
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
    {
        auto it = find_if(links.begin(), links.end(),
            [start_attr](const pair<_int, _int>& link) {
                return link.first == start_attr;
            });
        //Links 생성 (push_back) 한 친구는 각각 그때의 start_attr와 end_attr로 상호작용을 했었음\
        // 그래서 다시 삭제할때는 그때 애들 저장해서 원상태로 복구해주는 작업 필요
        if (it != links.end())
        {
            if (start_attr > 1500)
            {

            }

            links.erase(it);
        }


        if (start_attr > 1500 + (m_iNumberId * 10000))
        {
            links.push_back(make_pair(start_attr, end_attr));
            m_NodeTextures[(((m_iNumberId * 10000) - end_attr - 2) / 3 - 1) ]->Push_Shade_MoveTex(&m_MoveTex_Node_ids[(start_attr - (m_iNumberId * 10000) - 1 - 1501)].fDirection, &m_MoveTex_Node_ids[(start_attr - (m_iNumberId * 10000) - 1 - 1501)].fSpeed);
        }
        // Main_Node Diffuse에 연결된거임
        else if (end_attr == -2 + (m_iNumberId * 10000))
        {
            m_iMain_Input_Diffuse_id = (start_attr - (m_iNumberId * 10000)) / 3;
            links.push_back(make_pair(start_attr, end_attr));

            m_TestEffectModel_Texture->Set_SRV((ID3D11ShaderResourceView*)m_NodeTextureSRVs[m_iMain_Input_Diffuse_id - 1]);
            //m_Effect_Rect->Push_Texture_Diffuse((ID3D11ShaderResourceView*)m_NodeTextureSRVs[m_iMain_Input_Diffuse_id - 1], 0);
        }
        else
        {
            links.push_back(make_pair(start_attr, end_attr));
            m_NodeTextures[(end_attr - (m_iNumberId * 10000) -1)/3 - 1]->Push_InputTextures((ID3D11ShaderResourceView*)m_NodeTextureSRVs[(start_attr - (m_iNumberId * 10000)) / 3 - 1]);
        }
    }

    _int link_id;
    if (ImNodes::IsLinkDestroyed(&link_id))
    {
        links.erase(std::remove_if(links.begin(), links.end(),
            [link_id](const pair<int, int>& link) {
                return link.first == link_id || link.second == link_id;
            }), links.end());
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
        wstring prototypeKeyWithCount = prototypeKey + to_wstring(m_iNodeTextureCount) + TEXT("_") + to_wstring(m_iNumberId);

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
            _float fDiff = g_iWinSizeX - fTextureSize.x;
            fTextureSize.x += fDiff;
            fTextureSize.y += fDiff;
        }

        if (fTextureSize.y > g_iWinSizeY)
        {
            _float fDiff = g_iWinSizeY - fTextureSize.y;
            fTextureSize.x += fDiff;
            fTextureSize.y += fDiff;
        }

        m_pRenderInstance->Add_ClientRenderTarget(prototypeKeyWithCount.c_str(), prototypeKeyWithCount.c_str(), fTextureSize.x, fTextureSize.y, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(1.f, 1.f, 1.f, 0.f));
        
        m_NodeTextureSRVs.push_back((ImTextureID)m_pRenderInstance->Copy_RenderTarget_SRV(prototypeKeyWithCount.c_str()));

        node_ids.push_back(unique_node_id++);

        DragAcceptFiles(g_hWnd, TRUE);
        m_iNodeTextureCount++;
    }
}

void CIMGUI_Shader_Tab::Render_MainNode()
{
    if (isStart == true)
    {
        ImNodes::BeginNode(m_iNumberId * 10000 +  m_iMain_node_id);
        ImGui::Text("Main_Node");

        // Input attribute
        ImNodes::BeginInputAttribute(m_iNumberId * 10000  + m_iMain_node_id * 2);
        ImGui::Text("Diffuse");
        ImNodes::EndInputAttribute();

        ImNodes::BeginInputAttribute(m_iNumberId * 10000 + m_iMain_node_id * 2 - 1);
        ImGui::Text("Alpha");
        ImNodes::EndInputAttribute();

        ImNodes::EndNode();
    }
}

void CIMGUI_Shader_Tab::Render_TextureNode()
{
    for (size_t i = 0; i < node_ids.size(); ++i)
    {
        int node_id = node_ids[i];

        ImNodes::BeginNode(m_iNumberId * 10000 + node_id);
        ImGui::Text("Node %d", node_id);

        ImNodes::BeginOutputAttribute(m_iNumberId * 10000 + node_id * m_iAttributeCount);
        ImGui::Text("OutColor");
        ImNodes::EndOutputAttribute();

        ImNodes::BeginInputAttribute(m_iNumberId * 10000 + node_id * m_iAttributeCount + 1);
        ImGui::Text("Alpha");
        ImNodes::EndInputAttribute();

        if (i < m_NodeTextureSRVs.size())
        {
            ImGui::Image(m_NodeTextureSRVs[i], ImVec2(150, 150));
        }

        ImNodes::BeginInputAttribute(m_iNumberId * 10000 + node_id * m_iAttributeCount + 2);
        ImGui::Text("ShadeFunction");
        ImNodes::EndInputAttribute();

        ImNodes::EndNode();
    }
}

void CIMGUI_Shader_Tab::Render_MoveTexNode()
{
    for (size_t i = 0; i < m_MoveTex_Node_ids.size(); ++i)
    {
        int node_id = m_MoveTex_Node_ids[i].MoveTex_node_id;

        ImNodes::BeginNode(m_iNumberId * 10000 + node_id);

        ImGui::Dummy(ImVec2(1, 1));

        ImGui::Text("MoveTex_Node %d", node_id - 1500);

        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat2("Direction", &m_MoveTex_Node_ids[i].fDirection.x);

        ImGui::SetNextItemWidth(50);
        ImGui::DragFloat("Speed", &m_MoveTex_Node_ids[i].fSpeed, 0.1f, 0.0f, 30.0f, "%.2f");
        
        ImNodes::BeginOutputAttribute(m_iNumberId * 10000 + node_id + 1, 2);
        ImGui::Text("Out");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();
    }
}



CIMGUI_Shader_Tab* CIMGUI_Shader_Tab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CTexture* pTexture)
{
    CIMGUI_Shader_Tab* pInstance = new CIMGUI_Shader_Tab(pDevice, pContext, pTexture);

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
