#pragma once

#include "Base.h"
#include <thread>
#include <mutex>

BEGIN(Engine)

class ENGINE_DLL CThreadPool final : public CBase
{
public:
	HRESULT Initialize(size_t ThreadCount);


	void Shutdown();
	_uint Get_ThreadNumber() { return m_ThreadCount; };

	template<typename FunctionType>
	future<typename result_of<FunctionType()>::type> EnqueueTask(FunctionType task);

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

//���� �Լ� Ÿ���� ��� ó���ϱ� ���� '���ø�'���� �����.
// ���ø� �Լ��� ���Ǵ� ��� ���Ͽ� ���ԵǾ�� �մϴ�.
template<typename FunctionType>
future<typename result_of<FunctionType()>::type> CThreadPool::EnqueueTask(FunctionType task)
{
	using return_type = typename result_of<FunctionType()>::type;

	auto packagedTask = make_shared<packaged_task<return_type()>>(task);
	future<return_type> fut = packagedTask->get_future();

	{
		unique_lock<mutex> lock(m_QueueMutex);
		if (m_bStop)
			throw runtime_error("ThreadPool is stopped.");

		m_Tasks.emplace([packagedTask]() { (*packagedTask)(); });
	}
	m_Condition.notify_one();
	return fut;
}


END