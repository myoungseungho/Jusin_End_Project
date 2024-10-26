#pragma once

#include "Client_Defines.h"
#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

BEGIN(Client)

class CUI_Define
{
public:
	enum PLAYER_SLOT { LPLAYER1, LPLAYER2, RPLAYER1, RPLAYER2, SLOT_END };
	enum PLAYER_ID { GOKU, ANDROID21, BUU, HIT, PAWN_END };
	enum UI_LISTPOS { TOP, MID, BOT, LIST_END };

};

END

