#include "stdafx.h"

#include <algorithm> // std::remove

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
    static bool isStart = false;
    static int unique_node_id = 1;
    static std::vector<int> node_ids;
    static std::vector<std::pair<int, int>> links;

    // Store input values for each node
    static std::map<int, int> node_values; // Store node id and its corresponding value
    static std::map<int, int> input_accumulated_values; // Store accumulated values for input nodes

    if (ImGui::Button("Add Rect") && !isStart)
    {
        //몬스터 생성
        if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Rect"), TEXT("Layer_Effect_Rect"))))
            return;

        isStart = true;
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

    // Render nodes
    for (int node_id : node_ids)
    {
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
