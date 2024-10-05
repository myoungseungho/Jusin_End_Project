#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CIMGUI_Tab : public CBase
{

protected:
	CIMGUI_Tab();
	virtual ~CIMGUI_Tab() = default;

public:
	virtual HRESULT Initialize() = 0;
	virtual void Render(_float fTimeDelta) = 0;

public:
	virtual const _char* GetTabName() const = 0;
	virtual void Free() = 0;
};

END
