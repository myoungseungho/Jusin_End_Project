#include "Timer.h"

CTimer::CTimer()
{
	ZeroMemory(&m_OldTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CurTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_OriginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

HRESULT CTimer::Initialize()
{
	//QueryPerformanceCounter : ���κ��尡 ���� ���ػ� Ÿ�̸��� ���� ���� ������ �Լ�

	QueryPerformanceCounter(&m_CurTime);		// 1000
	QueryPerformanceCounter(&m_OldTime);		// 1024
	QueryPerformanceCounter(&m_OriginTime);		// 1030

	// QueryPerformanceFrequency : ���ػ� Ÿ�̸��� ���ļ��� ������ �Լ�, ���⼭ ���ļ��� cpu�� �ʴ� Ŭ������ �ǹ�
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

_float CTimer::Compute_TimeDelta()
{
	QueryPerformanceCounter(&m_CurTime);

	m_fUnscaledDeltaTime = _float(m_CurTime.QuadPart - m_OldTime.QuadPart) / _float(m_CpuTick.QuadPart);

	m_OldTime = m_CurTime;

	return m_fUnscaledDeltaTime;
}

CTimer * CTimer::Create()
{
	CTimer*		pInstance = new CTimer();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CTimer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTimer::Free()
{
	__super::Free();

}
