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
#include "Effect_Manager.h"
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
    m_PrototypeKeys.clear();
	return S_OK;
}

HRESULT CIMGUI_Shader_Tab::Load_Initialize(string strFilename)
{
    DragAcceptFiles(g_hWnd, TRUE);
    isStart = true;
    m_PrototypeKeys.clear();

    //if (strFilename.size() != NULL)
    //{
    //    
    //}

    return S_OK;
}

void CIMGUI_Shader_Tab::Update(_float fTimeDelta)
{
    for (auto& iter : m_NodeTextures)
    {
        iter->Priority_Update(fTimeDelta);
        iter->Update(fTimeDelta);
        iter->Late_Update(fTimeDelta);
    }

}

void CIMGUI_Shader_Tab::Render(_float fTimeDelta)
{
    if (ImGui::Button("Load") && isStart)
    {
        
    }

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

    if (ImGui::Button("Add MoveTex") && isStart)
    {
        MoveTex_Node nodeDesc{};
        nodeDesc.MoveTex_node_id = m_MoveTex_node_id++;
        m_MoveTex_Node_ids.push_back(nodeDesc);
      //  node_ids.push_back(nodeDesc.MoveTex_node_id - 1500);
    }
    ImGui::SameLine();
    if (ImGui::Button("Add Sprite") && isStart)
    {
        Sprite_Node nodeDesc{};
        nodeDesc.Sprite_node_id = m_Sprite_node_id++;
        m_Sprite_Node_ids.push_back(nodeDesc);
      //  node_ids.push_back(nodeDesc.Sprite_node_id - 3000);
    }
    if (ImGui::Button("Save") && isStart)
    {
        Click_Save_Shader_Tab("Test000");
    }
    

    ImNodes::BeginNodeEditor();  /* ��� ������ ������ ȣ���ؾ��� */

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    

    Render_MainNode();

    Render_TextureNode();

    Render_MoveTexNode();

    Render_SpriteNode();

    Render_Link();

    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_TopRight);
    ImGui::PopStyleColor();
    ImNodes::EndNodeEditor(); /* ��� �� */

    Check_Create_Link(); // �����Ҷ��� ��忡���Ͱ� ������ üũ
    Check_Delete_Link(); // �����Ϸ��Ҷ� ����

    //_int link_id;
    //if (ImNodes::IsLinkDestroyed(&link_id))
    //{
    //    links.erase(std::remove_if(links.begin(), links.end(),
    //        [link_id](const pair<int, int>& link) {
    //            return link.first == link_id || link.second == link_id;
    //        }), links.end());
    //}
    for (auto& iter : m_NodeTextures)
    {
        iter->Late_Update(fTimeDelta);
    }
}

void CIMGUI_Shader_Tab::TabPos_Init()
{
    for (const auto& node : node_positions)
    {
        int node_id = node.first;      // ��� ID
        ImVec2 node_pos = node.second; // ��� ��ġ

        // �� ����� ��ġ ����
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
        // ������ ������ ���� ���ڿ� ã��
        size_t lastSlashPos = relativePath.find_last_of('/');
        size_t lastDotPos = relativePath.find_last_of('.');

        std::string fileName = relativePath.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1); // "cmm_fire"

        wstring prototypeKey = TEXT("Prototype_Component_Texture_Effect_") + converter.from_bytes(fileName);
        //wstring prototypeKeyWithCount = prototypeKey + to_wstring(m_iNodeTextureCount) + TEXT("_") + to_wstring(m_iNumberId);
        wstring prototypeKeyWithAlpha = prototypeKey + TEXT("_Alpha");
        CShader_Texture::SHADER_TEXTURE_DESC tDesc{};
        tDesc.prototypeKey = prototypeKey.c_str();
        tDesc.iID = unique_node_id;
        tDesc.iShaderTab_ID = m_iNumberId;
        //Prototype_Component_Texture_Effect_cmn_aura00
        //if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, prototypeKeyWithCount.c_str(),
        //    CTexture::Create(m_pDevice, m_pContext, converter.from_bytes(fullPath).c_str(), 1))))
        //    return;

        CGameObject* pPrototype = m_pGameInstance->Find_Prototype(TEXT("Prototype_GameObject_Shader_Texture"));
        static_cast<CShader_Texture*>(pPrototype->Clone((void*)&tDesc));
        _float2 fTextureSize = m_NodeTextures.back()->m_pTextureCom->Get_TextureSize();

        _float fAspectRatio = fTextureSize.x / fTextureSize.y;
        _float fWinAspectRatio = (_float)g_iWinSizeX / (_float)g_iWinSizeY;

        if (fTextureSize.x > g_iWinSizeX || fTextureSize.y > g_iWinSizeY)
        {
            if (fTextureSize.x / g_iWinSizeX > fTextureSize.y / g_iWinSizeY)
            {
                fTextureSize.x = g_iWinSizeX;
                fTextureSize.y = g_iWinSizeX / fAspectRatio;
            }
            else
            {
                fTextureSize.y = g_iWinSizeY;
                fTextureSize.x = g_iWinSizeY * fAspectRatio;
            }
        }

        _int iRenderTargetCount = m_pRenderInstance->Add_ClientRenderTarget(prototypeKey.c_str(), prototypeKey.c_str(), fTextureSize.x, fTextureSize.y, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(0.f, 0.f, 0.f, 0.f));
        /* �̹� ����Ÿ���� �ߺ��̴� */
        if (iRenderTargetCount > 0)
        {
            SRV_Texture SRVDesc{};
            SRVDesc.iID = unique_node_id;
            SRVDesc.Texture = (ImTextureID)m_pRenderInstance->Copy_RenderTarget_SRV(prototypeKey.c_str() + to_wstring(iRenderTargetCount));
            SRVDesc.Alpha = (ImTextureID)m_pRenderInstance->Copy_RenderTarget_SRV(prototypeKey.c_str() + to_wstring(iRenderTargetCount) + L"_Alpha");

            if (SRVDesc.Texture == nullptr || SRVDesc.Alpha == nullptr)
            {
                int a = 10;
            }
            m_NodeTextureSRVs.push_back(SRVDesc);

            m_NodeTextures.back()->Set_PlusKey(prototypeKey.c_str() + to_wstring(iRenderTargetCount));

            Save_Key tSave_KeyDesc{};
            tSave_KeyDesc.iD = unique_node_id;
            tSave_KeyDesc.key = prototypeKey.c_str();
            m_PrototypeKeys.push_back(tSave_KeyDesc);

            m_NodeTextures.back()->m_iID = unique_node_id;
            node_ids.push_back(unique_node_id++);

            DragAcceptFiles(g_hWnd, TRUE);
            m_iNodeTextureCount++;

            return;
        }
        SRV_Texture SRVDesc{};
        SRVDesc.iID = unique_node_id;
        SRVDesc.Texture = (ImTextureID)m_pRenderInstance->Copy_RenderTarget_SRV(prototypeKey.c_str());
        SRVDesc.Alpha = (ImTextureID)m_pRenderInstance->Copy_RenderTarget_SRV(prototypeKeyWithAlpha.c_str());
        if (SRVDesc.Texture == nullptr || SRVDesc.Alpha == nullptr)
        {
            int a = 10;
        }
        m_NodeTextureSRVs.push_back(SRVDesc);

        Save_Key tSave_KeyDesc{};
        tSave_KeyDesc.iD = unique_node_id;
        tSave_KeyDesc.key = prototypeKey.c_str();
        m_PrototypeKeys.push_back(tSave_KeyDesc);

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
        
        ImNodes::BeginNode(node_id);

        ImGui::Dummy(ImVec2(1, 1));

        ImGui::Text("MoveTex_Node %d", node_id - 1500);

        ImGui::SetNextItemWidth(60);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::InputFloat2("Direction", &iter.fDirection.x, "%.1f");

        ImGui::SetNextItemWidth(50);
        ImGui::DragFloat("Speed", &iter.fSpeed, 0.1f, 0.0f, 30.0f, "%.2f");

        ImGui::PopStyleColor();
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

void CIMGUI_Shader_Tab::Render_SpriteNode()
{
    for (auto& iter : m_Sprite_Node_ids)
    {
        int node_id = iter.Sprite_node_id;

        ImNodes::BeginNode(node_id);

        ImGui::Dummy(ImVec2(1, 1));

        ImGui::Text("Sprite_Node %d", node_id - 3000);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::SetNextItemWidth(50);
        _int a[2] = { iter.fSpriteSizeNumber.x ,iter.fSpriteSizeNumber.y };
        ImGui::InputInt2("Width & Height", a, ImGuiInputTextFlags_DisplayEmptyRefVal);
        iter.fSpriteSizeNumber = { (_float)a[0],(_float)a[1] };
        ImGui::SetNextItemWidth(50);

        ImGui::DragFloat("Speed", &iter.fSpeed, 0.1f, 0.0f, 30.0f, "%.2f");

        ImGui::PopStyleColor();
        ImNodes::BeginOutputAttribute(node_id + 1, 2);
        ImGui::Text("Out");
        ImNodes::EndOutputAttribute();

        auto it = find_if(links.begin(), links.end(),
            [node_id](const pair<_int, _int>& link) {
                return link.first == node_id + 1;
            });

        if (it != links.end())
        {
            auto Texture_nodeit = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                [&](CShader_Texture* texture) {
                    return texture->m_iID == (*it).second / m_iAttributeCount;
                });

            if (Texture_nodeit != m_NodeTextures.end())
            {

                if (ImGui::Checkbox("Loop", &(iter.isLoop)))
                    (*Texture_nodeit)->Sprite_Loop(iter.isLoop);

                Draw_MusicButton((*Texture_nodeit));
            }
        }



        //else
        //{
        //    Draw_MusicButton(nullptr);
        //}

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
    // ���鳢�� ����� ��� �ֵ��� �ð�ȭ �ϱ� ���� �Լ�
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
            if (iter == -1)
                return;

            if (ImNodes::IsNodeHovered(&iter))
            {
                if (iter > 1500)
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
        /*list<MoveTex_Node> m_MoveTex_Node_ids*/
        for (auto iter : m_MoveTex_Node_ids)
        {
            if (ImNodes::IsNodeHovered(&iter.MoveTex_node_id))
            {
                if (iter.MoveTex_node_id == -1)
                    return;

                iID = iter.MoveTex_node_id;
                eNodeType = NODE_FUNCTION;
                return;
            }
        }

        for (auto iter : m_Sprite_Node_ids)
        {
            if (ImNodes::IsNodeHovered(&iter.Sprite_node_id))
            {
                if (iter.Sprite_node_id == -1)
                    return;

                iID = iter.Sprite_node_id;
                eNodeType = NODE_FUNCTION;
                return;
            }
        }
    }

    if (m_pGameInstance->Get_DIKeyState(DIK_DELETE) && eNodeType != NODE_END)
    {
        //id �ϴ� ����
        _int node_id = iID;

        switch (eNodeType)
        {
        case NODE_BASIC:
        {
            //Node ���� ����
            node_ids.erase(remove(node_ids.begin(), node_ids.end(), iID), node_ids.end());

            // m_NodeTextures�� �ִ°� ã�Ƽ� ����
            auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                [&](CShader_Texture* texture) {
                    return texture->m_iID == iID;
                });
            if (it != m_NodeTextures.end())
            {
                (*it)->Free();
                m_NodeTextures.erase(it);
            }

            // m_NodeTextureSRVs�� SRV�� ����ΰ� ����
            auto iter = std::find_if(m_NodeTextureSRVs.begin(), m_NodeTextureSRVs.end(),
                [&](const SRV_Texture& srv) {
                    return srv.iID == iID;
                });
            if (iter != m_NodeTextureSRVs.end())
            {
                m_NodeTextureSRVs.erase(iter);
            }

            // �� ��尡 �ٸ� ��忡�� ������ �������� ���� �� Ÿ�ٳ����̵�
            _int target_node_id = node_id * m_iAttributeCount;
            _int target_second_value = -1;
            // ã�Ƽ� ������� ��� ID ����
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
            // �� ��� ID�� ���Ŀ� ������ �����?
            else if (target_second_value % 4 == 2)
            {
                // m_NodeTextures���� ã�Ƽ� ����
                auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == target_second_value / 4;
                    });
                if (it != m_NodeTextures.end())
                {
                    (*it)->Remove_InputTextures(END_ALPHA);
                }
            }
            // �� ��� ID�� ��ǻ� ������ �����?
            else if (target_second_value % 4 == 1)
            {
                // m_NodeTextures���� ã�Ƽ� ����
                auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == target_second_value / 4;
                    });
                if (it != m_NodeTextures.end())
                {
                    (*it)->Remove_InputTextures(END_DIFFUSE);
                }
            }

            // ��ũ ����
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

            // Ȥ�� �Լ�?
            if (linkPos->first > 1500)
            {
                // m_NodeTextures���� ã�Ƽ� ����
                auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == linkPos->second / 4;
                    });
                if (it != m_NodeTextures.end())
                {
                    if (linkPos->first > 1500 && linkPos->first < 3000)
                        (*it)->Remove_InputFunction(FUNCTION_TEXMOVE);
                    else if (linkPos->first > 3000)
                        (*it)->Remove_InputFunction(FUNCTION_SPRITE);
                }
            }
            // Ȥ�� ���γ��?
            else if (linkPos->second == -2)
            {
                if (m_pDefaultTexture == nullptr)
                {
                    m_pDefaultTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Effect_Default_Diffuse")));
                }

                m_TestEffectModel_Texture->Set_SRV(m_pDefaultTexture->Get_SRV(0));

            }
            // �� ��� ID�� ���Ŀ� ������ �����?
            else if (linkPos->second % 4 == 2)
            {
                // m_NodeTextures���� ã�Ƽ� ����
                auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == linkPos->second / 4;
                    });
                if (it != m_NodeTextures.end())
                {
                    (*it)->Remove_InputTextures(END_ALPHA);
                }
            }
            // �� ��� ID�� ��ǻ� ������ �����?
            else if (linkPos->second % 4 == 1)
            {
                // m_NodeTextures���� ã�Ƽ� ����
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
            // �� ��尡 �ٸ� ��忡�� ������ �������� ���� �� Ÿ�ٳ����̵�
            _int target_node_id = node_id + 1;
            _int target_second_value = -1;
            // ã�Ƽ� ������� ��� ID ����
            auto LinkIter = std::find_if(links.begin(), links.end(),
                [target_node_id](const pair<_int, _int>& link) {
                    return link.first == target_node_id;
                });
            if (LinkIter != links.end()) {
                target_second_value = LinkIter->second;
            }

            // Ȥ�� ���γ��?
            if (target_second_value == -2)
            {

            }
            // �Լ� �ƿ���
            else if (target_second_value % 4 == 3)
            {
                // m_NodeTextures���� ã�Ƽ� ����
                auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == target_second_value / 4;
                    });
                if (it != m_NodeTextures.end())
                {
                    if (target_node_id > 1500 && target_node_id < 3000)
                    {
                        (*it)->Remove_InputFunction(FUNCTION_TEXMOVE);

                    }
                    else if (target_node_id > 3000)
                    {
                        (*it)->Remove_InputFunction(FUNCTION_SPRITE);

                    }
                }

            }
            if (target_node_id > 1500 && target_node_id < 3000)
            {
                m_MoveTex_Node_ids.remove_if([&](const MoveTex_Node& node) {
                    return node.MoveTex_node_id == iID;
                    });
            }
            else if (target_node_id > 3000)
            {
                m_Sprite_Node_ids.remove_if([&](const Sprite_Node& node) {
                    return node.Sprite_node_id == iID;
                    });
            }
            links.erase(remove_if(links.begin(), links.end(),
                [node_id](const pair<_int, _int>& link) {
                    return link.first == node_id + 1; //||
                }), links.end());
            //Node ���� ����

          /*  if(iID < 3000)
                node_ids.erase(remove(node_ids.begin(), node_ids.end(), iID - 1500), node_ids.end());
            else
                node_ids.erase(remove(node_ids.begin(), node_ids.end(), iID - 3000), node_ids.end());*/

                //// �� ��� ID�� ��ǻ� ������ �����?
                //else if (target_second_value % 4 == 1)
                //{
                //    // m_NodeTextures���� ã�Ƽ� ����
                //    auto it = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                //        [&](CShader_Texture* texture) {
                //            return texture->m_iID == target_second_value / 4;
                //        });
                //    if (it != m_NodeTextures.end())
                //    {
                //        (*it)->Remove_InputFunction(FUNCTION_TEXMOVE);
                //    }
                //}



            iID = -999;
            eNodeType = NODE_END;
            break;
        }
        }
    }
}


void CIMGUI_Shader_Tab::Draw_MusicButton(CShader_Texture* pShaderTexture)
{
    //�����ư   
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 35);
    ImVec2 button_size = ImVec2(20, 20);
    if (ImGui::InvisibleButton("play_button", button_size)) 
    {
        pShaderTexture->Sprite_ButtonClick(CShader_Texture::SB_PLAY);
    }

    ImVec2 p = ImGui::GetItemRectMin();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float size = 20.0f;
    ImVec2 triangle_pos[3] = {
        ImVec2(p.x + (button_size.x - size) * 0.5f, p.y),
        ImVec2(p.x + (button_size.x + size) * 0.5f, p.y + size / 2),
        ImVec2(p.x + (button_size.x - size) * 0.5f, p.y + size)
    };

    ImU32 triangle_color;

    if (ImGui::IsItemHovered()) 
        triangle_color = IM_COL32(155, 255, 155, 255);
    else
        triangle_color = IM_COL32(85, 255, 85, 255);

    draw_list->AddTriangleFilled(triangle_pos[0], triangle_pos[1], triangle_pos[2], triangle_color);

    // �Ͻ����� ��ư
    ImGui::SameLine();
    if (ImGui::InvisibleButton("pause_button", button_size)) 
    {
        pShaderTexture->Sprite_ButtonClick(CShader_Texture::SB_PAUSE);
    }

    p = ImGui::GetItemRectMin();
    ImVec2 pause_rect1_pos[2] = {
        ImVec2(p.x + 4, p.y),
        ImVec2(p.x + 8, p.y + size)
    };
    ImVec2 pause_rect2_pos[2] = {
        ImVec2(p.x + 12, p.y),
        ImVec2(p.x + 16, p.y + size)
    };
    ImU32 pause_color = IM_COL32(255, 255, 65, 255);
    if (ImGui::IsItemHovered()) 
        pause_color = IM_COL32(255, 255, 185, 255);

    draw_list->AddRectFilled(pause_rect1_pos[0], pause_rect1_pos[1], pause_color);
    draw_list->AddRectFilled(pause_rect2_pos[0], pause_rect2_pos[1], pause_color);

    // �ʱ�ȭ ��ư
    ImGui::SameLine();
    if (ImGui::InvisibleButton("reset_button", button_size)) 
    {
        pShaderTexture->Sprite_ButtonClick(CShader_Texture::SB_RESET);
    }

    p = ImGui::GetItemRectMin();
    ImVec2 reset_rect_pos[2] = {
        ImVec2(p.x, p.y),
        ImVec2(p.x + size, p.y + size)
    };
    ImU32 reset_color = IM_COL32(255, 85, 85, 255);
    if (ImGui::IsItemHovered())
        reset_color = IM_COL32(255, 135, 135, 255);

    draw_list->AddRectFilled(reset_rect_pos[0], reset_rect_pos[1], reset_color);
}

void CIMGUI_Shader_Tab::Click_Save_Shader_Tab(string fileName)
{
    Shader_Tab_Save tDesc{};
    /*------------ �� ä���------------ */

    // 1. Save_Key ä��� (m_PrototypeKeys���� ���� ������)
    for (const auto& prototypeKey : m_PrototypeKeys)
    {
        Save_Key saveKey;
        saveKey.key = prototypeKey.key;
        saveKey.iD = prototypeKey.iD;
        tDesc.keys.push_back(saveKey);
    }

    // 2. Node_Position ä��� (node_positions���� ���� ������)
    for (const auto& nodePosPair : node_positions)
    {
        Node_Position nodePos;
        nodePos.nodeID = nodePosPair.first;
        nodePos.nodePosition = nodePosPair.second;
        tDesc.nodePositions.push_back(nodePos);
    }

    // 3. MoveTex_Node_Save ä��� (m_MoveTex_Node_ids���� ���� ������)
    for (const auto& moveTexNode : m_MoveTex_Node_ids)
    {
        MoveTex_Node_Save moveTexNodeSave;
        moveTexNodeSave.MoveTex_node_id = moveTexNode.MoveTex_node_id;
        moveTexNodeSave.fDirection = moveTexNode.fDirection;
        moveTexNodeSave.fSpeed = moveTexNode.fSpeed;
        tDesc.moveTexNodes.push_back(moveTexNodeSave);
    }

    // 4. Sprite_Node_Save ä��� (m_Sprite_Node_ids���� ���� ������)
    for (const auto& spriteNode : m_Sprite_Node_ids)
    {
        Sprite_Node_Save spriteNodeSave;
        spriteNodeSave.Sprite_node_id = spriteNode.Sprite_node_id;
        spriteNodeSave.isLoop = spriteNode.isLoop;
        spriteNodeSave.fSpriteSizeNumber = spriteNode.fSpriteSizeNumber;
        spriteNodeSave.fSpeed = spriteNode.fSpeed;
        tDesc.spriteNodes.push_back(spriteNodeSave);
    }

    // 5. Link_Save ä��� (links���� ���� ������)
    for (const auto& link : links)
    {
        Link_Save linkSave;
        linkSave.srcNodeID = link.first;
        linkSave.destNodeID = link.second;
        tDesc.links.push_back(linkSave);
    }

    /*------------------------------------*/
    Save_Shader_Tab(fileName, tDesc);
}

void CIMGUI_Shader_Tab::Click_Load_Shader_Tab(string fileName)
{
    // �ҷ��� �����͸� ������ ����ü
    Shader_Tab_Save tDesc{};

    // ���Ͽ��� �����͸� �ҷ���
    Load_Shader_Tab(fileName, tDesc);

    // 1. �ؽ�ó Ű ���� �ε� �� ó��
    m_PrototypeKeys.clear();  // ���� �����͸� �ʱ�ȭ
    for (const auto& key : tDesc.keys)
    {
        m_PrototypeKeys.push_back(key);
    }
    Load_NodeTextures(m_PrototypeKeys);  // Prototype Ű�� ���� ��� �ؽ�ó�� �ε�

    // 2. �� ����� ��ġ ����
    //node_positions.clear();  // ���� ��ġ ������ �ʱ�ȭ
    for (const auto& nodePos : tDesc.nodePositions)
    {
        node_positions[nodePos.nodeID] = nodePos.nodePosition;
    }

    // 3. ���� �ؽ�ó ��� ���� �ε� �� �߰�
    m_MoveTex_Node_ids.clear();  // ���� ���� �ؽ� ��� �ʱ�ȭ
    for (const auto& moveTexNode : tDesc.moveTexNodes)
    {
        MoveTex_Node newMoveTexNode;
        newMoveTexNode.MoveTex_node_id = moveTexNode.MoveTex_node_id;
        newMoveTexNode.fDirection = moveTexNode.fDirection;
        newMoveTexNode.fSpeed = moveTexNode.fSpeed;
        m_MoveTex_Node_ids.push_back(newMoveTexNode);  // ���� �ؽ� ��� ����Ʈ�� �߰�
    }

    // 4. ��������Ʈ ��� ���� �ε� �� �߰�
    m_Sprite_Node_ids.clear();  // ���� ��������Ʈ ��� �ʱ�ȭ
    for (const auto& spriteNode : tDesc.spriteNodes)
    {
        Sprite_Node newSpriteNode;
        newSpriteNode.Sprite_node_id = spriteNode.Sprite_node_id;
        newSpriteNode.isLoop = spriteNode.isLoop;
        newSpriteNode.fSpriteSizeNumber = spriteNode.fSpriteSizeNumber;
        newSpriteNode.fSpeed = spriteNode.fSpeed;
        m_Sprite_Node_ids.push_back(newSpriteNode);  // ��������Ʈ ��� ����Ʈ�� �߰�
    }

    // 5. ��� �� ���� ���� �ε�
    links.clear();  // ���� ���� ���� �ʱ�ȭ
    for (const auto& link : tDesc.links)
    {
        Create_Link(link.srcNodeID, link.destNodeID);
        //links.push_back({ link.srcNodeID, link.destNodeID });  // ��� ���� ����Ʈ�� �߰�
    }

    // 6. �߰����� �ʱ�ȭ�� �ʿ��� ������ ������ �߰�
    // ���� ���, ������ ID �Ǵ� �Ӽ� �缳�� ���� �ʿ��� �� �ֽ��ϴ�.
}

void CIMGUI_Shader_Tab::Create_Link(_int start_attr, _int end_attr)
{

    auto it = find_if(links.begin(), links.end(),
        [end_attr](const pair<_int, _int>& link) {
            return link.second == end_attr;
        });

    if (it != links.end())
    {

        links.erase(it);
    }


    // �Լ� ����
    if (start_attr > 1500)
    {
        if (start_attr < 3000)
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
                (*Texture_nodeit)->Remove_InputFunction(FUNCTION_SPRITE);
            }
        }
        else if (start_attr > 3000/* ��� ���� �߰��Ǹ� ���� �� �޾ƾߵ�*/)
        {
            auto it = m_Sprite_Node_ids.begin();
            advance(it, (start_attr - 1 - 3001));

            links.push_back(make_pair(start_attr, end_attr));
            auto Texture_nodeit = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                [&](CShader_Texture* texture) {
                    return texture->m_iID == end_attr / m_iAttributeCount;
                });

            if (Texture_nodeit != m_NodeTextures.end())
            {
                (*Texture_nodeit)->Push_Shade_Sprite(&it->fSpriteSizeNumber, &it->fSpeed);
                (*Texture_nodeit)->Remove_InputFunction(FUNCTION_TEXMOVE);
            }

        }
    }
    // Main_Node Diffuse�� ����Ȱ���
    else if (end_attr == -2)
    {
        m_iMain_Input_Diffuse_id = (start_attr) / 3;
        links.push_back(make_pair(start_attr, end_attr));

        auto SRVit = std::find_if(m_NodeTextureSRVs.begin(), m_NodeTextureSRVs.end(),
            [&](SRV_Texture SRV) {
                return SRV.iID == start_attr / m_iAttributeCount;
            });
        //wstring abs = L"a";
        //wstring absb = L"b";
        //m_TestEffectModel_Texture = static_cast<CTexture*>(static_cast<CEffect_Single*>(CEffect_Manager::Get_Instance()->Find_In_Layer_Effect(abs, absb))->Get_Component(TEXT("Com_DiffuseTexture")));

        m_TestEffectModel_Texture->Set_SRV((ID3D11ShaderResourceView*)SRVit->Texture);
        m_TestEffectModel_Texture->Set_SRV((ID3D11ShaderResourceView*)SRVit->Alpha, 1);
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

            (*it)->Push_InputTextures((ID3D11ShaderResourceView*)SRVit->Texture, END_ALPHA);

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


void CIMGUI_Shader_Tab::Load_NodeTextures(vector<Save_Key>& PrototypeKeys)
{
    for (auto& iter : PrototypeKeys)
    {
        //size_t lastSlashPos = iter.key.find_last_of('/');
        //size_t lastDotPos = iter.key.find_last_of('.');

        //wstring fileName = iter.key.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1);

        CShader_Texture::SHADER_TEXTURE_DESC tDesc{};
        tDesc.prototypeKey = iter.key.c_str();
        tDesc.iShaderTab_ID = m_iNumberId;

        CGameObject* pPrototype = m_pGameInstance->Find_Prototype(TEXT("Prototype_GameObject_Shader_Texture"));
        static_cast<CShader_Texture*>(pPrototype->Clone((void*)&tDesc));

        _float2 fTextureSize = m_NodeTextures.back()->m_pTextureCom->Get_TextureSize();

        _float fAspectRatio = fTextureSize.x / fTextureSize.y;
        _float fWinAspectRatio = (_float)g_iWinSizeX / (_float)g_iWinSizeY;

        if (fTextureSize.x > g_iWinSizeX || fTextureSize.y > g_iWinSizeY)
        {
            if (fTextureSize.x / g_iWinSizeX > fTextureSize.y / g_iWinSizeY)
            {
                fTextureSize.x = g_iWinSizeX;
                fTextureSize.y = g_iWinSizeX / fAspectRatio;
            }
            else
            {
                fTextureSize.y = g_iWinSizeY;
                fTextureSize.x = g_iWinSizeY * fAspectRatio;
            }
        }

        m_pRenderInstance->Add_ClientRenderTarget(iter.key.c_str(), iter.key.c_str(), fTextureSize.x, fTextureSize.y, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(0.f, 0.f, 0.f, 0.f));

        SRV_Texture SRVDesc{};
        SRVDesc.iID = unique_node_id;
        SRVDesc.Texture = (ImTextureID)m_pRenderInstance->Copy_RenderTarget_SRV(iter.key.c_str());
        wstring AlphaName = iter.key + L"_Alpha";
        SRVDesc.Alpha = (ImTextureID)m_pRenderInstance->Copy_RenderTarget_SRV(AlphaName.c_str());
        m_NodeTextureSRVs.push_back(SRVDesc);

        //Save_Key tSaveKey{};
        //tSaveKey.iD = unique_node_id;
        //tSaveKey.key = iter.key.c_str();
        //m_PrototypeKeys.push_back(tSaveKey);

        m_NodeTextures.back()->m_iID = unique_node_id;
        node_ids.push_back(unique_node_id++);

        
        m_iNodeTextureCount++;
    }
}

void CIMGUI_Shader_Tab::Save_ClientBinary()
{

}

void CIMGUI_Shader_Tab::Check_Create_Link()
{
    // start �ִ³� end �޴³�
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


        // �Լ� ����
        if (start_attr > 1500)
        {
            if (start_attr < 3000)
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
                    (*Texture_nodeit)->Remove_InputFunction(FUNCTION_SPRITE);
                }
            }
            else if (start_attr > 3000/* ��� ���� �߰��Ǹ� ���� �� �޾ƾߵ�*/)
            {
                auto it = m_Sprite_Node_ids.begin();
                advance(it, (start_attr - 1 - 3001));

                links.push_back(make_pair(start_attr, end_attr));
                auto Texture_nodeit = std::find_if(m_NodeTextures.begin(), m_NodeTextures.end(),
                    [&](CShader_Texture* texture) {
                        return texture->m_iID == end_attr / m_iAttributeCount;
                    });

                if (Texture_nodeit != m_NodeTextures.end())
                {
                    (*Texture_nodeit)->Push_Shade_Sprite(&it->fSpriteSizeNumber, &it->fSpeed);
                    (*Texture_nodeit)->Remove_InputFunction(FUNCTION_TEXMOVE);
                }

            }
        }
        // Main_Node Diffuse�� ����Ȱ���
        else if (end_attr == -2)
        {
            m_iMain_Input_Diffuse_id = (start_attr) / 3;
            links.push_back(make_pair(start_attr, end_attr));

            auto SRVit = std::find_if(m_NodeTextureSRVs.begin(), m_NodeTextureSRVs.end(),
                [&](SRV_Texture SRV) {
                    return SRV.iID == start_attr / m_iAttributeCount;
                });
            //wstring abs = L"a";
            //wstring absb = L"b";
            //m_TestEffectModel_Texture = static_cast<CTexture*>(static_cast<CEffect_Single*>(CEffect_Manager::Get_Instance()->Find_In_Layer_Effect(abs, absb))->Get_Component(TEXT("Com_DiffuseTexture")));

            m_TestEffectModel_Texture->Set_SRV((ID3D11ShaderResourceView*)SRVit->Texture);
            m_TestEffectModel_Texture->Set_SRV((ID3D11ShaderResourceView*)SRVit->Alpha,1);
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

void CIMGUI_Shader_Tab::Save_Shader_Tab(string fileName, const Shader_Tab_Save& shaderTabSave)
{
    ofstream outFile(fileName, ios::binary);
    if (!outFile.is_open())
    {
        // ���� ���� ���� ó��
        return;
    }

    // Save_Key ����
    size_t keyCount = shaderTabSave.keys.size();
    outFile.write(reinterpret_cast<const char*>(&keyCount), sizeof(keyCount));
    for (const auto& key : shaderTabSave.keys)
    {
        size_t keySize = key.key.size();
        outFile.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));

        // wchar_t -> char*�� ��ȯ�Ͽ� ����
        outFile.write(reinterpret_cast<const char*>(key.key.data()), keySize * sizeof(wchar_t));

        outFile.write(reinterpret_cast<const char*>(&key.iD), sizeof(key.iD));
    }

    // ������ �κ� (Node_Position, MoveTex_Node_Save, Sprite_Node_Save, Link_Save) ���� �κ��� �״�� ����
    // Node_Position ����
    size_t nodePosCount = shaderTabSave.nodePositions.size();
    outFile.write(reinterpret_cast<const char*>(&nodePosCount), sizeof(nodePosCount));
    outFile.write(reinterpret_cast<const char*>(shaderTabSave.nodePositions.data()), nodePosCount * sizeof(Node_Position));

    // MoveTex_Node ����
    size_t moveTexNodeCount = shaderTabSave.moveTexNodes.size();
    outFile.write(reinterpret_cast<const char*>(&moveTexNodeCount), sizeof(moveTexNodeCount));
    outFile.write(reinterpret_cast<const char*>(shaderTabSave.moveTexNodes.data()), moveTexNodeCount * sizeof(MoveTex_Node_Save));

    // Sprite_Node ����
    size_t spriteNodeCount = shaderTabSave.spriteNodes.size();
    outFile.write(reinterpret_cast<const char*>(&spriteNodeCount), sizeof(spriteNodeCount));
    outFile.write(reinterpret_cast<const char*>(shaderTabSave.spriteNodes.data()), spriteNodeCount * sizeof(Sprite_Node_Save));

    // Link_Save ����
    size_t linkCount = shaderTabSave.links.size();
    outFile.write(reinterpret_cast<const char*>(&linkCount), sizeof(linkCount));
    outFile.write(reinterpret_cast<const char*>(shaderTabSave.links.data()), linkCount * sizeof(Link_Save));

    outFile.close();
}

void CIMGUI_Shader_Tab::Load_Shader_Tab(string fileName, Shader_Tab_Save& shaderTabSave)
{
    ifstream inFile(fileName, ios::binary);
    if (!inFile.is_open())
    {
        // ���� ���� ���� ó��
        return;
    }

    // Save_Key �ε�
    size_t keyCount;
    inFile.read(reinterpret_cast<char*>(&keyCount), sizeof(keyCount));
    shaderTabSave.keys.resize(keyCount);
    for (auto& key : shaderTabSave.keys)
    {
        size_t keySize;
        inFile.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));

        // wchar_t ũ�⿡ �°� ���� �Ҵ�
        key.key.resize(keySize);

        // const ���� �� wchar_t �����͸� �о��
        inFile.read(reinterpret_cast<char*>(const_cast<wchar_t*>(key.key.data())), keySize * sizeof(wchar_t));

        inFile.read(reinterpret_cast<char*>(&key.iD), sizeof(key.iD));
    }

    // Node_Position �ε�
    size_t nodePosCount;
    inFile.read(reinterpret_cast<char*>(&nodePosCount), sizeof(nodePosCount));
    shaderTabSave.nodePositions.resize(nodePosCount);
    inFile.read(reinterpret_cast<char*>(shaderTabSave.nodePositions.data()), nodePosCount * sizeof(Node_Position));

    // MoveTex_Node �ε�
    size_t moveTexNodeCount;
    inFile.read(reinterpret_cast<char*>(&moveTexNodeCount), sizeof(moveTexNodeCount));
    shaderTabSave.moveTexNodes.resize(moveTexNodeCount);
    inFile.read(reinterpret_cast<char*>(shaderTabSave.moveTexNodes.data()), moveTexNodeCount * sizeof(MoveTex_Node_Save));

    // Sprite_Node �ε�
    size_t spriteNodeCount;
    inFile.read(reinterpret_cast<char*>(&spriteNodeCount), sizeof(spriteNodeCount));
    shaderTabSave.spriteNodes.resize(spriteNodeCount);
    inFile.read(reinterpret_cast<char*>(shaderTabSave.spriteNodes.data()), spriteNodeCount * sizeof(Sprite_Node_Save));

    // Link_Save �ε�
    size_t linkCount;
    inFile.read(reinterpret_cast<char*>(&linkCount), sizeof(linkCount));
    shaderTabSave.links.resize(linkCount);
    inFile.read(reinterpret_cast<char*>(shaderTabSave.links.data()), linkCount * sizeof(Link_Save));

    inFile.close();
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
    for (auto& iter : m_NodeTextures)
    {
        Safe_Release(iter);
    }
    
    Safe_Release(m_TestEffectModel_Texture);
}

CIMGUI_Shader_Tab* CIMGUI_Shader_Tab::Create_Load(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CTexture* pTexture, string strFilename)
{
    CIMGUI_Shader_Tab* pInstance = new CIMGUI_Shader_Tab(pDevice, pContext, pTexture);

    if (FAILED(pInstance->Load_Initialize(strFilename)))
    {
        MSG_BOX(TEXT("Failed to Created : CIMGUI_Shader_Tab"));
        Safe_Release(pInstance);
    }

    return pInstance;
}
