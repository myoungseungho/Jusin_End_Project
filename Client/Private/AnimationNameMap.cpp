#include "stdafx.h"
#include "..\Public\AnimationNameMap.h"

#include "RenderInstance.h"
#include "GameInstance.h"


#include <fstream>  
#include <iostream> 
#include <vector>   
#include <string>   
#include <sstream>

#include "Animation.h"


CAnimationNameMap::CAnimationNameMap()
{
}

void CAnimationNameMap::Initalize()
{
	m_MAP_AnimationPedia[SELECT_GOKU]["TEST"] = 0;

	m_MAP_AnimationPedia[PLAY_HIT]["HTN021_body"] = 5;
	m_MAP_AnimationPedia[PLAY_HIT]["HTN403_body"] = 63;


	m_MAP_AnimationPedia[PLAY_HIT]["HTN0403_body"] = 63;

}

_int CAnimationNameMap::Get_AnimationIndex(_int Characterenum, string strAnimationName)
{
	_int iDebug = m_MAP_AnimationPedia[Characterenum][strAnimationName];
	_bool bDebug = true;
	return m_MAP_AnimationPedia[Characterenum][strAnimationName];
}

