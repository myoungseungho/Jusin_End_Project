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
    for (auto& iter : m_NodeTextures)
    {
        iter->Priority_Update(fTimeDelta);
        iter->Update(fTimeDelta);
    }
       
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

    Render_Link();

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_TopRight);

    ImNodes::EndNodeEditor(); /* 노드 끝 */

    Check_Create_Link(); // 생성할때는 노드에디터가 끝날때 체크
    Check_Delete_Link(); // 삭제하려할땐 전에

    _int link_id;
    if (ImNodes::IsLinkDestroyed(&link_id))
    {
        links.erase(std::remove_if(links.begin(), links.end(),
            [link_id](const pair<int, int>& link) {
                return link.first == link_id || link.second == link_id;
            }), links.end());
    }
    for (auto& iter : m_NodeTextures)
    {
        iter->Late_Update(fTimeDelta);
    }
}

void CIMGUI_Shader_Tab::TabPos_Init()
{
    for (const auto& node : node_positions)
    {
        int node_id = node.first;      // 노드 ID
        ImVec2 node_pos = node.second; // 노드 위치

        // 각 노드의 위치 설정
        ImNodes::SetNodeEditorSpacePos(node_id, node_pos);
    }
}

void CIMGUI_Shader_Tab::Create_NodeTexture(string szPath)
{
    size_t testModelsPos = szPath.find("ModelData");

    if (testModelsPos != string::npos)
    {
        string relativePath = szPath.substr(testModelsPos);

        replace(relativePath.begin(), relativePath.end(), '\\', '/');

        string fullPath = string("../Bin/") + relativePath;
        wstring_convert<codecvt_utf8_utf16<_tchar>> converter;

        wstring prototypeKey = TEXT("Prototype_Component_Texture_Shader_");
        wstring prototypeKeyWithCount = prototypeKey + to_wstring(m_iNodeTextureCount) + TEXT("_") + to_wstring(m_iNumberId);

        CShader_Texture::SHADER_TEXTURE_DESC tDesc{};
        tDesc.prototypeKey = prototypeKeyWithCount.c_str();

        if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, prototypeKeyWithCount.c_str(),
            CTexture::Create(m_pDevice, m_pContext, converter.from_bytes(fullPath).c_str(), 1))))
            return;

        CGameObject* pPrototype = m_pGameInstance->Find_Prototype(TEXT("Prototype_GameObject_Shader_Texture"));
        static_cast<CShader_Texture*>(pPrototype->Clone((void*)&tDesc));
 


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
        
        SRV_Texture SRVDesc{};
        SRVDesc.iID = unique_node_id;
        SRVDesc.Texture = (ImTextureID)m_pRenderInstance->Copy_RenderTarget_SRV(prototypeKeyWithCount.c_str());
        m_NodeTextureSRVs.push_back(SRVDesc);

        m_NodeTextures.back()->m_iID = unique_node_id;
        node_ids.push_back(unique_node_id++);

        DragAcceptFiles(g_hWnd, TRUE);
        m_iNodeTextureCount++;
    }
}

void CIMGUI_Shader_Tab::Render_MainNode()
{
    if (isStart == true)
    {
        //ImNodes::SetNodeEditorSpacePos(m_iMain_node_id, node_positions[m_iMain_node_id]);

        ImNodes::BeginNode(m_iMain_node_id);
        ImGui::Text("Main_Node");

        // Input attribute
        ImNodes::BeginInputAttribute( m_iMain_node_id * 2);
        ImGui::Text("Diffuse");
        ImNodes::EndInputAttribute();

        ImNodes::BeginInputAttribute(  m_iMain_node_id * 2 - 1);
        ImGui::Text("Alpha");
        ImNodes::EndInputAttribute();

        ImNodes::EndNode();


        ImVec2 current_pos = ImNodes::GetNodeEditorSpacePos(m_iMain_node_id);

        if (node_positions[m_iMain_node_id].x != current_pos.x ||
            node_positions[m_iMain_node_id].y != current_pos.y)
        {
            node_positions[m_iMain_node_id] = current_pos;
        }

        //
    }
}

void CIMGUI_Shader_Tab::Render_TextureNode()
{
    for (size_t i = 0; i < node_ids.size(); ++i)
    {
        int node_id = node_ids[i];

        ImNodes::BeginNode(  node_id);
        ImGui::Text("Node %d", node_id);

        ImNodes::BeginOutputAttribute(node_id * m_iAttributeCount);
        ImGui::Text("OutColor");
        ImNodes::EndOutputAttribute();

        ImNodes::BeginInputAttribute(node_id * m_iAttributeCount + 1);
        ImGui::Text("Diffuse");
        ImNodes::EndInputAttribute();

        ImNodes::BeginInputAttribute(node_id * m_iAttributeCount + 2);
        ImGui::Text("Alpha");
        ImNodes::EndInputAttribute();

        if (i < m_NodeTextureSRVs.size())
        {
            ImGui::Image(m_NodeTextureSRVs[i].Texture, ImVec2(150, 150));
        }

        ImNodes::BeginInputAttribute(  node_id * m_iAttributeCount + 3);
        ImGui::Text("ShadeFunction");
        ImNodes::EndInputAttribute();

        ImNodes::EndNode();

        ImVec2 current_pos = ImNodes::GetNodeEditorSpacePos(node_id);

        if (node_positions[node_id].x != current_pos.x ||
            node_positions[node_id].y != current_pos.y)
        {
            node_positions[node_id] = current_pos;
        }
    }
}

void CIMGUI_Shader_Tab::Render_MoveTexNode()
{
    for (auto& iter : m_MoveTex_Node_ids)
    {
        int node_id = iter.MoveTex_node_id;
        
        ImNodes::BeginNode(  node_id);

        ImGui::Dummy(ImVec2(1, 1));

        ImGui::Text("MoveTex_Node %d", node_id - 1500);

        ImGui::SetNextItemWidth(50);
        ImGui::InputFloat2("Direction", &iter.fDirection.x);

        ImGui::SetNextItemWidth(50);
        ImGui::DragFloat("Speed", &iter.fSpeed, 0.1f, 0.0f, 30.0f, "%.2f");
        
        ImNodes::BeginOutputAttribute(node_id + 1, 2);
        ImGui::Text("Out");
        ImNodes::EndOutputAttribute();

        ImNodes::EndNode();

        ImVec2 current_pos = ImNodes::GetNodeEditorSpacePos(node_id);

        if (node_positions[node_id].x != current_pos.x ||
            node_positions[node_id].y != current_pos.y)
        {
            node_positions[node_id] = current_pos;
        }
    }
}

void CIMGUI_Shader_Tab::Render_Link()
{
    // 노드들끼리 연결된 모든 애들을 시각화 하기 위한 함수
    for (size_t i = 0; i < links.size(); i++)
    {
        ImNodes::Link(i, links[i].first, links[i].second);
    }
}

void CIMGUI_Shader_Tab::Check_Delete_Link()
{
    static NODE_TYPE eNodeType = NODE_END;
    static int iID = -999;

    if (m_pGameInstance->Get_DIMouseState(DIMK_LBUTTON))
    {
        /*vector<pair<_int, _int>> links*/
        for (_int i = 0; i < links.size(); i++)
        {
            if (ImNodes::IsLinkHovered(&i))
            {
                iID = i;
                eNodeType = NODE_LINK;
                return;
            }
        }
        /*vector<_int> node_ids*/
        for (auto iter : node_ids)
        {
            if (ImNodes::IsNodeHovered(&iter))
            {
                if (iter == -1)
                    return;
                else if (iter > 1500)
                {
                    iID = iter;
                    eNodeType = NODE_FUNCTION;
                    return;
                }

                iID = iter;
                eNodeType = NODE_BASIC;
                return;
            }
        }
        ///*list<MoveTex_Node> m_MoveTex_Node_ids*/
        //for (auto& iter : m_MoveTex_Node_ids)
        //{
        //    if (ImNodes::IsNodeHovered(&iter.MoveTex_node_id))
        //    {
        //        iID = iter.MoveTex_node_id;
        //        eNodeType = NODE_FUNCTION;
        //        return;
        //    }
        //}
    }
    
    if (m_pGameInstance->Get_DIKeyState(DIK_DELETE) && eNodeType != NODE_END)
    {
        //id 일단 저장
        _int node_id = iID;

        switch (eNodeType)
        {
        case NODE_BASIC:
        {
            //Node 부터 지움
            node_ids.erase(remove(node_ids.begin(), node_ids.end(), iID), node_ids.end());

            // m_NodeTextures에 있는거 찾아서 삭제
            auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                [&](CShader_Texture* texture) {
                    return texture->m_iID == iID;
                });
            if (it != m_NodeTextures.end())
            {
                (*it)->Free();
                m_NodeTextures.erase(it);
            }

            // m_NodeTextureSRVs는 SRV는 내비두고 삭제
            auto iter = std::find_if(m_NodeTextureSRVs.begin(), m_NodeTextureSRVs.end(),
                [&](const SRV_Texture& srv) {
                    return srv.iID == iID;
                });
            if (iter != m_NodeTextureSRVs.end())
            {
                m_NodeTextureSRVs.erase(iter);
            }
            
            // 이 노드가 다른 노드에게 영향을 줬을수도 있음 그 타겟노드아이디
            _int target_node_id = node_id * m_iAttributeCount;
            _int target_second_value = -1;
            // 찾아서 영향받은 노드 ID 저장
            auto LinkIter = std::find_if(links.begin(), links.end(),
                [target_node_id](const pair<_int, _int>& link) {
                    return link.first == target_node_id;
                });
            if (LinkIter != links.end()) {
                target_second_value = LinkIter->second;
            }

            if (target_second_value == -2)
            {
                if (m_pDefaultTexture == nullptr)
                {
                    m_pDefaultTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_Default_Diffuse")));
                }
                
                m_TestEffectModel_Texture->Set_SRV(m_pDefaultTexture->Get_SRV(0));
 
            }
            // 그 노드 ID가 알파에 영향을 줬었냐?
            else if (target_second_value % 4 == 2)
            {
                // m_NodeTextures에서 찾아서 삭제
                auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == target_second_value / 4;
                    });
                if (it != m_NodeTextures.end())
                {
                    (*it)->Remove_InputTextures(END_ALPHA);
                }
            }
            // 그 노드 ID가 디퓨즈에 영향을 줬었냐?
            else if (target_second_value % 4 == 1)
            {
                // m_NodeTextures에서 찾아서 삭제
                auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == target_second_value / 4;
                    });
                if (it != m_NodeTextures.end())
                {
                    (*it)->Remove_InputTextures(END_DIFFUSE);
                }
            }

            // 링크 삭제
            links.erase(remove_if(links.begin(), links.end(),
                [node_id](const pair<_int, _int>& link) {
                    return link.first == node_id * 4 || link.second == node_id * 4 ||
                        link.first == node_id * 4 + 1 || link.second == node_id * 4 + 1 ||
                        link.first == node_id * 4 + 2 || link.second == node_id * 4 + 2 ||
                        link.first == node_id * 4 + 3 || link.second == node_id * 4 + 3;
                }), links.end());

            iID = -999;
            eNodeType = NODE_END;
            break;
        }
        case NODE_LINK:
        {
            auto linkPos = links.begin() + iID;

            // 혹시 함수?
            if (linkPos->first > 1500)
            {
                // m_NodeTextures에서 찾아서 삭제
                auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == linkPos->second / 4;
                    });
                if (it != m_NodeTextures.end())
                {
                    (*it)->Remove_InputFunction(FUNCTION_TEXMOVE);
                }
            }
            // 혹시 메인노드?
            else if (linkPos->second == -2)
            {
                if (m_pDefaultTexture == nullptr)
                {
                    m_pDefaultTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_Default_Diffuse")));
                }

                m_TestEffectModel_Texture->Set_SRV(m_pDefaultTexture->Get_SRV(0));

            }
            // 그 노드 ID가 알파에 영향을 줬었냐?
            else if (linkPos->second % 4 == 2)
            {
                // m_NodeTextures에서 찾아서 삭제
                auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == linkPos->second / 4;
                    });
                if (it != m_NodeTextures.end())
                {
                    (*it)->Remove_InputTextures(END_ALPHA);
                }
            }
            // 그 노드 ID가 디퓨즈에 영향을 줬었냐?
            else if (linkPos->second % 4 == 1)
            {
                // m_NodeTextures에서 찾아서 삭제
                auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == linkPos->second / 4;
                    });
                if (it != m_NodeTextures.end())
                {
                    (*it)->Remove_InputTextures(END_DIFFUSE);
                }
            }

            links.erase(linkPos);

            iID = -999;
            eNodeType = NODE_END;
            break;
        }
        case NODE_FUNCTION:
        {
            // 이 노드가 다른 노드에게 영향을 줬을수도 있음 그 타겟노드아이디
            _int target_node_id = node_id + 1;
            _int target_second_value = -1;
            // 찾아서 영향받은 노드 ID 저장
            auto LinkIter = std::find_if(links.begin(), links.end(),
                [target_node_id](const pair<_int, _int>& link) {
                    return link.first == target_node_id;
                });
            if (LinkIter != links.end()) {
                target_second_value = LinkIter->second;
            }

            // 혹시 메인노드?
            if (target_second_value == -2)
            {

            }
            // 함수 아웃쪽
            else if (target_second_value % 4 == 3)
            {
                // m_NodeTextures에서 찾아서 삭제
                auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == target_second_value / 4;
                    });
                if (it != m_NodeTextures.end())
                {
                    (*it)->Remove_InputFunction(FUNCTION_TEXMOVE);
                }

            }

            links.erase(remove_if(links.begin(), links.end(),
                [node_id](const pair<_int, _int>& link) {
                    return link.first == node_id + 1; //||
                }), links.end());
            //// 그 노드 ID가 디퓨즈에 영향을 줬었냐?
            //else if (target_second_value % 4 == 1)
            //{
            //    // m_NodeTextures에서 찾아서 삭제
            //    auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
            //        [&](CShader_Texture* texture) {
            //            return texture->m_iID == target_second_value / 4;
            //        });
            //    if (it != m_NodeTextures.end())
            //    {
            //        (*it)->Remove_InputFunction(FUNCTION_TEXMOVE);
            //    }
            //}

            m_MoveTex_Node_ids.remove_if([&](const MoveTex_Node& node) {
                return node.MoveTex_node_id == iID;
                });

            iID = -999;
            eNodeType = NODE_END;
            break;
        }
        }
    }
}

void CIMGUI_Shader_Tab::Check_Create_Link()
{
    // start 주는놈 end 받는놈
    _int start_attr = 0, end_attr = 0;
    if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
    {
        auto it = find_if(links.begin(), links.end(),
            [end_attr](const pair<_int, _int>& link) {
                return link.second == end_attr;
            });

        if (it != links.end())
        {
            links.erase(it);
        }


        if (start_attr > 1500)
        {
            auto it = m_MoveTex_Node_ids.begin();
            advance(it, (start_attr - 1 - 1501));

            links.push_back(make_pair(start_attr, end_attr));
            auto Texture_nodeit = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                [&](CShader_Texture* texture) {
                    return texture->m_iID == end_attr / m_iAttributeCount;
                });

            if (Texture_nodeit != m_NodeTextures.end())
            {
                (*Texture_nodeit)->Push_Shade_MoveTex(&it->fDirection, &it->fSpeed);
            }
            
        }
        // Main_Node Diffuse에 연결된거임
        else if (end_attr == -2)
        {
            m_iMain_Input_Diffuse_id = (start_attr) / 3;
            links.push_back(make_pair(start_attr, end_attr));

            auto SRVit = std::find_if(m_NodeTextureSRVs.begin(), m_NodeTextureSRVs.end(),
                [&](SRV_Texture SRV) {
                    return SRV.iID == start_attr / m_iAttributeCount;
                });

            m_TestEffectModel_Texture->Set_SRV((ID3D11ShaderResourceView*)SRVit->Texture);
            //m_Effect_Rect->Push_Texture_Diffuse((ID3D11ShaderResourceView*)m_NodeTextureSRVs[m_iMain_Input_Diffuse_id - 1], 0);
        }
        else if (end_attr % 4 == 2)
        {
            links.push_back(make_pair(start_attr, end_attr));

            auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                [&](CShader_Texture* texture) {
                    return texture->m_iID == end_attr / m_iAttributeCount;
                });

            if (it != m_NodeTextures.end())
            {
                auto SRVit = std::find_if(m_NodeTextureSRVs.begin(), m_NodeTextureSRVs.end(),
                    [&](SRV_Texture SRV) {
                        return SRV.iID == start_attr / m_iAttributeCount;
                    });

                (*it)->Push_InputTextures((ID3D11ShaderResourceView*)SRVit->Texture,END_ALPHA);

            }
            //m_NodeTextures[(end_attr - 2) / 4 - 1]->Push_InputTextures((ID3D11ShaderResourceView*)m_NodeTextureSRVs[(start_attr) / 4 - 1].Texture, END_ALPHA);
        }
        else if (end_attr % 4 == 1)
        {
            links.push_back(make_pair(start_attr, end_attr));

            auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                [&](CShader_Texture* texture) {
                    return texture->m_iID == end_attr / m_iAttributeCount;
                });
            if (it != m_NodeTextures.end())
            {
                auto SRVit = std::find_if(m_NodeTextureSRVs.begin(), m_NodeTextureSRVs.end(),
                    [&](SRV_Texture SRV) {
                        return SRV.iID == start_attr / 4;
                    });

                (*it)->Push_InputTextures((ID3D11ShaderResourceView*)SRVit->Texture, END_DIFFUSE);

            }

           // m_NodeTextures[(end_attr - 1) / 4 - 1]->Push_InputTextures((ID3D11ShaderResourceView*)m_NodeTextureSRVs[(start_attr) / 4 - 1].Texture, END_DIFFUSE);
        }
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

    Safe_Release(m_TestEffectModel_Texture);
}
