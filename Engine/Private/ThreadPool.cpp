#include "..\Public\ThreadPool.h"


CThreadPool::CThreadPool()
{

}

HRESULT CThreadPool::Initialize(size_t ThreadCount)
{
    m_ThreadCount = ThreadCount;

    try
    {
        for (size_t i = 0; i < ThreadCount; ++i)
        {
            m_Workers.emplace_back(thread(&CThreadPool::WorkerThread, this));
        }
    }
    catch (const exception& e)
    {
        return E_FAIL;
    }
    return S_OK;
}

//�� ��Ŀ �����尡 �����ϴ� �Լ��Դϴ�. �۾� ť���� �۾��� �����ͼ� �����մϴ�.
void CThreadPool::WorkerThread()
{
    while (true)
    {
        function<void()> task;

        {
            // �۾� ť�� �����ϰ� �����ϱ� ���� ���ؽ��� ��޴ϴ�.
            unique_lock<mutex> lock(m_QueueMutex);

            //- ���� ���� `m_Condition`�� ����Ͽ� �۾��� �߰��ǰų� ������Ǯ ���� ��ȣ(`m_bStop`)�� �� ������ ����մϴ�.
            //- ���� : `m_bStop`�� `true`�̰ų� `m_Tasks`�� �۾��� ���� ������ ����մϴ�.
            m_Condition.wait(lock, [this] { return m_bStop || !m_Tasks.empty(); });

            //������Ǯ ���� ��ȣ(m_bStop�� true)�̰�, ó���� �۾��� ���ٸ� �����带 �����մϴ�.
            if (m_bStop && m_Tasks.empty())
                return;

            //�۾� ť���� ���� �տ� �ִ� �۾��� ������ task�� �����ϰ�, ť���� �����մϴ�.
            task = move(m_Tasks.front());
            m_Tasks.pop();
        }

        //������ �۾��� �����մϴ�.
        task();
    }
}


//������Ǯ�� �����ϰ� ��� �����带 �����մϴ�.
void CThreadPool::Shutdown()
{
    {
        //�۾� ť�� ��ȣ�ϱ� ���� ���ؽ��� ��װ�, ������Ǯ ���� ��ȣ�� true�� �����մϴ�.
        unique_lock<mutex> lock(m_QueueMutex);
        m_bStop = true;
    }
    //��� ���� ��� �����忡�� ��ȣ�� ������ ����ϴ�. ��������� m_bStop�� Ȯ���ϰ� ���� ������ ����ϴ�.
    m_Condition.notify_all();

    for (thread& worker : m_Workers)
    {
        //��� ��Ŀ �����尡 ����� ������ ��ٸ��ϴ�.
        //`join()`�� �����尡 ����� ������ ����ϴ� �Լ��Դϴ�


        //�۾� ���� ���� : � ������ ���� �ϰ� �ִٸ�, �� ������ ���� ��ĥ ������ ���� ����մϴ�.
        //ȸ�� �� �ݱ� : ȸ�簡 ���� �ݴ´ٴ� ������ ��������, ���� ���� �������� ���� �������� �ٷ� ����մϴ�.
        //���� ��ģ ���� : ���� ��ģ ������ �� �̻� ���ο� ���� ���� �ʰ� ����մϴ�.
        //��� ������ ����� ������ ��ٸ� : ȸ��� ��� ������ �����ϰ� ����� ������ ��ٸ��ϴ�.
        if (worker.joinable())
            worker.join();
    }
}

CThreadPool* CThreadPool::Create()
{
	CThreadPool* pInstance = new CThreadPool();

	return pInstance;
}

void CThreadPool::Free()
{
	__super::Free();
}
