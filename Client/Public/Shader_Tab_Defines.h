#pragma once
#include <vector>
#include <string>
#include <list>
#include <unordered_map>
#include <fstream>


    struct Save_Key
    {
        _wstring key;
        _int iD;
    };
    struct MoveTex_Node_Save
    {
        _int MoveTex_node_id;
        _float2 fDirection;
        _float fSpeed;
    };
    struct Node_Position
    {
        _int nodeID;          // ��� ID
        ImVec2 nodePosition;  // ����� ��ġ
    };
    struct Sprite_Node_Save
    {
        _int Sprite_node_id;
        _bool isLoop;
        _float2 fSpriteSizeNumber;
        _float fSpeed;
    };
    struct Link_Save
    {
        _int srcNodeID;   // ����� �ҽ� ��� ID
        _int destNodeID;  // ����� ������ ��� ID
    };
	typedef struct
	{
		vector<Save_Key> keys;                        // �ؽ�ó Ű ����
		vector<Node_Position> nodePositions;          // �� ����� ��ġ
		vector<MoveTex_Node_Save> moveTexNodes;       // ���� �ؽ� ��� ����
		vector<Sprite_Node_Save> spriteNodes;         // ��������Ʈ ��� ����
		vector<Link_Save> links;                      // ��� ���� ����
	}Shader_Tab_Save;

