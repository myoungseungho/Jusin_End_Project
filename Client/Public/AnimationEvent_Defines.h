#pragma once

#include <process.h>
#include "GameInstance.h"

namespace Client
{
	
	//모든 프레임에 하면 캐릭터당 약 80개 애니메이션.  전부 하면  1600바이트   4명이니 6400바이트

	//play animation 중에 현재 캐릭터를 이용해 들어옴, 현재 애니메이션을 


	const static enum CHARACTER_INDEX {
		SELECT_GOKU, SELECT_BOU, SELECT_21, SELECT_HIT,
		PLAY_GOKU = 0, PLAY_BOU, PLAY_21, PLAY_HIT, CHARACTER_INDEX_END
	};


	using Event = list<string>;
	using FrameEvent_KeyFrame = map<_float, Event>;
	using FrameEvent_AnimationIndex = map<_int, FrameEvent_KeyFrame>; //애니메이션 인덱스 구분
	using FrameEventMap = map<CHARACTER_INDEX, FrameEvent_AnimationIndex>;  //캐릭터enum,프레임



	//FrameEventMap FrameEvent;


	//1. 캐릭터로 구분
	//2. 애니메이션 인덱스로 구분
	//3. 프레임으로 구분
	 
	//map<_float, _float>FrameEvent_KeyFrame;
	//map<_int, FrameEvent_KeyFrame>FrameEvent_AnimationIndex; //애니메이션 인덱스 구분
	//map<CHARACTER_INDEX, Frame_AnimationIndex>FrameEvent;  //캐릭터enum,프레임
	
	 
	//[캐릭터enum][애니메이션index][키프레임] =  어쩌구;

	//FrameEvent[CHARACTER_INDEX_END][287][10] = 5.f;

	//FrameEvent[PLAY_GOKU][IDLE][5].push_back("test");
	
	//FrameEvent[PLAY_GOKU][IDLE][0].push_back(속도2배);
	//FrameEvent[PLAY_GOKU][ATTACK][2].push_back(충돌처리);



	//이벤트를 실행할때는 업데이트 이전 프레임과  업데이트 이후 프레임 사이의 모든걸 실행함




}
