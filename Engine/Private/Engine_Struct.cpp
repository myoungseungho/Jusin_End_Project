
#include "Engine_Defines.h"

namespace Engine
{
	const D3D11_INPUT_ELEMENT_DESC		VTXPOSTEX::Elements[VTXPOSTEX::iNumElements] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	const D3D11_INPUT_ELEMENT_DESC		VTXANIMMESH::Elements[VTXANIMMESH::iNumElements] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32,  D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

    std::unordered_map<_int, _wstring> modelIDToString = {
        {-1, L"MODELID_NOT"},
        {0, L"MODELID_DEFAULT"},
        {1, L"MODELID_SON"},
        {2, L"MODELID_HIT"},
        {3, L"MODELID_MINE"},
        {4, L"MODELID_21"}
    };
    std::unordered_map<_wstring, _int> stringToModelID = {
        {L"MODELID_NOT", -1},
        {L"MODELID_DEFAULT", 0},
        {L"MODELID_SON", 1},
        {L"MODELID_HIT", 2},
        {L"MODELID_MINE", 3},
        {L"MODELID_21", 4}
    };
}