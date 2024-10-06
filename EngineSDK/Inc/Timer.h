#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer final : public CBase
{
private:
	CTimer();
	virtual ~CTimer() = default;

public:
	HRESULT Initialize();
	_float Compute_TimeDelta();

	void Set_UnscaledDeltaTime(_float fDeltaTime) { m_fUnscaledDeltaTime = fDeltaTime; }
	void Set_ScaledDeltaTime(_float fDeltaTime) { m_fScaledDeltaTime = fDeltaTime; }

	_float Get_UnscaledDeltaTime() const { return m_fUnscaledDeltaTime; }
	_float Get_ScaledDeltaTime() const { return m_fScaledDeltaTime; }

private:
	LARGE_INTEGER			m_CurTime;
	LARGE_INTEGER			m_OldTime;
	LARGE_INTEGER			m_OriginTime;
	LARGE_INTEGER			m_CpuTick;

	_float m_fUnscaledDeltaTime = 0.f;
	_float m_fScaledDeltaTime = 0.f;

public:
	static CTimer* Create();
	virtual void Free() override;
};

//GetTickCount()										QueryPerformanceCounter()
//
//os ����(������ �����Ǵ� �������� �ð� ���� ����)				�ϵ���� ����(��ǻ�Ͱ� ���õǴ� �������� �ð� ���� ����)
//1 / 1000 ��											1 / 1,000,000 ��
//��� ������												���� �庮�� ����
//														��������� ���ȼ��� �پ

END