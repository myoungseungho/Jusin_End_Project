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
	vector<thread> m_Workers; //��Ŀ ������ ����
	queue<function<void()>> m_Tasks; //�۾� ť
	mutex m_QueueMutex; //ť ��ȣ�� ���� ���ؽ�
	condition_variable m_Condition; //������ ����ȭ�� ���� ����
	_bool m_bStop = { false }; //������ Ǯ ���� �÷���
	size_t m_ThreadCount = 1; //������ ����

public:
	static CThreadPool* Create();
	virtual void Free() override;
};

END

//�۾� ť�� ���ο� �۾��� �߰��մϴ�.
template<typename FunctionType>
inline void CThreadPool::EnqueueTask(FunctionType task)
{
	{
		unique_lock<mutex> lock(m_QueueMutex);
		if (m_bStop)
			throw runtime_error("ThreadPool is stopped.");

		m_Tasks.emplace(task);
	}
	m_Condition.notify_one();
}
