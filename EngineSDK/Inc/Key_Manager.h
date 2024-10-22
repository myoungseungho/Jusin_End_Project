#pragma once
#include "Base.h"
#define			VK_MAX		0xff
/* ������ ��ü���� �����Ѵ�. */
/* Prototype ������� ��ü�� �߰��Ѵ�. */


BEGIN(Engine)


class CKey_Manager final : public CBase
{
private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	HRESULT Initialize();
	


	_bool Key_Down(_uint _iKey);
	_bool Key_Pressing(_uint _iKey);
	_bool Key_Up(_uint _iKey);

	void Update();


private:

	bool m_bKeyState[VK_MAX];

public:
	//static CKey_Manager* Create();
	//virtual void Free() override;
	
	static CKey_Manager* Create();
	virtual void Free() override;


};
END
