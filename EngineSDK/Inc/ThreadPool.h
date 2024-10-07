#pragma once

#include "Base.h"
#include <thread>
#include <mutex>

BEGIN(Engine)

class ENGINE_DLL CThreadPool final : public CBase
{
public:
	HRESULT Initialize(size_t ThreadCount);

	template<typename FunctionType>
	void EnqueueTask(FunctionType task);
	void Shutdown();
private:
	CThreadPool();
	virtual ~CThreadPool() = default;
	void WorkerThread();

private:
	vector<thread> m_Workers; //워커 스레드 모음
	queue<function<void()>> m_Tasks; //작업 큐
	mutex m_QueueMutex; //큐 보호를 위한 뮤텍스
	condition_variable m_Condition; //스레드 동기화를 위한 변수
	_bool m_bStop = { false }; //스레드 풀 종료 플래그
	size_t m_ThreadCount = 1; //스레드 개수

public:
	static CThreadPool* Create();
	virtual void Free() override;
};

END

template<typename FunctionType>
inline void CThreadPool::EnqueueTask(FunctionType task)
{
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		if (m_bStop)
			throw std::runtime_error("ThreadPool is stopped.");

		m_Tasks.emplace(task);
	}
	m_Condition.notify_one();
}
