
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
        {MODELID_NOT, L"MODELID_NOT"},
        {MODELID_DEFAULT, L"MODELID_DEFAULT"},
        {MODELID_SON, L"MODELID_SON"},
        {MODELID_HIT, L"MODELID_HIT"},
        {MODELID_MINE, L"MODELID_MINE"},
        {MODELID_21, L"MODELID_21"}
    };

    std::unordered_map<_int, _wstring> skillIDToString = {
        {SKILL_NOT, L"SKILL_NOT"},
        {SKILL1, L"SKILL1"},
        {SKILL2, L"SKILL2"},
        {SKILL3, L"SKILL3"}
    };

    std::unordered_map<_wstring, _int> stringToModelID = {
        {L"MODELID_NOT", MODELID_NOT},
        {L"MODELID_DEFAULT", MODELID_DEFAULT},
        {L"MODELID_SON", MODELID_SON},
        {L"MODELID_HIT", MODELID_HIT},
        {L"MODELID_MINE", MODELID_MINE},
        {L"MODELID_21", MODELID_21}
    };

    std::unordered_map<_wstring, _int> stringToSkillID = {
        {L"SKILL_NOT", SKILL_NOT},
        {L"SKILL1", SKILL1},
        {L"SKILL2", SKILL2},
        {L"SKILL3", SKILL3}
    };

}