#pragma once

#include "GameObject.h"
#include "Client_Defines.h"
#include "AnimationEvent_Defines.h"
#include "FrameEvent_Manager.h"
BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

// ���� �Է� ����
enum DirectionInput {
    MOVEKEY_NEUTRAL,      // �߸�
    MOVEKEY_UP,           // ��
    MOVEKEY_DOWN,         // �Ʒ�
    MOVEKEY_LEFT,         // ����
    MOVEKEY_RIGHT,        // ������
    MOVEKEY_UP_LEFT,      // �� ����
    MOVEKEY_UP_RIGHT,     // �� ������
    MOVEKEY_DOWN_LEFT,    // �Ʒ� ����
    MOVEKEY_DOWN_RIGHT    // �Ʒ� ������
};

// ��ư �Է� ����
enum ButtonInput {
    ATTACK_NONE,         // ��ư�� ������ ����
    ATTACK_LIGHT,        // ��ġ ��ư
    ATTACK_MEDIUM,          // ű ��ư
    ATTACK_HEAVY,
    ATTACK_SPECIAL
};

// Input Ŭ���� ���� (���� + ��ư�� ����)
class CInput {
public:
    DirectionInput direction;  // �÷��̾ �Է��� ����
    ButtonInput button;        // �÷��̾ �Է��� ��ư


   // int frameTime;             // �Է��� �߻��� ������ �ð� (Optional)

    // �⺻ ������
   // CInput(DirectionInput dir = NEUTRAL, ButtonInput btn = NONE, int frame = 0)
   //     : direction(dir), button(btn), frameTime(frame) {}
   
    // �⺻ ������
    CInput(DirectionInput dir = MOVEKEY_NEUTRAL, ButtonInput btn = ATTACK_NONE)
        : direction(dir), button(btn) {}


    // �Է� �񱳸� ���� ������ �����ε�
    bool operator==(const CInput& other) const {
        return direction == other.direction && button == other.button;
    }
};

END