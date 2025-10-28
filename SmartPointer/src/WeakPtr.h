#pragma once
template<typename T>
class WeakPtr
{
public:
	WeakPtr() : m_Raw(nullptr), m_ControlBlock(nullptr) {}
	WeakPtr(const SharedPtr<T>& sPtr)
		: m_Raw(sPtr.m_Raw), m_ControlBlock(sPtr.m_ControlBlock)
	{
		m_ControlBlock->WeakCounter++;
	}
	~WeakPtr() { Release(); }
	//��������Ϳ�����ֵ
	WeakPtr(const WeakPtr& other)
		:m_Raw(other.m_Raw), m_ControlBlock(other.m_ControlBlock)
	{
		m_ControlBlock->WeakCounter++;
	}
	WeakPtr& operator=(const WeakPtr& other)
	{
		if (this == &other)return *this;
		//�ͷ��Լ���ռ��
		Release();
		//ָ����˵���Դ
		m_Raw = other.m_Raw;
		m_ControlBlock = other.m_ControlBlock;
	}
	//�ƶ�������ƶ���ֵ
	WeakPtr(WeakPtr&& other) noexcept
		:m_Raw(other.m_Raw), m_ControlBlock(other.m_ControlBlock)
	{
		other.m_Raw = nullptr;
		other.m_ControlBlock = nullptr;
	}
	WeakPtr& operator=(WeakPtr&& other) noexcept
	{
		if (this == &other)return *this;
		//�ͷ��Լ���ռ��
		Release();
		//ָ����˵���Դ
		m_Raw = other.m_Raw;
		m_ControlBlock = other.m_ControlBlock;
		//�����˵���Դ�ÿ�
		other.m_Raw = nullptr;
		other.m_ControlBlock = nullptr;
	}

	//�����ĺ���
	inline bool Expired() { return m_ControlBlock->WeakCounter == 0; }
	inline SharedPtr<T> Lock() { return SharedPtr<T>(m_Raw, m_ControlBlock); }

private:
	void Release()
	{
		if (m_ControlBlock)
		{
			if (--m_ControlBlock->WeakCounter == 0 && m_ControlBlock->SharedCounter == 0)
			{
				delete m_ControlBlock;
				m_ControlBlock = nullptr;
			}
		}

	}

private:
	T* m_Raw;
	ControlBlock* m_ControlBlock;
};