#pragma once

#include <process.h>
#include "GameInstance.h"

namespace Client
{
	
	//��� �����ӿ� �ϸ� ĳ���ʹ� �� 80�� �ִϸ��̼�.  ���� �ϸ�  1600����Ʈ   4���̴� 6400����Ʈ

	//play animation �߿� ���� ĳ���͸� �̿��� ����, ���� �ִϸ��̼��� 


	const static enum CHARACTER_INDEX {
		SELECT_GOKU, SELECT_BOU, SELECT_21, SELECT_HIT,
		PLAY_GOKU = 0, PLAY_BOU, PLAY_21, PLAY_HIT, CHARACTER_INDEX_END
	};


	using Event = list<string>;
	using FrameEvent_KeyFrame = map<_float, Event>;
	using FrameEvent_AnimationIndex = map<_int, FrameEvent_KeyFrame>; //�ִϸ��̼� �ε��� ����
	using FrameEventMap = map<CHARACTER_INDEX, FrameEvent_AnimationIndex>;  //ĳ����enum,������



	//FrameEventMap FrameEvent;


	//1. ĳ���ͷ� ����
	//2. �ִϸ��̼� �ε����� ����
	//3. ���������� ����
	 
	//map<_float, _float>FrameEvent_KeyFrame;
	//map<_int, FrameEvent_KeyFrame>FrameEvent_AnimationIndex; //�ִϸ��̼� �ε��� ����
	//map<CHARACTER_INDEX, Frame_AnimationIndex>FrameEvent;  //ĳ����enum,������
	
	 
	//[ĳ����enum][�ִϸ��̼�index][Ű������] =  ��¼��;

	//FrameEvent[CHARACTER_INDEX_END][287][10] = 5.f;

	//FrameEvent[PLAY_GOKU][IDLE][5].push_back("test");
	
	//FrameEvent[PLAY_GOKU][IDLE][0].push_back(�ӵ�2��);
	//FrameEvent[PLAY_GOKU][ATTACK][2].push_back(�浹ó��);



	//�̺�Ʈ�� �����Ҷ��� ������Ʈ ���� �����Ӱ�  ������Ʈ ���� ������ ������ ���� ������




}
